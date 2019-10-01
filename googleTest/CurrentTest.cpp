#include "stdafx.h"

#ifdef GTEST
#include "Data.h"
#include "Current.h"
#include "gtest/gtest.h"
#include "CoreAPI.h"
#include "Const.h"
using namespace Core;
using namespace Eigen;

class CurrentTest :public testing::Test
{
public:
	static void SetUpTestCase()
	{
		try
		{
			SystemConfig.ImpConfig.ImpType = Core::Array;
			SystemConfig.ImpConfig.ArrayNumX = 4;
			SystemConfig.ImpConfig.ArrayNumY = 4;
			SystemConfig.ImpConfig.ArrayIntervalX = 1.5;
			SystemConfig.ImpConfig.ArrayIntervalY = 1.5;
			SystemConfig.IEConfig.type = EFIE;
			SystemConfig.SolverConfig.Precond = Solution::ILU;
			if (Mesh::GetInstance()->IsLock())ASSERT_EQ(0, Core::CreatMesh()) << "Error in Creat Mesh";
			if (ComponentList::BFvector.size() < 1)ASSERT_EQ(0, Core::CreatBasicFunction(false)) << "Error in Load BasicFunction";
			if (!Core::IGreen::GetInstance())EXPECT_EQ(0, Core::SetGreenFunction());

			LoadCurrentBenchMarkBinary();
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


	static void LoadCurrentBenchMarkBinary()
	{
		string filename = SystemConfig.ProjectDir + "\\" + "CurrentBenchMark.dat";
		ifstream ifs(filename, ios::in | ios::binary);
		size_t unknowns = 0;
		if (!ifs.is_open()) {
			SaveCurrentBenchMarkBinary();
			ifs.open(filename, ios::in | ios::binary);
		}

		ifs.read(reinterpret_cast<char*>(&unknowns),sizeof(size_t));
		CurrentBenchMark.resize(unknowns);

		for(size_t zmc=0;zmc<unknowns;zmc++)
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

		if(ofs.is_open())
		{
			
			ofs.write(reinterpret_cast<char*>(&unknowns), sizeof(size_t));

			for(auto val:bf)
			{
				ofs.write(reinterpret_cast<char*>(&val->Current()), sizeof(dcomplex));
			}
			ofs.flush();
			ofs.close();
		}
	}

	static VectorXcd CurrentBenchMark;
};

VectorXcd CurrentTest::CurrentBenchMark;


TEST_F(CurrentTest, ElementCurrentTest)
{
	try
	{
		
		auto bf = ComponentList::BFvector;
		auto info = Solution::CurrentInfo::GetInstance();
		size_t unknowns = ComponentList::BFvector.size();
		
		//Current Initial
		string tag= "ElementCurrentTest";


		Solution::ElementCurrent* current = new Solution::ElementCurrent(unknowns,Frequency,tag);

		string filename = SystemConfig.CurrentFilePath;

		Console->debug("Generate the current from bf...");

		for (size_t zmc = 0;zmc < CurrentBenchMark.size();zmc++)
			current->_data.push_back(CurrentBenchMark[zmc]);


		info->Reformat(AIM);
		info->_numberOfConfig = 1;
		info->Current.push_back(current);
		
		Console->info("Test write ElementCurrent");
		if (info->SaveCurrent(filename.c_str()))
		{
			info->Reformat();
			Console->info("Test Read ElementCurrent");			
			if (info->ReadCurrent(filename.c_str()))
			{
				int num = 1;
				
				ASSERT_EQ(AIM, info->category);
				ASSERT_EQ(num, info->_numberOfConfig);
				current = info->Current[0];
				ASSERT_EQ(unknowns, current->_unknowns);
				ASSERT_LT(abs(current->_frequency - Frequency) / Frequency, 1.0e-8);
				for (auto zmc = 0; zmc < current->_unknowns; ++zmc)
				{
					EXPECT_LT(norm(current->GetCurrent(zmc) - CurrentBenchMark[zmc]) / norm(CurrentBenchMark[zmc]),1.0e-7);
				}
			}
			else throw spdlog::spdlog_ex("Read Current File Error");
		}
		else throw spdlog::spdlog_ex("Save Current File Error");		

	}
	catch (spd::spdlog_ex& message)
	{
		Assist::LogException(message);
		FAIL();
	}

}

TEST_F(CurrentTest, ArrayCurrentTest)
{
	try
	{
		
		auto bf = ComponentList::BFvector;
		auto info = Solution::CurrentInfo::GetInstance();
		size_t elementUnknowns = ComponentList::BFvector.size();


		//Current Initial
		string tag1 = "ArrayCurrentTest1",tag2="ArrayCurrentTest2";
		int num = 2;
		double bx = SystemConfig.ImpConfig.ArrayIntervalX, by = SystemConfig.ImpConfig.ArrayIntervalY;
		size_t numE1 = SystemConfig.ImpConfig.ArrayNumX*SystemConfig.ImpConfig.ArrayNumY;
		size_t unknowns1 = elementUnknowns * numE1;
		size_t numE2 = 12;//Sparse Array
		size_t unknowns2 = elementUnknowns * numE2;
		
		
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

		Solution::ArrayCurrent* current1 = new Solution::ArrayCurrent(elementUnknowns,
			Frequency,tag1,numE1,bx,by);
		Solution::ArrayCurrent* current2 = new Solution::ArrayCurrent(elementUnknowns,
			Frequency, tag2,  numE2, bx, by);

		info->Reformat(Core::Array);
		info->_numberOfConfig = num;


		//string filename = SystemConfig.ProjectDir + "\\" + SystemConfig.ProjectName + ".cu";
		string filename = SystemConfig.CurrentFilePath;
		Console->debug("Generate the current from bf...");

		for (int zmcy = 0; zmcy < SystemConfig.ImpConfig.ArrayNumY; ++zmcy)
		{
			for (int zmcx = 0; zmcx < SystemConfig.ImpConfig.ArrayNumX; ++zmcx)
			{
				current1->_arrayLocation.push_back({ zmcx,zmcy });
				if (arrayConfig(zmcx, zmcy))current2->_arrayLocation.push_back({ zmcx,zmcy });

				for (size_t zmc = 0;zmc < CurrentBenchMark.size();zmc++)
				{
					current1->_data.push_back(CurrentBenchMark[zmc]);
					if (arrayConfig(zmcx, zmcy))
					{
						current2->_data.push_back(CurrentBenchMark[zmc]);
					}
				}				
			}
		}
		

		info->Current.push_back(current1);
		info->Current.push_back(current2);

		//Test
		Console->info("Test write ElementCurrent");
		if (info->SaveCurrent(filename.c_str()))
		{
			info->Reformat();
			Console->info("Test Read ElementCurrent");
			if (info->ReadCurrent(filename.c_str()))
			{
				ASSERT_EQ(Core::Array, info->category);
				ASSERT_EQ(num, info->_numberOfConfig);
				current1 = static_cast<Solution::ArrayCurrent*>(info->Current[0]);
				current2 = static_cast<Solution::ArrayCurrent*>(info->Current[1]);
				ASSERT_EQ(unknowns1, current1->_unknowns);
				ASSERT_EQ(unknowns2, current2->_unknowns);
				ASSERT_LT(abs(current1->_frequency - Frequency) / Frequency, 1.0e-8);
				ASSERT_LT(abs(current2->_frequency - Frequency) / Frequency, 1.0e-8);
				ASSERT_EQ(elementUnknowns, current1->_elementUnknowns);
				ASSERT_EQ(elementUnknowns, current2->_elementUnknowns);
				ASSERT_EQ(numE1, current1->_numberOfElement);
				ASSERT_EQ(numE2, current2->_numberOfElement);
				ASSERT_EQ(numE1, current1->_arrayLocation.size());
				ASSERT_EQ(numE2, current2->_arrayLocation.size());
				ASSERT_LT(abs(current1->_arrayBiasX - bx) / bx, 1.0e-8);
				ASSERT_LT(abs(current1->_arrayBiasY - by) / by, 1.0e-8);
				ASSERT_LT(abs(current2->_arrayBiasX - bx) / bx, 1.0e-8);
				ASSERT_LT(abs(current2->_arrayBiasY - by) / by, 1.0e-8);

				
				for (int zmcy = 0; zmcy < SystemConfig.ImpConfig.ArrayNumY; ++zmcy)
				{
					for (int zmcx = 0; zmcx < SystemConfig.ImpConfig.ArrayNumX; ++zmcx)
					{
						for (auto zmc = 0; zmc < elementUnknowns; ++zmc)
						{
							EXPECT_LT(norm(current1->GetCurrent(zmc,zmcx,zmcy) - CurrentBenchMark[zmc]) / norm(CurrentBenchMark[zmc]), 1.0e-7);
							if (arrayConfig(zmcx, zmcy))
							{
								EXPECT_LT(norm(current2->GetCurrent(zmc, zmcx, zmcy) - CurrentBenchMark[zmc]) / norm(CurrentBenchMark[zmc]), 1.0e-7);
							}
						}
						
					}
				}
				
			}
			else throw spdlog::spdlog_ex("Read Current File Error");
		}
		else throw spdlog::spdlog_ex("Save Current File Error");

	}
	catch (spd::spdlog_ex& message)
	{
		Assist::LogException(message);
		FAIL();
	}
}
#endif