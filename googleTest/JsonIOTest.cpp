#include "stdafx.h"
#ifdef GTEST
#include "gtest/gtest.h"
#include "CoreAPI.h"
#include "Data.h"
#include "rapidjson/document.h"
#include <rapidjson/istreamwrapper.h>
#include <iostream>
#include "rapidjson/pointer.h"


using namespace std;
namespace rj = rapidjson;

using namespace Eigen;
using namespace std;

class JsonIO :public testing::Test
{
protected:

	//TestSuite
	static void SetUpTestCase()
	{
		doc = ReadJson("E:\\ZMC\\Code\\C_program\\MUL\\SourceData\\test.json");
	}

	static void TearDownTestCase()
	{
	}

	//Releae BasicFunction every case if exist
	void TearDown()override
	{

	}


	static rj::Document ReadJson(string filename)
	{
		

		// read a JSON file
		
		std::ifstream ifs(filename);
		
		if (ifs.is_open())
		{
			rj::Document d;
			rj::IStreamWrapper isw(ifs);
			d.ParseStream(isw);

			ifs.clear();
			ifs.close();

			return d;
		}
		else throw exception("Read Error");		

	}

	//static bool SaveJson(json j,string filename)
	//{

	//	// read a JSON file
	//	std::ofstream ofs(filename);
	//	if (ofs.is_open())ofs << std::setw(4) << j << std::endl;
	//	else throw exception("Save Error");

	//	ofs.clear();
	//	ofs.close();

	//	return true;
	//}

	string sourceFilePath = "E:\\ZMC\\Code\\C_program\\MUL\\SourceData";

	string sourceName = "test.json";
	string destName = "new.json";
	
	static rj::Document doc;
};


rj::Document JsonIO::doc;

TEST_F(JsonIO, IOTest)
{
	string readFileName = sourceFilePath + "\\" + sourceName;
	string saveFileName = sourceFilePath + "\\" + destName;


	rj::Document jd= ReadJson(readFileName);	

	EXPECT_TRUE(jd["version"].IsString());
	EXPECT_TRUE(jd["date"].IsString());
	
	//cout<< std::setw(4) << j << std::endl;

	cout << "version\t" << jd["version"].GetString() << '\n' << "date\t" << jd["date"].GetString() << endl;
	
	//! Type of JSON value
	//enum Type {
	//	kNullType = 0,      //!< null
	//	kFalseType = 1,     //!< false
	//	kTrueType = 2,      //!< true
	//	kObjectType = 3,    //!< object
	//	kArrayType = 4,     //!< array 
	//	kStringType = 5,    //!< string
	//	kNumberType = 6     //!< number
	//};
}


TEST_F(JsonIO,File)
{
	try
	{
		
		//Project
		if(rj::Value* jpro=rj::GetValueByPointer(doc,"/File/Project"))
		{
			auto val = jpro->HasMember("Name") ? jpro->FindMember("Name") : throw exception();
			string name=val->value.GetString();

			val= jpro->HasMember("Directory") ? jpro->FindMember("Directory") : throw exception();
			string directory = val->value.GetString();

			val= jpro->HasMember("Report") ? jpro->FindMember("Report") : throw exception();
			string report = val->value.GetString();


			EXPECT_EQ("test", name);
			EXPECT_EQ("E:\\ZMC\\Code\\C_program\\MUL\\SourceData", directory);
			EXPECT_EQ("E:\\ZMC\\Code\\C_program\\MUL\\SourceData\\test.out", report);
		}
		else throw exception();

		//Mesh
		if(rj::Value* jmesh=rj::GetValueByPointer(doc,"/File/Mesh"))
		{

			auto val = jmesh->HasMember("Category") ? jmesh->FindMember("Category") : throw exception();
			int category  = val->value.GetInt64();

			val = jmesh->HasMember("FilePath") ? jmesh->FindMember("FilePath") : throw exception();
			string filepath = val->value.GetString();

			EXPECT_EQ(3, category);
			EXPECT_EQ("E:\\ZMC\\Code\\C_program\\MUL\\SourceData\\MeshTest.nas", filepath);
		}
		else throw exception();

		//OS
		if(rj::Value* jos=rj::GetValueByPointer(doc,"/File/OS"))
		{
			auto val = jos->HasMember("Category") ? jos->FindMember("Category") : throw exception();
			int category = val->value.GetInt64();

			val = jos->HasMember("BasisFunctionFilePath") ? jos->FindMember("BasisFunctionFilePath") : throw exception("BasisFunction is error");
			string bfp = val->value.GetString();

			val = jos->HasMember("CurrentFilePath") ? jos->FindMember("CurrentFilePath") : throw exception();
			string cp = val->value.GetString();

			EXPECT_EQ(0, category);
			EXPECT_EQ("E:\\ZMC\\Code\\C_program\\MUL\\SourceData\\test.bf", bfp);
			EXPECT_EQ("E:\\ZMC\\Code\\C_program\\MUL\\SourceData\\test.cu", cp);
		}
		else
		{
			cout << "OS is not found" << endl;
		}
	}
	catch (exception& ex)
	{
		cout << ex.what() << endl;
		FAIL();
	}
}

TEST_F(JsonIO, Impedance)
{
	try
	{
		enum IMP { MoM, AIM, Array };
		IMP category = MoM;

		bool tfs = false, vgt = false;

		int grid = -2, dim = -2, ax = -3, ay = -3;
		double interval = -1, thr = -1, ntt = -1, aix = -1, aiy = -1;

		if(rj::Value* jimp=rj::GetValueByPointer(doc,"/Impedance"))
		{
			auto val = jimp->HasMember("Category") ? jimp->FindMember("Category") : throw exception("Cat");
			category = IMP(val->value.GetInt64());

			val = jimp->HasMember("TriangleFillingStrategy") ? jimp->FindMember("TriangleFillingStrategy") : throw;
			tfs = val->value.GetBool();

			val = jimp->HasMember("VirtualGridTechnique") ? jimp->FindMember("VirtualGridTechnique") : throw;
			vgt = val->value.GetBool();

			
			val = jimp->HasMember("Dimension") ? jimp->FindMember("Dimension") : throw;
			dim = val->value.GetInt64();

			val = jimp->HasMember("GridOrder") ? jimp->FindMember("GridOrder") : throw;
			grid = val->value.GetInt64();

			val = jimp->HasMember("ArrayNumX") ? jimp->FindMember("ArrayNumX") : throw;
			ax = val->value.GetInt64();

			val = jimp->HasMember("ArrayNumY") ? jimp->FindMember("ArrayNumY") : throw;
			ay = val->value.GetInt64();

			val = jimp->HasMember("Interval") ? jimp->FindMember("Interval") : throw;
			interval = val->value.GetDouble();

			val = jimp->HasMember("Threshold") ? jimp->FindMember("Threshold") : throw;
			thr = val->value.GetDouble();

			val = jimp->HasMember("NearThresholdTolerance") ? jimp->FindMember("NearThresholdTolerance") : throw;
			ntt = val->value.GetDouble();

			val = jimp->HasMember("ArrayIntervalX") ? jimp->FindMember("ArrayIntervalX") : throw;
			aix = val->value.GetDouble();

			val = jimp->HasMember("ArrayIntervalY") ? jimp->FindMember("ArrayIntervalY") : throw;
			aiy = val->value.GetDouble();

		}

		EXPECT_EQ(Array, category);
		EXPECT_EQ(true, tfs);
		EXPECT_EQ(true, vgt);
		EXPECT_EQ(3, dim);
		EXPECT_EQ(2, grid);
		EXPECT_EQ(4, ax);
		EXPECT_EQ(4, ay);
		EXPECT_NEAR(0.05, interval, 1.0e-5);
		EXPECT_NEAR(0.3, thr, 1.0e-5);
		EXPECT_NEAR(0.01, ntt, 1.0e-5);
		EXPECT_NEAR(1.5, aix, 1.0e-5);
		EXPECT_NEAR(1.5, aiy, 1.0e-5);
	}
	catch (exception& message)
	{
		cout << message.what() << endl;
		FAIL();
	}
	
}

TEST_F(JsonIO, IE)
{
	try
	{
		enum IE { EFIE, MFIE, CFIE };
		IE category = CFIE;

		double alpha = -1, eta = -1, zs = -1;

		// 使用 Get() 访问 DOM。若该值不存在则返回 nullptr。
		if (rj::Value* jie = GetValueByPointer(doc, "/IE"))
		{

			if (jie->HasMember("Category"))
			{
				category = IE(jie->FindMember("Category")->value.GetUint64());
			}

			if (jie->HasMember("Alpha"))
			{
				alpha = jie->FindMember("Alpha")->value.GetDouble();
			}

			if (jie->HasMember("Eta"))
			{
				eta = jie->FindMember("Eta")->value.GetDouble();
			}

			if (jie->HasMember("Zs"))
			{
				zs = jie->FindMember("Zs")->value.GetDouble();
			}

			EXPECT_EQ(EFIE, category);
			EXPECT_NEAR(0.3, alpha, 1.0e-5);
			EXPECT_NEAR(376.99112, eta, 1.0e-5);
			EXPECT_NEAR(376.99112, zs, 1.0e-5);
		}
	}
	catch(void*)
	{
		FAIL();
	}
	
}

TEST_F(JsonIO, Solver)
{
	try
	{
		enum SolutionType { CG, LU };
		SolutionType st = LU;

		enum PreCondition { Identity, Jacobi, ILU };
		PreCondition pd = ILU;

		int itr = -1;
		double stop = -1, res = -1;

		if(rj::Value* jsolver=rj::GetValueByPointer(doc,"/Solver"))
		{
			auto val = jsolver->HasMember("SolutionType")?jsolver->FindMember("SolutionType"):throw;
			st= SolutionType(val->value.GetInt64());

			val = jsolver->HasMember("Preconditioning") ? jsolver->FindMember("Preconditioning") : throw;
			pd = PreCondition(val->value.GetInt64());

			val = jsolver->HasMember("StopTolerance") ? jsolver->FindMember("StopTolerance") : throw;
			stop = val->value.GetDouble();

			val = jsolver->HasMember("Residum") ? jsolver->FindMember("Residum") : throw;
			res = val->value.GetDouble();

			val = jsolver->HasMember("Iterations") ? jsolver->FindMember("Iterations") : throw;
			itr = val->value.GetInt64();
		}


		EXPECT_EQ(CG, st);
		EXPECT_EQ(Identity, pd);
		EXPECT_NEAR(10.0, stop, 1.0e-5);
		EXPECT_NEAR(1.0e-4, res, 1.0e-9);
		EXPECT_EQ(0, itr);
	}
	catch (...)
	{
		FAIL();
	}
	
}

TEST_F(JsonIO, Request)
{
	try
	{
		rj::Value& jrequest = doc["Request"];

		for(rj::Value::ValueIterator vp=jrequest.Begin();vp!=jrequest.End();++vp)
		{
			rj::Value& val = *vp;

			string name = val["Name"].GetString();
			cout << "Name:\t" << name << endl;

			int numt=-1, nump = -1;
			double sth = -1, ith = -1, sph = -1, iph = -1;

			//Theta
			rj::Value& theta = val["Theta"];
			if (theta.IsArray())
			{
				numt = theta[0].GetInt64();
				sth = theta[1].GetDouble(), ith = theta[2].GetDouble();
			}

			//Phi
			rj::Value& phi = val["Phi"];
			if (phi.IsArray())
			{

				nump = phi[0].GetInt64();
				sph = phi[1].GetDouble(), iph = phi[2].GetDouble();
			}

			cout << std::left << "Theta:\tNum=" << numt << "\tStart=" << sth << "\tIncrement=" << ith << endl;
			cout << std::left << "Phi:\tNum=" << nump << "\tStart=" << sph << "\tIncrement=" << iph << endl;
		}
	}
	catch(...)
	{
		
	}
}

TEST_F(JsonIO, Sweep)
{

	//ConfiguraitonSweep
	class config
	{
	public:

		config(string tag, double fre, int num) :Tag(tag), Frequency(fre), NumberOfElement(num)
		{
			ArrayLocation.clear();
			ArrayLocation.reserve(num);
		}
		string Tag;
		double Frequency = -1.0;
		int NumberOfElement = 1;
		vector<pair<int, int>> ArrayLocation;
	};


	try
	{
		rj::Value& jsweep = doc["Sweep"];
		vector<config> sweep;

		for (rj::Value::ValueIterator sp = jsweep.Begin();sp!=jsweep.End();++sp)
		{
			rj::Value& rsp = *sp;

			config zmc_config(rsp["Tag"].GetString(),
				rsp["Frequency"].GetDouble(),
				rsp["Number"].GetInt64());
			for (rj::Value::ValueIterator ap =rsp["ArrayLocation"].Begin();
				ap!=rsp["ArrayLocation"].End();++ap)
			{			
				pair<int, int> alzmc = make_pair((*ap)[0].GetInt64(), (*ap)[1].GetInt64());
				zmc_config.ArrayLocation.push_back(alzmc);
			}
			sweep.push_back(zmc_config);
		}

		for (auto val : sweep)
		{
			cout << "Tag:\t" << val.Tag << endl;
			cout << "Frequency:\t" << val.Frequency << endl;
			cout << "Number:\t" << val.NumberOfElement << endl;
			cout << "Size of ArrayLocatioin:\t" << val.ArrayLocation.size() << endl;
		}
	}
	catch (...)
	{
	}	
}


TEST_F(JsonIO, Excitation)
{
	try
	{

		if(rj::Value* jsp=rj::GetValueByPointer(doc,"/Excitation"))
		{
			rj::Value& jsource = *jsp;

			Source::SourceType category = Source::SourceType(jsource["Category"].GetInt64());
			string name = jsource["Name"].GetString();
			Source::SourceType pol = Source::SourceType(jsource["Polarisation"].GetInt64());


			EXPECT_EQ("T0P270", name);
			EXPECT_EQ(Source::EXCITATION_LINEAR, category);
			EXPECT_EQ(Source::EXCITATION_CIRC_RIGHT, pol);
			cout << "Name:\t" << name << endl;
			cout << "Category:\tA0" << endl;
			cout << "Polarisation:\tLinear" << endl;

			//Theta
			rj::Value& theta = jsource["Theta"];
			if(theta.IsArray())
			{		
				int num = theta[0].GetInt64();
				double s = theta[1].GetDouble(), i = theta[2].GetDouble();
				EXPECT_EQ(1, num);
				EXPECT_NEAR(0.0, s, 1.0e-5);
				EXPECT_NEAR(1.0, i, 1.0e-5);
			}
			
			//Phi
			rj::Value& phi = jsource["Phi"];
			if (phi.IsArray())
			{
				
				int num = phi[0].GetInt64();
				double s = phi[1].GetDouble(), i = phi[2].GetDouble();
				EXPECT_EQ(1, num);
				EXPECT_NEAR(270.0, s, 1.0e-5);
				EXPECT_NEAR(1.0, i, 1.0e-5);
			}

			//Rotation
			rj::Value& jtemp = jsource["Rotation"];
			if (jtemp.IsArray())
			{			
				double x = jtemp[0].GetDouble(), y = jtemp[1].GetDouble(), z = jtemp[2].GetDouble();
				EXPECT_NEAR(0.0, x, 1.0e-5);
				EXPECT_NEAR(0.0, y, 1.0e-5);
				EXPECT_NEAR(0.0, z, 1.0e-5);
			}

			rj::Value& jfield = jsource["EField"];
			if(jfield.IsObject())
			{
				
				double eta = jfield["Eta"].GetDouble(),
					mag = jfield["Magnitude"].GetDouble(),
					phase = jfield["Phase"].GetDouble(),
					ell = jfield["Ellipticity"].GetDouble();
				EXPECT_NEAR(0.0, eta, 1.0e-5);
				EXPECT_NEAR(1.0, mag, 1.0e-5);
				EXPECT_NEAR(0.0, phase, 1.0e-5);
				EXPECT_NEAR(0.0, ell, 1.0e-5);
			}
		}
		else throw exception();

	}
	catch (exception&ex)
	{
		cout << ex.what() << endl;
		FAIL();
	}
}


TEST_F(JsonIO,ParseConfig)
{
	char* filename="E:\\ZMC\\Code\\C_program\\MUL\\SourceData\\test.json";
	//Result Report Is Not Initialization
	//EXPECT_EQ(0, ParseConfiguratoinFile(filename));
}
#endif