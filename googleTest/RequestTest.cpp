#include "stdafx.h"

#ifdef GTEST
#include "Data.h"
#include "FarField.h"
#include "gtest/gtest.h"
#include "CoreAPI.h"
#include "Const.h"
using namespace Core;
using namespace Eigen;

class RequestTest :public testing::Test
{
public:
	static void SetUpTestCase()
	{
		try
		{
			SystemConfig.ImpConfig.ImpType = Core::AIM;
			SystemConfig.ImpConfig.ArrayNumX = 4;
			SystemConfig.ImpConfig.ArrayNumY = 4;
			SystemConfig.ImpConfig.ArrayIntervalX = 1.5;
			SystemConfig.ImpConfig.ArrayIntervalY = 1.5;
			SystemConfig.IEConfig.type = EFIE;
			SystemConfig.SolverConfig.Precond = Solution::ILU;

			if (Mesh::GetInstance()->IsLock())ASSERT_EQ(0, Core::CreatMesh()) << "Error in Creat Mesh";
			if(ComponentList::BFvector.size()<1)ASSERT_EQ(0, Core::CreatBasisFunction(false)) << "Error in Load BasicFunction";
			if (!Core::IGreen::GetInstance())EXPECT_EQ(0, Core::SetGreenFunction());


			LoadCurrentBenchMarkBinary();
		}
		catch (spd::spdlog_ex&ex)
		{
			Console->warn(ex.what());
			Runtime->warn(ex.what());
			Runtime->flush();
		}
	}

	static void TearDownTestCase()
	{
	}

	static void LoadCurrentBenchMarkBinary()
	{
		string filename = SystemConfig.ProjectDir + "\\" + "CurrentBenchMark.dat";
		ifstream ifs(filename, ios::in | ios::binary);
		size_t unknowns = 0;
		if (!ifs.is_open()) {
			SaveCurrentBenchMarkBinary();
			ifs.open(filename, ios::in | ios::binary);
		}

		ifs.read(reinterpret_cast<char*>(&unknowns), sizeof(size_t));
		CurrentBenchMark.resize(unknowns);

		for (size_t zmc = 0;zmc < unknowns;zmc++)
		{
			ifs.read(reinterpret_cast<char*>(&CurrentBenchMark[zmc]), sizeof(dcomplex));
		}

		ifs.clear();
		ifs.close();
	}

	static void SaveCurrentBenchMarkBinary()
	{
		string filename = SystemConfig.ProjectDir + "\\" + "CurrentBenchMark.dat";
		auto bf = ComponentList::BFvector;
		size_t unknowns = bf.size();

		ofstream ofs(filename, ios::out | ios::binary | ios::trunc);

		if (ofs.is_open())
		{

			ofs.write(reinterpret_cast<char*>(&unknowns), sizeof(size_t));

			for (auto val : bf)
			{
				ofs.write(reinterpret_cast<char*>(&val->Current()), sizeof(dcomplex));
			}
			ofs.flush();
			ofs.close();
		}
	}

	static VectorXcd CurrentBenchMark;
};

VectorXcd RequestTest::CurrentBenchMark;


TEST_F(RequestTest, BenchMarkTest)
{
	
	Request::FarField post(&ComponentList::BFvector, Mesh::GetInstance());
	const double coef = post.Coef;
	const int thetaNum = 181, phiNum = 1;

	ofstream ofs("E:/ZMC/Code/C_program/MUL/SourceData/RCSTest.csv", ios_base::out);
	ofs.flags(ios::left);
	if (!ofs.is_open())GTEST_FAIL() << "Can't construct RCSTest.csv file!";
	ofs << "Theta" << ',' << "Phi" << ',' << "RCS(m2)" << '\n';


	const clock_t start = clock();
	for (int ph = 0; ph < phiNum; ++ph)
	{
		for (int th = 1;th < thetaNum; ++th)
		{
			const double theta = 0 + th * 1;
			const double phi = 0 + ph * 1;
			Vector3cd efield(post.EFieldBenchMark(theta*M_PI_180, phi*M_PI_180, CurrentBenchMark));
			Console->debug("({0},{1}) = {2:12.7e} m^2", theta,phi,coef * efield.squaredNorm());
			ofs << theta << ',' << phi << ',' << coef * efield.squaredNorm() << "\n";
		}
	}
	const clock_t end = clock();
	ofs.flush();
	ofs.close();

	double timecost = double(end - start) / CLOCKS_PER_SEC;
	Console->info("FarField Calculate cost {}s", timecost);

}

TEST_F(RequestTest, FarFieldTest)
{
	try
	{
#pragma region GenerateCurrent
		//Current Initial
		auto Cinfo = Solution::CurrentInfo::GetInstance();
		auto* bf = &ComponentList::BFvector;
		size_t unknowns = ComponentList::BFvector.size();
		string tag = "ElementCurrent";

		Solution::ElementCurrent* current = new Solution::ElementCurrent(unknowns, Frequency, tag);

		Console->debug("Generate the current from bf...");

		for (size_t zmc = 0;zmc < CurrentBenchMark.size();zmc++)
			current->_data.push_back(CurrentBenchMark[zmc]);

		Cinfo->Reformat(AIM);
		Cinfo->_numberOfConfig = 1;
		Cinfo->Current.push_back(current);
#pragma endregion

		//Request Part
		Request::FarField post(bf, Mesh::GetInstance(), Cinfo);
		const double coef = post.Coef;
		const int thetaNum = 181, phiNum = 1;
		double newTimeCost = 0.0, oldTimeCost = 0.0;

		Console->debug("Vertical RCS Node={0}*{1}={2}", thetaNum, phiNum, thetaNum*phiNum);

		for (int ph = 0; ph < phiNum; ++ph)
		{
			for (int th = 0;th < thetaNum; ++th)
			{
				const double theta = 0 + th * 1;
				const double phi = 0 + ph * 1;

				clock_t newStart = clock();
				Vector3cd efield(post.EField(theta*M_PI_180, phi*M_PI_180, current));
				clock_t newEnd = clock();
				newTimeCost += double(newEnd - newStart) / CLOCKS_PER_SEC;

				clock_t oldStart = clock();
				Vector3cd efieldRef(post.EFieldBenchMark(theta*M_PI_180, phi*M_PI_180, CurrentBenchMark));
				clock_t oldEnd = clock();
				oldTimeCost += double(oldEnd - oldStart) / CLOCKS_PER_SEC;

				EXPECT_NEAR(efieldRef.norm(), efield.norm(), 1.0e-5) << "error in FarField Request (th,phi)=" << theta << phi;
				//cout << "Ref:\t" << efieldRef.transpose() << '\n' << "Cal:\t" << efield.transpose() << endl;
				Console->debug("({0},{1}) in Ref={2:12.7e} m^2,Test={3:12.7e} m^2", theta, phi,
					coef * efieldRef.squaredNorm(), coef * efield.squaredNorm());
				
			}
		}
		

		Console->info("New method of FarField Calculate cost {}s", newTimeCost);
		Console->info("Old method of FarField Calculate cost {}s", oldTimeCost);
	}
	catch (spd::spdlog_ex& message)
	{
		Assist::LogException(message);
		FAIL();
	}
}


TEST_F(RequestTest, FarFieldArrayTest)
{
	try
	{
		auto bf = ComponentList::BFvector;
		auto info = Solution::CurrentInfo::GetInstance();
		size_t elementUnknowns = ComponentList::BFvector.size();

#pragma region GenerateCurrent
		//Current Initial
		string tag = "ArrayCurrent";
		int num = 1;
		double bx = SystemConfig.ImpConfig.ArrayIntervalX, by = SystemConfig.ImpConfig.ArrayIntervalY;
		size_t numE = 12;//Sparse Array

		Matrix<bool, 4, 4> arrayConfig;
		arrayConfig.array() = true;
		arrayConfig(0, 0) = false;
		arrayConfig(1, 1) = false;
		arrayConfig(2, 2) = false;
		arrayConfig(1, 3) = false;
		/*	A	A	A	A
		 *	A	A	x	A
		 *	A	x	A	x
		 *	x	A	A	A
		 */

		Solution::ArrayCurrent* current = new Solution::ArrayCurrent(elementUnknowns,
			Frequency, tag,  numE, bx, by);

		info->Reformat(Core::Array);
		info->_numberOfConfig = num;

		Console->debug("Generate the current from bf...");

		for (int zmcy = 0; zmcy < SystemConfig.ImpConfig.ArrayNumY; ++zmcy)
		{
			for (int zmcx = 0; zmcx < SystemConfig.ImpConfig.ArrayNumX; ++zmcx)
			{
				if (arrayConfig(zmcx, zmcy))
				{
					current->_arrayLocation.push_back({ zmcx,zmcy });
					for (size_t zmc = 0;zmc < CurrentBenchMark.size();zmc++)
						current->_data.push_back(CurrentBenchMark[zmc]);
				}

			}
		}
		info->Current.push_back(current);
#pragma endregion

		//RequestArray Test
		Request::FarField post(&ComponentList::BFvector, Mesh::GetInstance(), info);
		const double coef = post.Coef;
		const int thetaNum = 181, phiNum = 1;
		double newTimeCost = 0.0, oldTimeCost = 0.0;

		for (int ph = 0; ph < phiNum; ++ph)
		{
			for (int th = 0;th < thetaNum; ++th)
			{
				const double theta = 0 + th * 1;
				const double phi = 0 + ph * 1;


				clock_t newStart = clock();
				Vector3cd efield(post.EField(theta*M_PI_180, phi*M_PI_180, current));
				clock_t newEnd = clock();
				newTimeCost += double(newEnd - newStart) / CLOCKS_PER_SEC;

				clock_t oldStart = clock();
				Vector3cd efieldRef(post.EFieldBenchMark(theta*M_PI_180, phi*M_PI_180, CurrentBenchMark,current));
				clock_t oldEnd = clock();
				oldTimeCost += double(oldEnd - oldStart) / CLOCKS_PER_SEC;

				EXPECT_NEAR(efieldRef.norm(), efield.norm(), 1.0e-5) << "error in FarField Request (th,phi)=" << theta << phi;
				Console->debug("({0},{1}) = {2:12.7e} m^2,{3:12.7e} m^2", theta, phi,
					coef * efieldRef.squaredNorm(), coef * efield.squaredNorm());
				
			}
		}

		Console->info("New method of FarField Calculate cost {}s", newTimeCost);
		Console->info("Old method of FarField Calculate cost {}s", oldTimeCost);
	}
	catch (spd::spdlog_ex& message)
	{
		Assist::LogException(message);
		FAIL();
	}
	
}


TEST_F(RequestTest, RCSGetTest)
{

	try
	{
		auto bf = ComponentList::BFvector;
		auto info = Solution::CurrentInfo::GetInstance();
		size_t elementUnknowns = ComponentList::BFvector.size();

#pragma region GenerateCurrent
		//Current Initial
		string tag1 = "MoMCurrent1", tag2 = "MoMCurrent2",tag3="MoMCurrent3";
		int num = 3;	
		size_t unknowns1 = elementUnknowns;		
		size_t unknowns2 = elementUnknowns;
		size_t unknowns3 = elementUnknowns;

		Solution::ElementCurrent* current1 = new Solution::ElementCurrent(unknowns1, Frequency, tag1);
		Solution::ElementCurrent* current2 = new Solution::ElementCurrent(unknowns2, Frequency, tag2);
		Solution::ElementCurrent* current3 = new Solution::ElementCurrent(unknowns3, Frequency, tag3);

		info->Reformat(Core::AIM);
		info->_numberOfConfig = num;

		string filename = SystemConfig.ProjectDir + "\\" + SystemConfig.ProjectName + ".cu";

		Console->debug("Generate the current from bf...");

		for (size_t zmc = 0;zmc < CurrentBenchMark.size();zmc++)
		{
			current1->_data.push_back(CurrentBenchMark[zmc]);
			current2->_data.push_back(CurrentBenchMark[zmc]);
			current3->_data.push_back(CurrentBenchMark[zmc]);
		}
			
		info->Current.push_back(current1);
		info->Current.push_back(current2);
		info->Current.push_back(current3);
#pragma endregion

		//RequestArray Test

		Request::FarField post(&ComponentList::BFvector, Mesh::GetInstance(), info);

		ofstream ofs;
		ofs.flags(ios::left);

		const clock_t start = clock();
		for (auto value : SystemConfig.PostConfig)
		{
			const string savename = SystemConfig.ProjectDir + '\\' + value.FarFileName + "_RCS.csv";
			ofs.open(savename, ios_base::out);
			if (!ofs.is_open())throw spd::spdlog_ex("Save RCS Directory Error in " + savename);

			Console->info("RCS of Far Field::\t" + value.FarFileName);
			post.CalculateRCS(value, ofs);

			ofs.flush();
			ofs.close();
		}
		const clock_t end = clock();
		double timecost = double(end - start) / CLOCKS_PER_SEC;
		Console->info("FarField Calculate cost {:f} s", timecost);

	}
	catch (spd::spdlog_ex& message)
	{
		Assist::LogException(message);
		FAIL();
	}
	
}
#endif