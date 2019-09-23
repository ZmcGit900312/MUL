#include "stdafx.h"

#ifdef GTEST
#include "gtest/gtest.h"
#include "AIMArray.h"
#include "CoreAPI.h"
#include "Const.h"
#include "Data.h"

using namespace Core;
using namespace Eigen;
using namespace AIMAssist;

class AIMArrayTest :public testing::Test
{
protected:
	RWGImpOperator _compute;
	AIMArrayTest() :_compute(k, W4, W7, eta) {}
	~AIMArrayTest() {}

	static void SetUpTestCase()
	{
		try
		{
			SystemConfig.ImpConfig.impType = Core::Array;
			SystemConfig.ImpConfig.numArrayX = 4;
			SystemConfig.ImpConfig.numArrayY = 4;
			SystemConfig.ImpConfig.distanceBiasX = 1.5;
			SystemConfig.ImpConfig.distanceBiasY = 1.5;
			SystemConfig.IEConfig.type = EFIE;
			SystemConfig.SolverConfig.Precond = Solution::ILU;
			if (Mesh::GetInstance()->IsLock())
			{
				ASSERT_EQ(0, Core::CreatMesh()) << "Error in Creat Mesh";
				ASSERT_EQ(0, Core::CreatBasicFunction(false)) << "Error in Load BasicFunction";
			}
			if (ComponentList::BFvector.size() < 1)ASSERT_EQ(0, Core::CreatBasicFunction(false)) << "Error in Load BasicFunction";
			if (!Core::IGreen::GetInstance())EXPECT_EQ(0, Core::SetGreenFunction());
			ASSERT_EQ(0, Core::PreCalculateSelfTriangleImpedance()) << "Error in Pre-compute the SelfTriangle Impedance";
			ASSERT_EQ(0, Core::CreatImpedance()) << "Error in Initial the Impedance class";

			cout << "\n";

			//Random initial
			srand(static_cast<unsigned>(time(nullptr)));


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
		if (ComponentList::ImpService)
		{
			delete ComponentList::ImpService;
			ComponentList::ImpService = nullptr;
			Console->debug("Release Matrix");
		}
		if (Core::Solver)
		{
			delete Core::Solver;
			Core::Solver = nullptr;
		}
	}

	dcomplex GetImpedance(size_t row, size_t col)
	{
		ImpAIM* imp = static_cast<ImpAIM*>(ComponentList::ImpService);

		const size_t unknowns = SystemConfig.ImpConfig.ImpSize;
		const double threshold = SystemConfig.ImpConfig.Threshold*Lambda;


		VectorXcd vrowx = imp->GetGammaX().col(row);
		VectorXcd vrowy = imp->GetGammaY().col(row);
		VectorXcd vrowz = imp->GetGammaZ().col(row);
		VectorXcd vrowd = imp->GetGammaD().col(row);
		VectorXcd vcolx = imp->GetGammaX().col(col);
		VectorXcd vcoly = imp->GetGammaY().col(col);
		VectorXcd vcolz = imp->GetGammaZ().col(col);
		VectorXcd vcold = imp->GetGammaD().col(col);

		_mvptool.MVP(imp->CGetGreen(), vcolx);
		_mvptool.MVP(imp->CGetGreen(), vcoly);
		_mvptool.MVP(imp->CGetGreen(), vcolz);
		_mvptool.MVP(imp->CGetGreen(), vcold);

		dcomplex tempx = vrowx.dot(vcolx);
		dcomplex tempy = vrowy.dot(vcoly);
		dcomplex tempz = vrowz.dot(vcolz);
		dcomplex tempd = vrowd.dot(vcold);
		return (tempx + tempy + tempz - tempd / (k*k)) * 1i*k*eta;
	}
	MulFFTMultiplicator _mvptool;
};


TEST_F(AIMArrayTest, MultipoleExpansion)
{
	AIMArray* fillingTool = new AIMArray(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
	auto& bf = ComponentList::BFvector;
	Console->debug("Allocate the MatrixSetting oject");

	fillingTool->MultipoleExpansion(bf);

	delete fillingTool;

	
}


TEST_F(AIMArrayTest, GreenBase)
{
	AIMArray* fillingTool = new AIMArray(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
	auto& bf = ComponentList::BFvector;

	char* path = "E:/ZMC/Code/C_program/MUL/SourceData/ArrayGreen.dat";

	ifstream ifs(path, ios::in | ios::binary);
	
	VectorXcd Green;
	bool readflag = true;
	if(ifs.is_open()&&readflag)
	{
		Console->debug("Load Green");
		int length = 0;
		ifs.read(reinterpret_cast<char*>(&length), sizeof(int));
		Console->debug("Size of Green is {}",length);
		Green.resize(length);
		for (int i = 0; i < length; ++i)
		{
			dcomplex val;
			ifs.read(reinterpret_cast<char*>(&val), sizeof(dcomplex));
			Green(i) = val;
		}
		ifs.clear();
		ifs.close();
	}
	else 
	{
		ofstream ofs(path, ios::out | ios::binary);
		if (ofs.is_open())
		{
			Console->debug("ConstructGreen");
			const clock_t starttime = clock();
			fillingTool->_green = IGreen::GetInstance();
			VectorXi position{ VectorXi::Zero(5) };//x-y-z-X-Y
			Green = fillingTool->ConstructIterated(position, 4);
			const clock_t endtime = clock();
			double timecost = double(endtime - starttime) / CLOCKS_PER_SEC;
			Console->info("ConstructGreen need:\t{}s", timecost);

			int length = Green.size();
			ofs.write(reinterpret_cast<char*>(&length), sizeof(int));
			for (int i = 0; i < length; ++i)
			{
				dcomplex val = Green[i];
				ofs.write(reinterpret_cast<char*>(&val), sizeof(dcomplex));
			}
			ofs.flush();
			ofs.close();
		}
		else FAIL();
		
	}
	
	//Initial Test Parameters
	
	VectorXi weight{ VectorXi::Zero(5) }, greenWeight{ weight }, greenWeightAcu{ weight };
	weight << SystemConfig.ImpConfig.xNumber, 
	SystemConfig.ImpConfig.yNumber, 
	SystemConfig.ImpConfig.zNumber,
	SystemConfig.ImpConfig.numArrayX,
	SystemConfig.ImpConfig.numArrayX;
	greenWeight = 2 * weight.array() - 1;
	greenWeightAcu(0) = 1;
	for (int i = 1 ;i < greenWeightAcu.size();i++)
	{
		greenWeightAcu(i) = greenWeight.head(i).prod();
	}

	//Test the Upper Triangle value 
	MatrixXi test{ MatrixXi::Zero(7,5) };
	test.row(0) << 7, 13, 4, 2, 0;
	test.row(1) << 14, 7, 24, 1, 2;
	test.row(2) << 9, 11, 1, 1, 1;
	test.row(3) << 11, 6, 7, 3, 2;
	test.row(4) << 5, 2, 13, 1, 0;
	test.row(5) << 15, 0, 19, 3, 2;
	test.row(6) << 17, 23, 23, 3, 1;
	for (int caseid = 0; caseid < 7; ++caseid)
	{
		Console->info("#Case {5}:\tTest=[{0},{1},{2},{3},{4}]",
			test(caseid,0),test(caseid,1),test(caseid, 2),test(caseid,3),test(caseid,4), caseid + 1);

		int testid = test.row(caseid).dot(greenWeightAcu);
		dcomplex greenRef = Green(testid);
		Vector3d Rb{ SystemConfig.ImpConfig.distanceBiasX * test(caseid,3) ,
			SystemConfig.ImpConfig.distanceBiasY * test(caseid,4) ,0 };
		Vector3d Ru{ SystemConfig.ImpConfig.Interval * test(caseid,0) ,
			SystemConfig.ImpConfig.Interval * test(caseid,1) ,
			SystemConfig.ImpConfig.Interval * test(caseid, 2) };
		dcomplex ref = IGreen::GetInstance()->Scalar(Rb + Ru, Vector3d::Zero());
		Console->debug("ID={0:5d}\tValue=({1:12.7f},{2:12.7f}i)",
			testid, greenRef.real(), greenRef.imag());
		EXPECT_NEAR(greenRef.real(), ref.real(), 1.0e-6);
		EXPECT_NEAR(greenRef.imag(), ref.imag(), 1.0e-6);

	}
	//Test the Lower Triangle value 


	delete fillingTool;
}



TEST_F(AIMArrayTest, ApproximationFarField)
{
	//AIMArray* fillingTool = new AIMArray(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
	//auto& bf = ComponentList::BFvector;
	//IGreen* green = IGreen::GetInstance();

	//Console->debug("Allocate the MatrixSetting oject");
	//fillingTool->MultipoleExpansion(bf);
	//fillingTool->GreenMatrixSet(green);
	//VectorXcd gb = fillingTool->GetGreenBase();

	////TEST FarField

	//const dcomplex refZ1 = { -0.0080060247635108418 ,-0.0019764567076286750 };//Z(18��123)
	//const dcomplex compZ1 = fillingTool->GetImpAIM(18, 123);
	//EXPECT_NEAR(refZ1.real(), compZ1.real(), 1.0e-6) << "Error in (18, 123) real";
	//EXPECT_NEAR(refZ1.imag(), compZ1.imag(), 1.0e-6) << "Error in (18, 123) imag";

	//const dcomplex refZ2 = { -0.0027595945849962643,0.0059947329121803053 };//Z(647, 22)
	//const dcomplex compZ2 = fillingTool->GetImpAIM(647, 22);
	//EXPECT_NEAR(refZ2.real(), compZ2.real(), 1.0e-6) << "Error in (647, 22) real";
	//EXPECT_NEAR(refZ2.imag(), compZ2.imag(), 1.0e-6) << "Error in (647, 22) imag";

	//const dcomplex refZ3 = { 0.0034725828079872414, 0.0047437269001039282 };//Z(38, 123)
	//const dcomplex compZ3 = fillingTool->GetImpAIM(38, 123);
	//EXPECT_NEAR(refZ3.real(), compZ3.real(), 1.0e-6) << "Error in (38, 123) real";
	//EXPECT_NEAR(refZ3.imag(), compZ3.imag(), 1.0e-6) << "Error in (38, 123) imag";

	//const dcomplex refZ4 = { -0.0067436242889280231 , 0.0033305745353076361 };//Z(17,653)
	//const dcomplex compZ4 = fillingTool->GetImpAIM(17, 653);
	//EXPECT_NEAR(refZ4.real(), compZ4.real(), 1.0e-6) << "Error in (17,653) real";
	//EXPECT_NEAR(refZ4.imag(), compZ4.imag(), 1.0e-6) << "Error in (17,653) imag";

	//const dcomplex refZ9 = { -0.0034002845471970793 , 0.0033173618753603450 };//Z(18,654)
	//const dcomplex compZ9 = fillingTool->GetImpAIM(18, 654);
	//EXPECT_NEAR(refZ9.real(), compZ9.real(), 1.0e-6) << "Error in (18,654) real";
	//EXPECT_NEAR(refZ9.imag(), compZ9.imag(), 1.0e-6) << "Error in (18,654) imag";

	//delete fillingTool;
}

TEST_F(AIMArrayTest, Multiplication)
{
	//AIMArray* fillingTool = new AIMArray(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
	//auto& bf = ComponentList::BFvector;
	//Console->debug("Allocate the MatrixSetting oject");

	//fillingTool->MultipoleExpansion(bf);
	//fillingTool->GreenMatrixSet(IGreen::GetInstance());

	////Initial multiplicator
	//MKL_LONG dim = 3, layer[3] = { 2 * SystemConfig.ImpConfig.zNumber - 2,
	//	2 * SystemConfig.ImpConfig.yNumber - 2,
	//	2 * SystemConfig.ImpConfig.xNumber - 2 };
	//_mvptool.Reset(dim, layer);


	//const dcomplex refZ1 = { -0.0080060247635108418 ,-0.0019764567076286750 };//Z(18��123)
	//const dcomplex compZ1 = GetImpedance(18, 123);
	//EXPECT_NEAR(refZ1.real(), compZ1.real(), 1.0e-6) << "Error in (18, 123) real";
	//EXPECT_NEAR(refZ1.imag(), compZ1.imag(), 1.0e-6) << "Error in (18, 123) imag";

	//const dcomplex refZ2 = { -0.0027595945849962643,0.0059947329121803053 };//Z(647, 22)
	//const dcomplex compZ2 = GetImpedance(647, 22);
	//EXPECT_NEAR(refZ2.real(), compZ2.real(), 1.0e-6) << "Error in (647, 22) real";
	//EXPECT_NEAR(refZ2.imag(), compZ2.imag(), 1.0e-6) << "Error in (647, 22) imag";

	//const dcomplex refZ3 = { 0.0034725828079872414, 0.0047437269001039282 };//Z(38, 123)
	//const dcomplex compZ3 = GetImpedance(38, 123);
	//EXPECT_NEAR(refZ3.real(), compZ3.real(), 1.0e-6) << "Error in (38, 123) real";
	//EXPECT_NEAR(refZ3.imag(), compZ3.imag(), 1.0e-6) << "Error in (38, 123) imag";

	//const dcomplex refZ4 = { -0.0067436242889280231 , 0.0033305745353076361 };//Z(17,653)
	//const dcomplex compZ4 = GetImpedance(17, 653);
	//EXPECT_NEAR(refZ4.real(), compZ4.real(), 1.0e-6) << "Error in (17,653) real";
	//EXPECT_NEAR(refZ4.imag(), compZ4.imag(), 1.0e-6) << "Error in (17,653) imag";

	//const dcomplex refZ9 = { -0.0034002845471970793 , 0.0033173618753603450 };//Z(18,654)
	//const dcomplex compZ9 = GetImpedance(18, 654);
	//EXPECT_NEAR(refZ9.real(), compZ9.real(), 1.0e-6) << "Error in (18,654) real";
	//EXPECT_NEAR(refZ9.imag(), compZ9.imag(), 1.0e-6) << "Error in (18,654) imag";
}

TEST_F(AIMArrayTest, NearFieldFilling)
{
	try
	{
		AIMArray* fillingTool = new AIMArray(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
		auto& bf = ComponentList::BFvector;

		fillingTool->TriangleFillingStrategy(*Mesh::GetInstance(), bf);

		delete fillingTool;
	}
	catch (spd::spdlog_ex&ex)
	{
		Console->warn(ex.what());
		RuntimeLog->warn(ex.what());
		RuntimeLog->flush();
	}
}
TEST_F(AIMArrayTest, Solving)
{
	//try
	//{
	//	//throw spd::spdlog_ex("AIMCalculate is not Testing");

	//	AIMArray* fillingTool = new AIMArray(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
	//	auto& bf = ComponentList::BFvector;
	//	Console->debug("Allocate the MatrixSetting oject");

	//	fillingTool->MultipoleExpansion(bf);
	//	fillingTool->GreenMatrixSet(IGreen::GetInstance());
	//	fillingTool->TriangleFillingStrategy(*Mesh::GetInstance(), ComponentList::BFvector);

	//	ASSERT_EQ(0, Core::SetRightHand()) << "Error in Set RightHand";
	//	auto info = Core::Solve();
	//	EXPECT_EQ(0, info) << "Error in Solve Matrix with BicgStab";
	//	if (info == 0)
	//	{
	//		EXPECT_EQ(0, Core::SaveBasicFunction(SystemConfig.BasicFunctionFileName.c_str())) << "Error in save BasicFunction";
	//		EXPECT_EQ(0, Core::CalculateRequest()) << "Error in Calculate the FarField";
	//	}
	//}
	//catch (spd::spdlog_ex&ex)
	//{
	//	Console->warn(ex.what());
	//	RuntimeLog->warn(ex.what());
	//	RuntimeLog->flush();
	//}
}
#endif