#include "stdafx.h"

#ifdef GTEST
#include "gtest/gtest.h"
#include "CoreAPI.h"
#include "Data.h"
#include "Const.h"
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
			ASSERT_EQ(0, Core::DataInitialization()) << "Error in Initialization";
			
			srand(static_cast<unsigned>(time(nullptr)));
		}
		catch (spd::spdlog_ex&ex)
		{
			Console->warn(ex.what());
			Runtime->warn(ex.what());
			Runtime->flush();
		}
	}

	//Update Frequency before IE
	static void EMCParameterUpdate(double fre)
	{
		Frequency = fre;
		Omega = 2 * M_PI*Frequency;
		k = Omega / c0;
		Lambda = c0 / Frequency;
	}

	static void TearDownTestCase()
	{
		//Release IE
		if (Core::equation)
		{
			delete equation;
			equation = nullptr;
			Console->debug("Release IE");
		}
	}


	VectorXcd LoadBenchmark(char* filename)const
	{
		size_t size = 0;

		VectorXcd ref;
		ifstream ifile(filename, ios::in);

		//Read Excitation
		if (ifile.is_open())
		{
			stringstream buffer;
			buffer << ifile.rdbuf();
			ifile.close();
			//Split the words
			string line, word;

			//Read Title
			Vector3d direction;
			getline(buffer, line);
			getline(buffer, word, ',');
			int type = stoi(word);
			getline(buffer, word, ',');
			direction.x() = stod(word);
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

			for (size_t zmc = 0;zmc < size;zmc++)
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
		return ref;
	}
};


TEST_F(ExcitationTest, PlanewaveMoM)
{
	try
	{
		Console->debug("Right Hand Test");
		//IE
		EMCParameterUpdate(3.0e8);
		equation = IE::FIE(EFIE);

		char* excitationName = "E:/ZMC/Code/C_program/MUL/SourceData/Excitation.csv";
		//ki=(0,0,-1),ei=(0,1,0);
		//const size_t size = ComponentList::BFvector.size();
		
		
		VectorXcd ref= LoadBenchmark(excitationName);
		size_t size = ref.size();

		VectorXcd comp = SystemConfig.SourceConfig->SetExcitation(ComponentList::BFvector, 
			SystemConfig.ImpConfig);
		for (auto i = 0; i < size; ++i)
		{
			EXPECT_NEAR(0, norm(ref(i) - comp(i)) / norm(ref(i)), 0.005) << "Error in Excitation term\t" << i;
		}
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		FAIL();
	}
}


TEST_F(ExcitationTest, PlanewaveArray)
{
	try
	{
		SystemConfig.ImpConfig.ImpType = Core::Array;
		SystemConfig.ImpConfig.ArrayNumX = 4;
		SystemConfig.ImpConfig.ArrayNumY = 4;
		SystemConfig.ImpConfig.ArrayIntervalX = 1.5;
		SystemConfig.ImpConfig.ArrayIntervalY = 1.5;
		//IE
		EMCParameterUpdate(3.0e8);
		equation = IE::FIE(EFIE);

		char* excitationName = "E:/ZMC/Code/C_program/MUL/SourceData/Excitation.csv";

		VectorXcd ref = LoadBenchmark(excitationName);
		size_t size = ref.size();

		VectorXcd comp = SystemConfig.SourceConfig->SetExcitation(ComponentList::BFvector,
			SystemConfig.ImpConfig);
		Console->debug("Size of Element is :\t{0}", size);
		Console->debug("Size of RightHand is:\t{0}", comp.size());
		//for (auto i = 0; i < size; ++i)
		//{
		//	EXPECT_NEAR(0, norm(ref(i) - comp(i)) / norm(ref(i)), 0.005) << "Error in Excitation term\t" << i;
		//}


		//
		for (int zmcy = 0; zmcy < SystemConfig.ImpConfig.ArrayNumY; ++zmcy)
		{
			for (int zmcx = 0; zmcx < SystemConfig.ImpConfig.ArrayNumX; ++zmcx)
			{
				Vector3d bias{ zmcx*SystemConfig.ImpConfig.ArrayIntervalX,
					zmcy*SystemConfig.ImpConfig.ArrayIntervalY,0 };
				dcomplex patch = exp(-1i*k*bias.dot(SystemConfig.SourceConfig->Ki));

				for (auto i = 0; i < size; ++i)
				{
					dcomplex arrayRef = ref(i)*patch;
					int loc = SystemConfig.ImpConfig.ImpSize*(zmcx + zmcy * 4) + i;
					/*Console->debug("Ref=({0},{1})\tComp=({2},{3})",
						arrayRef.real(),arrayRef.imag(),comp(loc).real(),comp(loc).imag());*/
					EXPECT_LT(norm(arrayRef - comp(loc)) / norm(arrayRef), 0.001);
				}
			}
		}
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		FAIL();
	}
	
}
#endif