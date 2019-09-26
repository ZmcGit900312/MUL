#include "stdafx.h"

#ifdef GTEST
#include "Data.h"
#include "IntegrationRWG.h"
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
			if (Mesh::GetInstance()->IsLock())ASSERT_EQ(0, Core::CreatMesh()) << "Error in Creat Mesh";
			if(ComponentList::BFvector.size()<1)ASSERT_EQ(0, Core::CreatBasicFunction(false)) << "Error in Load BasicFunction";
			if (!Core::IGreen::GetInstance())EXPECT_EQ(0, Core::SetGreenFunction());
		}
		catch (spd::spdlog_ex&ex)
		{
			Console->warn(ex.what());
			RuntimeLog->warn(ex.what());
			RuntimeLog->flush();
		}
	}

	static void TearDownTestCase()
	{
	}
};

TEST_F(RequestTest, BenchmarkTest)
{
	Request::FarField post(&ComponentList::BFvector, Mesh::GetInstance());
	const double coef = post.Radius*post.Radius * 4 * M_PI / (1 * 1);
	const int thetaNum = 181, phiNum = 1;
	const clock_t start = clock();
	for (int ph = 0; ph < phiNum; ++ph)
	{
		for (int th = 1;th < thetaNum; ++th)
		{
			const double theta = 0 + th * 1;
			const double phi = 0 + ph * 1;
			Vector3cd temp = post.EFieldBenchMark(theta*M_PI_180, phi*M_PI_180);
			Console->debug("({0},{1}) = {2:12.7e} m^2", theta,phi,coef * temp.squaredNorm());
		}
	}

	const clock_t end = clock();
	double timecost = double(end - start) / CLOCKS_PER_SEC;
	Console->info("FarField Calculate cost {}s", timecost);

}

TEST_F(RequestTest, FarFieldTest)
{
	Request::FarField post(&ComponentList::BFvector, Mesh::GetInstance());
	const double coef = post.Coef;
	const int thetaNum = 181, phiNum = 1;
	double newTimeCost = 0.0, oldTimeCost = 0.0;

	Console->debug("Vertical RCS Node={0}*{1}={2}", thetaNum, phiNum, thetaNum*phiNum);

	ofstream ofs("E:/ZMC/Code/C_program/MUL/SourceData/RCSTest.csv",ios_base::out);
	ofs.flags(ios::left);
	if (!ofs.is_open())GTEST_FAIL()<<"Can't construct RCSTest.csv file!";
	ofs << "Theta" << ',' << "Phi" << ',' << "RCS(m2)" << '\n';
	for (int ph = 0; ph < phiNum; ++ph)
	{
		for (int th = 0;th < thetaNum; ++th)
		{
			const double theta = 0 + th * 1;
			const double phi = 0 + ph * 1;

			clock_t newStart = clock();
			Vector3cd efield(post.EField(theta*M_PI_180, phi*M_PI_180));
			clock_t newEnd = clock();
			newTimeCost += double(newEnd - newStart) / CLOCKS_PER_SEC;
			
			clock_t oldStart = clock();
			Vector3cd efieldRef(post.EFieldBenchMark(theta*M_PI_180, phi*M_PI_180));
			clock_t oldEnd = clock();
			oldTimeCost+= double(oldEnd - oldStart) / CLOCKS_PER_SEC;
			
			EXPECT_NEAR(efieldRef.norm(), efield.norm(), 1.0e-5) << "error in FarField Request (th,phi)=" << theta << phi;
			//cout << "Ref:\t" << efieldRef.transpose() << '\n' << "Cal:\t" << efield.transpose() << endl;
			Console->debug("({0},{1}) = {2:12.7e} m^2,{3:12.7e} m^2", theta, phi, 
				coef * efieldRef.squaredNorm(),coef * efield.squaredNorm());
			ofs << theta << ',' << phi << ',' << coef * efield.squaredNorm() << "\n";
		}
	}
	ofs.flush();
	ofs.close();

	Console->info("New method of FarField Calculate cost {}s", newTimeCost);
	Console->info("Old method of FarField Calculate cost {}s", oldTimeCost);
}


TEST_F(RequestTest, FarFieldArrayTest)
{
	Request::FarField post(&ComponentList::BFvector, Mesh::GetInstance());
	const double coef = post.Coef;
	const int thetaNum = 181, phiNum = 1;
	const clock_t start = clock();
	ofstream ofs("E:/ZMC/Code/C_program/MUL/SourceData/RCSTest.csv", ios_base::out);
	ofs.flags(ios::left);
	if (!ofs.is_open())GTEST_FAIL() << "Can't construct RCSTest.csv file!";
	ofs << "Theta" << ',' << "Phi" << ',' << "RCS(m2)" << '\n';
	for (int ph = 0; ph < phiNum; ++ph)
	{
		for (int th = 0;th < thetaNum; ++th)
		{
			const double theta = 0 + th * 1;
			const double phi = 0 + ph * 1;
			Vector3cd efield(post.EFieldArray(theta*M_PI_180, phi*M_PI_180));
			Vector3cd efieldRef(post.EFieldArrayBenchMark(theta*M_PI_180, phi*M_PI_180));
			EXPECT_NEAR(efieldRef.norm(), efield.norm(), 1.0e-5) << "error in FarField Request (th,phi)=" << theta << phi;	
			Console->debug("({0},{1}) = {2:12.7e} m^2,{3:12.7e} m^2", theta, phi,
				coef * efieldRef.squaredNorm(), coef * efield.squaredNorm());
			ofs << theta << ',' << phi << ',' << coef * efield.squaredNorm() << "\n";
		}
	}
	ofs.flush();
	ofs.close();
	const clock_t end = clock();
	double timecost = double(end - start) / CLOCKS_PER_SEC;
	Console->info("FarField Calculate cost {}s", timecost);

}
#endif