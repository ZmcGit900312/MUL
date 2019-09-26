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
			if (Mesh::GetInstance()->IsLock())ASSERT_EQ(0, Core::CreatMesh()) << "Error in Creat Mesh";
			if (ComponentList::BFvector.size() < 1)ASSERT_EQ(0, Core::CreatBasicFunction(false)) << "Error in Load BasicFunction";
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


TEST_F(CurrentTest, ElementCurrentWriteTest)
{
	try
	{
		Solution::ElementCurrent* current = new Solution::ElementCurrent;
		auto bf = ComponentList::BFvector;
		auto info = Solution::CurrentInfo::GetInstance();
		size_t unknowns = ComponentList::BFvector.size();

		current->ConfigName = "ElementCurrentTest";
		current->_unknowns = ComponentList::BFvector.size();
		current->_frequency = Frequency;
		current->_data.clear();

		string filename = SystemConfig.ProjectDir + "\\" + SystemConfig.ProjectName + ".cu";

		Console->debug("Generate the current from bf...");

		for (auto val : ComponentList::BFvector)
		{
			current->_data.push_back(val->Current());
		}


		info->category = AIM;
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
					EXPECT_LT(norm(current->GetCurrent(zmc) - bf[zmc]->Current()) / norm(bf[zmc]->Current()),1.0e-7);
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

//ÒªÐÞ¸ÄµÄ²âÊÔ
TEST_F(CurrentTest, ArrayCurrentWriteTest)
{
	try
	{
		Solution::ElementCurrent* current = new Solution::ArrayCurrent;
		auto bf = ComponentList::BFvector;
		auto info = Solution::CurrentInfo::GetInstance();
		size_t unknowns = ComponentList::BFvector.size();
		int num = 2;

		info->category = Core::Array;
		info->_numberOfConfig = num;

		current->ConfigName = "ElementCurrentTest";
		current->_unknowns = ComponentList::BFvector.size();
		current->_frequency = Frequency;
		current->_data.clear();

		string filename = SystemConfig.ProjectDir + "\\" + SystemConfig.ProjectName + ".cu";

		Console->debug("Generate the current from bf...");

		/*for (auto val : ComponentList::BFvector)
		{
			current->_data.push_back(val->Current());
		}*/

		info->Current.push_back(current);

		Console->info("Test write ElementCurrent");
		if (info->SaveCurrent(filename.c_str()))
		{
			info->Reformat();
			Console->info("Test Read ElementCurrent");
			if (info->ReadCurrent(filename.c_str()))
			{
				ASSERT_EQ(Core::Array, info->category);
				ASSERT_EQ(num, info->_numberOfConfig);
				current = info->Current[0];
				ASSERT_EQ(unknowns, current->_unknowns);
				ASSERT_LT(abs(current->_frequency - Frequency) / Frequency, 1.0e-8);
				for (auto zmc = 0; zmc < current->_unknowns; ++zmc)
				{
					EXPECT_LT(norm(current->GetCurrent(zmc) - bf[zmc]->Current()) / norm(bf[zmc]->Current()), 1.0e-7);
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