#include "stdafx.h"

#ifdef GTEST
#include "gtest/gtest.h"
#include "CoreAPI.h"
#include "Data.h"
#include "IntegrationRWG.h"
#include "Excitation.h"
using namespace Core;
using namespace Eigen;

class ExcitationTest :public testing::Test
{
public:
	static void SetUpTestCase()
	{
		try
		{
			SystemConfig.ImpConfig.ImpType = MoM;
			if (Mesh::GetInstance()->IsLock())ASSERT_EQ(0, Core::CreatMesh()) << "Error in Creat Mesh";
			if (ComponentList::BFvector.size() < 1)ASSERT_EQ(0, Core::CreatBasisFunction(false)) << "Error in Load BasicFunction";
			if (!Core::IGreen::GetInstance())EXPECT_EQ(0, Core::SetGreenFunction());
			//srand(static_cast<unsigned>(time(nullptr)));
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


TEST_F(ExcitationTest, PlanewaveTest)
{
	try
	{
		Console->debug("Right Hand Test");
		char* excitationName = "E:/ZMC/Code/C_program/MUL/SourceData/Excitation.csv";
		//ki=(0,0,-1),ei=(0,1,0);
		//const size_t size = ComponentList::BFvector.size();
		size_t size=0;
		
		VectorXcd ref;
		ifstream ifile(excitationName, ios::in);

		//Read Excitation
		if (ifile.is_open())
		{
			stringstream buffer;
			buffer << ifile.rdbuf();
			ifile.close();
			//Split the words
			string line,word;
			
			//Read Title
			Vector3d direction;
			getline(buffer, line);
			getline(buffer, word,',');
			int type = stoi(word);
			getline(buffer, word, ',');
			direction.x()=stod(word);
			getline(buffer, word, ',');
			direction.y() = stod(word);
			getline(buffer, word);
			direction.z() = stod(word);
			//SystemConfig.SourceConfig.Ki = direction;

			Vector3d pol;
			getline(buffer, line);
			getline(buffer, word, ',');
			getline(buffer, word, ',');
			pol.x() = stod(word);
			getline(buffer, word, ',');
			pol.y() = stod(word);
			getline(buffer, word);
			pol.z() = stod(word);
			//SystemConfig.SourceConfig.Ei = pol;
			getline(buffer, word);
			size = stoi(word);
			ref.resize(size);

			for(size_t zmc=0;zmc<size;zmc++)
			{
				getline(buffer, word, ',');
				ref[zmc].real(stod(word));
				getline(buffer, word);
				ref[zmc].imag(stod(word));
			}

			Console->info("Direction:[{0} {1} {2}]", direction.x(), direction.y(), direction.z());
			Console->info("Polarization:[{0} {1} {2}]", pol.x(), pol.y(), pol.z());
			
		}
		else throw spdlog::spdlog_ex("Can't open Excitation.csv");

		//Console->info("Direction:[{0} {1} {2}]", SystemConfig.SourceConfig.Ki.x(), SystemConfig.SourceConfig.Ki.y(), SystemConfig.SourceConfig.Ki.z());
		//Console->info("Polarization:[{0} {1} {2}]", SystemConfig.SourceConfig.Ei.x(), SystemConfig.SourceConfig.Ei.y(), SystemConfig.SourceConfig.Ei.z());
		
		//Source::Planewave source(&SystemConfig.SourceConfig);
		VectorXcd comp = SystemConfig.SourceConfig->SetExcitation(ComponentList::BFvector, 
			SystemConfig.ImpConfig);
		for (auto i = 0; i < size; ++i)
		{
			EXPECT_NEAR(0, norm(ref(i) - comp(i)) / norm(ref(i)), 0.005) << "Error in Excitation term\t" << i;
		}
	}
	catch (spd::spdlog_ex&ex)
	{
		Console->warn(ex.what());
		RuntimeLog->warn(ex.what());
		RuntimeLog->flush();
		FAIL();
	}
}

#endif