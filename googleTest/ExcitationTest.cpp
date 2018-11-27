#include "stdafx.h"
#include "Data.h"
#include "IntegrationRWG.h"
#include "Planewave.h"
#ifdef GTEST
#include "gtest/gtest.h"
#include "CoreAPI.h"
using namespace Core;
using namespace Eigen;

class ExcitationTest :public testing::Test
{
public:
	static void SetUpTestCase()
	{
		try
		{
			ASSERT_EQ(0, Core::CreatMesh()) << "Error in Creat Mesh";
			ASSERT_EQ(0, Core::CreatBasicFunction(false)) << "Error in Load BasicFunction";
			ASSERT_EQ(0, Core::SetGreenFunction()) << "Error in set Green Function";
			//srand(static_cast<unsigned>(time(nullptr)));
		}
		catch (spd::spdlog_ex&ex)
		{
			Console->warn(ex.what());
			RuntimeL->warn(ex.what());
			RuntimeL->flush();
		}
	}

	static void TearDownTestCase()
	{
	}
};


TEST_F(ExcitationTest, PlanewaveTest)
{
	try
	{
		Console->debug("Right Hand Test");
		char* excitationName = "E:/ZMC/Code/C_program/MUL/SourceData/Excitation.txt";
		//ki=(0,0,-1),ei=(0,1,0);
		const size_t size = ComponentList::BFvector.size();
		Source::Planewave source(&SystemConfig.SourceConfig);
		VectorXcd comp = source.SetExcitation(ComponentList::BFvector);
		VectorXcd ref{ size };
		ifstream ifile(excitationName, ios::in);

		//Read Excitation
		if (ifile.is_open())
		{
			stringstream buffer;
			buffer << ifile.rdbuf();
			ifile.close();
			//Split the words
			string word;
			buffer >> word;
			int l = stoi(word), zmc = 0;
			while (buffer >> word)
			{
				ref[zmc].real(stod(word));
				buffer >> word;
				ref[zmc++].imag(stod(word));
			}
		}

		for (auto i = 0; i < size; ++i)
		{
			EXPECT_NEAR(0, norm(ref(i) - comp(i)) / norm(ref(i)), 5.0e-3) << "Error in Excitation term\t" << i;
		}
	}
	catch (spd::spdlog_ex&ex)
	{
		Console->warn(ex.what());
		RuntimeL->warn(ex.what());
		RuntimeL->flush();
	}
}

#endif