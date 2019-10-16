#include "stdafx.h"

#ifdef GTEST
#include "gtest/gtest.h"
#include "AIMArray.h"
#include "CoreAPI.h"
#include "Const.h"
#include "Data.h"
#include "Current.h"

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
			SystemConfig.ImpConfig.ImpType = Core::Array;
			SystemConfig.ImpConfig.ArrayNumX = 2;
			SystemConfig.ImpConfig.ArrayNumY = 1;
			SystemConfig.ImpConfig.ArrayIntervalX = 1.5;
			SystemConfig.ImpConfig.ArrayIntervalY = 1.5;
			SystemConfig.IEConfig.type = EFIE;
			SystemConfig.SolverConfig.Precond = Solution::Identity;
			ASSERT_EQ(0, Core::DataInitialization()) << "Error in Initialization";
			srand(static_cast<unsigned>(time(nullptr)));

		}
		catch (spd::spdlog_ex&ex)
		{
			Assist::LogException(ex);
			FAIL();
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
		if (Core::equation)
		{
			delete equation;
			equation = nullptr;
			Console->debug("Release IE");
		}
	}

	static void InitialImpConfig()
	{
		//Initial Current
		auto curInfo = Solution::CurrentInfo::GetInstance();
		curInfo->Reformat(SystemConfig.ImpConfig.ImpType);
		curInfo->Current.push_back(new Solution::ArrayCurrent(ComponentList::BFvector.size(), 3.0e8, "AIMArrayTest", 16, SystemConfig.ImpConfig.ArrayIntervalX, SystemConfig.ImpConfig.ArrayIntervalY));
		Solution::ArrayCurrent* ac = static_cast<Solution::ArrayCurrent*>(curInfo->Current.back());
		ac->EMCParameterUpdate();
		SystemConfig.ImpConfig.ArrayNumX = 4;
		SystemConfig.ImpConfig.ArrayNumY = 4;
		for (int zmcy = 0;zmcy < SystemConfig.ImpConfig.ArrayNumY;zmcy++)
		{
			for (int zmcx = 0; zmcx < SystemConfig.ImpConfig.ArrayNumX; ++zmcx)
			{
				ac->_arrayLocation.push_back({ zmcx,zmcy });
			}
		}

		curInfo->_numberOfConfig = curInfo->Current.size();
		//Initial IE Impedance and solver
		equation = IE::FIE(SystemConfig.IEConfig.type);

		ASSERT_EQ(0, Core::PreCalculateSelfTriangleImpedance()) << "Error in Pre-compute the SelfTriangle Impedance";
		ASSERT_EQ(0, Core::InitialSolverAndImpedance()) << "Error in Initial the Impedance class";
	}

	dcomplex GetImpedance(size_t source, size_t field)
	{
		ImpArrayAIM* imp = static_cast<ImpArrayAIM*>(ComponentList::ImpService);

		const size_t unknowns = SystemConfig.ImpConfig.ImpSize;
		const double threshold = SystemConfig.ImpConfig.Threshold*Lambda;


		VectorXcd vrowx = imp->GetGammaX().col(field);
		VectorXcd vrowy = imp->GetGammaY().col(field);
		VectorXcd vrowz = imp->GetGammaZ().col(field);
		VectorXcd vrowd = imp->GetGammaD().col(field);
		VectorXcd vcolx = imp->GetGammaX().col(source);
		VectorXcd vcoly = imp->GetGammaY().col(source);
		VectorXcd vcolz = imp->GetGammaZ().col(source);
		VectorXcd vcold = imp->GetGammaD().col(source);

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

	VectorXcd GetGreenBase(char* path, AIMArray* tool,bool readflag=true) const
	{
		VectorXcd Green;
		ifstream ifs(path, ios::in | ios::binary);
		if (ifs.is_open() && readflag)
		{
			Console->debug("Load Green");
			const clock_t starttime = clock();
			int length = 0;
			ifs.read(reinterpret_cast<char*>(&length), sizeof(int));
			Console->debug("Size of Green is {}", length);
			Green.resize(length);
			for (int i = 0; i < length; ++i)
			{
				dcomplex val;
				ifs.read(reinterpret_cast<char*>(&val), sizeof(dcomplex));
				Green(i) = val;
			}
			ifs.clear();
			ifs.close();

			const clock_t endtime = clock();
			double timecost = double(endtime - starttime) / CLOCKS_PER_SEC;
			Console->info("Read Green need:\t{}s", timecost);
		}
		else
		{
			ofstream ofs(path, ios::out | ios::binary);
			if (ofs.is_open())
			{
				Console->debug("ConstructGreen");
				const clock_t starttime = clock();

				VectorXi position{ VectorXi::Zero(5) };//x-y-z-X-Y
				Green = tool->ConstructIterated(position, 4);
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
			
		}
		return Green;
	}

	MulFFTMultiplicator _mvptool;
};


TEST_F(AIMArrayTest, MultipoleExpansion)
{
	InitialImpConfig();
	
	AIMArray* fillingTool = new AIMArray(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
	auto& bf = ComponentList::BFvector;
	Console->debug("Allocate the MatrixSetting oject");

	fillingTool->MultipoleExpansion(bf);

	delete fillingTool;

	
}


TEST_F(AIMArrayTest, GenerateGreen)
{	

	InitialImpConfig();

	//Initial AIMArray
	AIMArray* fillingTool = new AIMArray(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
	auto& bf = ComponentList::BFvector;

	char* path = "E:/ZMC/Code/C_program/MUL/SourceData/ArrayGreen.dat";
	fillingTool->_green = IGreen::GetInstance();

	bool readflag = true;

	//Get Green UnFFT
	VectorXcd Green = GetGreenBase(path,fillingTool,readflag);
	
	//Initial Test Parameters
	
	VectorXi weight{ VectorXi::Zero(5) }, greenWeightAcu{ weight };
	weight << SystemConfig.ImpConfig.xNumber, 
	SystemConfig.ImpConfig.yNumber, 
	SystemConfig.ImpConfig.zNumber,
	SystemConfig.ImpConfig.ArrayNumX,
	SystemConfig.ImpConfig.ArrayNumY;
	VectorXi greenWeight = 2 * weight.array() - 1;
	greenWeightAcu(0) = 1;
	for (int i = 1 ;i < greenWeightAcu.size();i++)
	{
		greenWeightAcu(i) = greenWeight.head(i).prod();
	}

	//Test the Upper Triangle value 
	MatrixXi test{ MatrixXi::Zero(7,5) };
	test.row(0) << 13, 2, 6, 3, 1;
	test.row(1) << 2, 2, 6, 0, 1;
	test.row(2) << 9, 11, 1, 1, 1;
	test.row(3) << 11, 6, 7, 3, 2;
	test.row(4) << 5, 2, 13, 1, 0;
	test.row(5) << 15, 0, 19, 3, 2;
	test.row(6) << 17, 23, 23, 3, 1;
	Console->info("Test upper triangle green");
	for (int caseid = 0; caseid < 7; ++caseid)
	{
		Console->info("#Case {5}:\tTest=[{0},{1},{2},{3},{4}]",
			test(caseid,0),test(caseid,1),test(caseid, 2),test(caseid,3),test(caseid,4), caseid + 1);

		int testid = test.row(caseid).dot(greenWeightAcu);
		dcomplex greenTest = Green(testid);
		Vector3d Rb{ SystemConfig.ImpConfig.ArrayIntervalX * test(caseid,3) ,
			SystemConfig.ImpConfig.ArrayIntervalY * test(caseid,4) ,0 };
		Vector3d Ru{ SystemConfig.ImpConfig.Interval * test(caseid,0) ,
			SystemConfig.ImpConfig.Interval * test(caseid,1) ,
			SystemConfig.ImpConfig.Interval * test(caseid, 2) };
		dcomplex ref = IGreen::GetInstance()->Scalar(Rb + Ru, Vector3d::Zero());
		Console->debug("ID={0:5d}\tValue=({1:12.7f},{2:12.7f}i)",
			testid, greenTest.real(), greenTest.imag());
		EXPECT_NEAR(greenTest.real(), ref.real(), 1.0e-6);
		EXPECT_NEAR(greenTest.imag(), ref.imag(), 1.0e-6);

	}

	Console->info("Test lower triangle green");
	test.row(0) << 2, -2, 6, 3, 1;
	test.row(1) << -2, 2, -6, -3, -1;
	test.row(2) << 9, -11, 1, -1, 1;
	test.row(3) << -11, 6, -7, 3, 2;
	test.row(4) << 5, -2, -13, 1, 0;
	test.row(5) << -15, 0, 19, 3, -2;
	test.row(6) << -17, -23, 23, 3, -1;
	for (int caseid = 0; caseid < 7; ++caseid)
	{
		Console->info("#Case {5}:\tTest=[{0},{1},{2},{3},{4}]",
			test(caseid, 0), test(caseid, 1), test(caseid, 2), test(caseid, 3), test(caseid, 4), caseid + 1);

		VectorXi lowerId = test.row(caseid);
		for (int zmc = 0; zmc < test.cols(); ++zmc)
		{
			if (test(caseid, zmc) < 0)lowerId(zmc) = greenWeight(zmc) +test(caseid, zmc);
		}
		int testid = lowerId.dot(greenWeightAcu);
		dcomplex greenRef = Green(testid);
		Vector3d Rb{ SystemConfig.ImpConfig.ArrayIntervalX * test(caseid,3) ,
			SystemConfig.ImpConfig.ArrayIntervalY * test(caseid,4) ,0 };
		Vector3d Ru{ SystemConfig.ImpConfig.Interval * test(caseid,0) ,
			SystemConfig.ImpConfig.Interval * test(caseid,1) ,
			SystemConfig.ImpConfig.Interval * test(caseid, 2) };
		dcomplex ref = IGreen::GetInstance()->Scalar(Rb + Ru, Vector3d::Zero());
		Console->debug("ID={0:5d}\tValue=({1:12.7f},{2:12.7f}i)",
			testid, greenRef.real(), greenRef.imag());
		EXPECT_NEAR(greenRef.real(), ref.real(), 1.0e-6);
		EXPECT_NEAR(greenRef.imag(), ref.imag(), 1.0e-6);

	}
	delete fillingTool;
}

TEST_F(AIMArrayTest, GreenFFT)
{
	//Initial Current
	InitialImpConfig();

#pragma region Initial Temporary Parameters
	VectorXi weight{ VectorXi::Zero(5) }, greenWeightAcu{ weight };
	weight << SystemConfig.ImpConfig.xNumber,
		SystemConfig.ImpConfig.yNumber,
		SystemConfig.ImpConfig.zNumber,
		SystemConfig.ImpConfig.ArrayNumX,
		SystemConfig.ImpConfig.ArrayNumY;
	VectorXi greenWeight = 2 * weight.array() - 1;
	greenWeightAcu(0) = 1;
	for (int i = 1;i < greenWeightAcu.size();i++)
	{
		greenWeightAcu(i) = greenWeight.head(i).prod();
	}
#pragma endregion 

#pragma region AIMArrayAPI
	AIMArray* fillingTool = new AIMArray(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
	auto& bf = ComponentList::BFvector;
	ImpArrayAIM* imp = static_cast<ImpArrayAIM*>(ComponentList::ImpService);

	//Multipole	
	//fillingTool->MultipoleExpansion(bf);

	//Get Green UnFFT
	char* path = "E:/ZMC/Code/C_program/MUL/SourceData/ArrayGreen.dat";
	fillingTool->_green = IGreen::GetInstance();

	bool readflag = true;
	VectorXcd GreenBase= GetGreenBase(path, fillingTool, readflag);
	imp->GetGreen() = GreenBase;

	//Initial multiplicator

	MKL_LONG layer[5] = { greenWeight[4] ,greenWeight[3], greenWeight[2] ,greenWeight[1], greenWeight[0] };
	_mvptool.Reset(5, layer);

	_mvptool.fwd(imp->GetGreen());//FFT
#pragma endregion 


	Matrix<int, 5, 1> source, field;
	source<< 20,8,5,0,0 ;
	field << 22, 6, 11, 3, 1;
	Matrix<int, 5, 1> dis = field - source;
	size_t sourceID = source.dot(greenWeightAcu), fieldID = field.dot(greenWeightAcu);
	size_t L = imp->GetGreen().size();

	VectorXcd sourceVec{ VectorXcd::Zero(L) }, fieldVec{ sourceVec };
	sourceVec(sourceID) = 1;
	fieldVec(fieldID) = 1;

	Vector3d Rb{ SystemConfig.ImpConfig.ArrayIntervalX * dis(3) ,
			SystemConfig.ImpConfig.ArrayIntervalY * dis(4) ,0 };
	Vector3d Ru{ SystemConfig.ImpConfig.Interval * dis(0) ,
		SystemConfig.ImpConfig.Interval * dis(1) ,
		SystemConfig.ImpConfig.Interval * dis(2) };
	dcomplex ref = IGreen::GetInstance()->Scalar(Rb + Ru, Vector3d::Zero());
	//Info 
	Console->info("Test FFT Multiplication by selecting Green:");
	Console->debug("Source({0},{1},{2},{3},{4})\tLocation={5}",
		source(0), source(1), source(2), source(3), source(4), sourceID);
	Console->debug("Field({0},{1},{2},{3},{4})\tLocation={5}",
		field(0), field(1), field(2), field(3), field(4), fieldID);
	Console->debug("Dis({0},{1},{2},{3},{4})",
		dis(0), dis(1), dis(2), dis(3), dis(4));
	

	for (int zmc = 0; zmc < dis.size(); ++zmc)
	{
		if (dis(zmc) < 0)dis(zmc) += greenWeight(zmc);
	}
	size_t greenID = dis.dot(greenWeightAcu);

	size_t greenID_ = dis.dot(greenWeightAcu);
	dcomplex GreenRef = GreenBase(greenID),GreenRef_= GreenBase(greenID_);

	//FFT and IFFT
	_mvptool.MVP(imp->CGetGreen(), sourceVec);
	dcomplex Val = fieldVec.dot(sourceVec);
	//Sym
	sourceVec.setZero();
	sourceVec(sourceID) = 1;
	_mvptool.MVP(imp->CGetGreen(), fieldVec);
	dcomplex Sym = sourceVec.dot(fieldVec);

	Console->debug("GRef     =({0},{1})\tID={2}", GreenRef.real(), GreenRef.imag(),greenID);
	Console->debug("GRefSym  =({0},{1})\tID={2}", GreenRef_.real(), GreenRef_.imag(), greenID_);
	Console->debug("Reference=({0},{1})", ref.real(), ref.imag());
	Console->debug("Value    =({0},{1})",Val.real(), Val.imag());
	Console->debug("Symmetry =({0},{1})", Sym.real(), Sym.imag());
	EXPECT_NEAR(Val.real(), ref.real(), 1.0e-6);
	EXPECT_NEAR(Val.imag(), ref.imag(), 1.0e-6);
	
	delete fillingTool;
}

#ifdef _DEBUG
TEST_F(AIMArrayTest, GetFarFieldApproximateFunction)
{
	//Initial Current
	InitialImpConfig();

	AIMArray* fillingTool = new AIMArray(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
	auto& bf = ComponentList::BFvector;
	fillingTool->_green = IGreen::GetInstance();

	Console->debug("Allocate the MatrixSetting oject");
	fillingTool->MultipoleExpansion(bf);
	Vector3i arrayBias{ 0,0,0 };
	//TEST FarField

	const dcomplex refZ1 = { -0.0080060247635108418 ,-0.0019764567076286750 };//Z(18£¬123)
	const dcomplex compZ1 = fillingTool->GetFarFieldApproximateImpedacne(18, 123, arrayBias);
	EXPECT_NEAR(refZ1.real(), compZ1.real(), 1.0e-6) << "Error in (18, 123) real";
	EXPECT_NEAR(refZ1.imag(), compZ1.imag(), 1.0e-6) << "Error in (18, 123) imag";

	const dcomplex refZ2 = { -0.0027595945849962643,0.0059947329121803053 };//Z(647, 22)
	const dcomplex compZ2 = fillingTool->GetFarFieldApproximateImpedacne(647, 22, arrayBias);
	EXPECT_NEAR(refZ2.real(), compZ2.real(), 1.0e-6) << "Error in (647, 22) real";
	EXPECT_NEAR(refZ2.imag(), compZ2.imag(), 1.0e-6) << "Error in (647, 22) imag";

	const dcomplex refZ3 = { 0.0034725828079872414, 0.0047437269001039282 };//Z(38, 123)
	const dcomplex compZ3 = fillingTool->GetFarFieldApproximateImpedacne(38, 123, arrayBias);
	EXPECT_NEAR(refZ3.real(), compZ3.real(), 1.0e-6) << "Error in (38, 123) real";
	EXPECT_NEAR(refZ3.imag(), compZ3.imag(), 1.0e-6) << "Error in (38, 123) imag";

	const dcomplex refZ4 = { -0.0067436242889280231 , 0.0033305745353076361 };//Z(17,653)
	const dcomplex compZ4 = fillingTool->GetFarFieldApproximateImpedacne(17, 653, arrayBias);
	EXPECT_NEAR(refZ4.real(), compZ4.real(), 1.0e-6) << "Error in (17,653) real";
	EXPECT_NEAR(refZ4.imag(), compZ4.imag(), 1.0e-6) << "Error in (17,653) imag";

	const dcomplex refZ9 = { -0.0034002845471970793 , 0.0033173618753603450 };//Z(18,654)
	const dcomplex compZ9 = fillingTool->GetFarFieldApproximateImpedacne(18, 654, arrayBias);
	EXPECT_NEAR(refZ9.real(), compZ9.real(), 1.0e-6) << "Error in (18,654) real";
	EXPECT_NEAR(refZ9.imag(), compZ9.imag(), 1.0e-6) << "Error in (18,654) imag";

	delete fillingTool;
}

TEST_F(AIMArrayTest, Multiplication)
{
	//Initial Current
	InitialImpConfig();

#pragma region Initial Temporary Parameters
	VectorXi weight{ VectorXi::Zero(5) }, greenWeightAcu{ weight };
	weight << SystemConfig.ImpConfig.xNumber,
		SystemConfig.ImpConfig.yNumber,
		SystemConfig.ImpConfig.zNumber,
		SystemConfig.ImpConfig.ArrayNumX,
		SystemConfig.ImpConfig.ArrayNumY;
	VectorXi greenWeight = 2 * weight.array() - 1;
	greenWeightAcu(0) = 1;
	for (int i = 1;i < greenWeightAcu.size();i++)
	{
		greenWeightAcu(i) = greenWeight.head(i).prod();
	}
#pragma endregion 

#pragma region AIMArrayAPI
	equation = IE::FIE(SystemConfig.IEConfig.type);
	AIMArray* fillingTool = new AIMArray(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
	auto& bf = ComponentList::BFvector;
	ImpArrayAIM* imp = static_cast<ImpArrayAIM*>(ComponentList::ImpService);
	
	//Multipole	
	fillingTool->MultipoleExpansion(bf);
	
	//Get Green UnFFT
	char* path = "E:/ZMC/Code/C_program/MUL/SourceData/ArrayGreen.dat";
	fillingTool->_green = IGreen::GetInstance();

	bool readflag = true;
	imp->GetGreen() = GetGreenBase(path, fillingTool, readflag);

	//Initial multiplicator
	
	//5-dimension FFT VERY IMPORTANT
	MKL_LONG layer[5] = { greenWeight[4] ,greenWeight[3], greenWeight[2] ,greenWeight[1], greenWeight[0]};
	_mvptool.Reset(5, layer);

	_mvptool.fwd(imp->GetGreen());//FFT
#pragma endregion 

	Console->info("Test Multiplication of AIMARRAY:");
	//size_t Row[9] = {18,647,38,47,111,333,478,598,901},Col[9] = {964,825,747,666,532,401,369,267,147};
	Matrix<int, 3, 16> ArrayBias;
	ArrayBias.row(0) << 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3;
	ArrayBias.row(1) << 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3;
	ArrayBias.row(2).setZero();
	for(int id=0;id<16;)
	{
		Vector3i arrayBias{ rand() % SystemConfig.ImpConfig.ArrayNumX,
			rand()% SystemConfig.ImpConfig.ArrayNumY,0 };
		if(arrayBias.sum()==0)continue;
		else id++;

		size_t row = rand() % SystemConfig.ImpConfig.ImpSize,col = rand() % SystemConfig.ImpConfig.ImpSize, unitCol = col;
		//Vector3i arrayBias{ ArrayBias.col(id++) };
		//size_t row = 1227, col = 1022, unitCol = col;
		
		const dcomplex refZ1 = fillingTool->GetFarFieldApproximateImpedacne(row, col, arrayBias);

		col = col + SystemConfig.ImpConfig.ImpSize*(arrayBias.x() + SystemConfig.ImpConfig.ArrayNumX*arrayBias.y());

		Console->info("Case\t{0}:\tUnit({1},{2})\tImp({3},{4})\tArrayBias({5},{6})", 
			id, row, unitCol,row,col,arrayBias.x(), arrayBias.y());

		dcomplex compZ1 = GetImpedance(row, col);
		dcomplex compZ2 = GetImpedance(col, row);
		Console->debug("Ref=({0},{1})", refZ1.real(), refZ1.imag());
		Console->debug("Res=({0},{1})", compZ1.real(), compZ1.imag());
		Console->debug("Sym=({0},{1})", compZ2.real(), compZ2.imag());
		EXPECT_LT(norm(refZ1-compZ1)/norm(refZ1), 1.0e-6)<<' ';
	}

	delete fillingTool;
}

TEST_F(AIMArrayTest, Multiplication2)
{
	//Initial Current
	InitialImpConfig();

#pragma region Initial Temporary Parameters
	VectorXi weight{ VectorXi::Zero(5) }, greenWeightAcu{ weight };
	weight << SystemConfig.ImpConfig.xNumber,
		SystemConfig.ImpConfig.yNumber,
		SystemConfig.ImpConfig.zNumber,
		SystemConfig.ImpConfig.ArrayNumX,
		SystemConfig.ImpConfig.ArrayNumY;
	VectorXi greenWeight = 2 * weight.array() - 1;
	greenWeightAcu(0) = 1;
	for (int i = 1;i < greenWeightAcu.size();i++)
	{
		greenWeightAcu(i) = greenWeight.head(i).prod();
	}
#pragma endregion 

#pragma region AIMArrayAPI
	AIMArray* fillingTool = new AIMArray(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
	auto& bf = ComponentList::BFvector;
	ImpArrayAIM* imp = static_cast<ImpArrayAIM*>(ComponentList::ImpService);

	//Multipole	
	fillingTool->MultipoleExpansion(bf);

	//Get Green UnFFT
	char* path = "E:/ZMC/Code/C_program/MUL/SourceData/ArrayGreen.dat";
	fillingTool->_green = IGreen::GetInstance();

	bool readflag = true;
	imp->GetGreen() = GetGreenBase(path, fillingTool, readflag);

	//5-dimension FFT VERY IMPORTANT
	MKL_LONG layer[5] = { greenWeight[4] ,greenWeight[3], greenWeight[2] ,greenWeight[1], greenWeight[0] };
	imp->_fftTools = new MulFFTMultiplicator;
	imp->_fftTools->Reset(5, layer);
	imp->_fftTools->fwd(imp->GetGreen());//FFT

	size_t unknowns = SystemConfig.ImpConfig.ArrayNumX*SystemConfig.ImpConfig.ArrayNumX*SystemConfig.ImpConfig.ImpSize;

#pragma endregion 

	Console->info("Test Multiplication of AIMARRAY:");
	//size_t Row[9] = {18,647,38,47,111,333,478,598,901},Col[9] = {964,825,747,666,532,401,369,267,147};
	Matrix<int, 3, 16> ArrayBias;
	ArrayBias.row(0) << 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3;
	ArrayBias.row(1) << 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3;
	ArrayBias.row(2).setZero();
	for (int id = 0;id < 16;)
	{
		Vector3i arrayBias{ rand() % SystemConfig.ImpConfig.ArrayNumX,
			rand() % SystemConfig.ImpConfig.ArrayNumY,0 };
		if (arrayBias.sum() == 0)continue;
		else id++;

		size_t row = rand() % SystemConfig.ImpConfig.ImpSize, col = rand() % SystemConfig.ImpConfig.ImpSize, unitCol = col;
		//Vector3i arrayBias{ ArrayBias.col(id++) };
		//size_t row = 1227, col = 1022, unitCol = col;

		const dcomplex refZ1 = fillingTool->GetFarFieldApproximateImpedacne(row, col, arrayBias);

		col = col + SystemConfig.ImpConfig.ImpSize*(arrayBias.x() + SystemConfig.ImpConfig.ArrayNumX*arrayBias.y());

		Console->info("Case\t{0}:\tUnit({1},{2})\tImp({3},{4})\tArrayBias({5},{6})",
			id, row, unitCol, row, col, arrayBias.x(), arrayBias.y());

		VectorXcd I1{ VectorXcd::Zero(unknowns) }, I2{ I1 };
		I1(row) = 1;
		I2(col) = 1;

		VectorXcd Res1 = imp->FarFieldMultiplication(I1);
		VectorXcd Res2 = imp->FarFieldMultiplication(I2);
		dcomplex compZ1 = Res1(col), compZ2 = Res2(row);
		Console->debug("Ref=({0},{1})", refZ1.real(), refZ1.imag());
		Console->debug("Res=({0},{1})", compZ1.real(), compZ1.imag());
		Console->debug("Sym=({0},{1})", compZ2.real(), compZ2.imag());
		EXPECT_LT(norm(refZ1 - compZ1) / norm(refZ1), 1.0e-6) << ' ';
	}

	delete fillingTool;
}
#endif

TEST_F(AIMArrayTest, NearFieldFilling)
{
	try
	{

		//Initial Current
		InitialImpConfig();
		AIMArray* fillingTool = new AIMArray(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
		auto& bf = ComponentList::BFvector;

		fillingTool->TriangleFillingStrategy(*Mesh::GetInstance(), bf);

		delete fillingTool;
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		FAIL();
	}
}


TEST_F(AIMArrayTest, Solving)
{
	try
	{
		auto curInfo = Solution::CurrentInfo::GetInstance();
		ASSERT_EQ(0, Core::SetCurrent());
		Console->info("{0:*^40}","OLD Current");
		Console->info("Category:\t{0}", curInfo->category);
		Console->info("NumOfConfig:\t{0}", curInfo->_numberOfConfig);
		Console->info("Size of Current:\t{0}", curInfo->Current.size());
		for(auto val:curInfo->Current)
		{
			auto current = static_cast<Solution::ArrayCurrent*>(val);
			Console->info("{0:-^30}", current->Tag);
			Console->info("Frequency:\t{0}", current->_frequency);
			Console->info("NumOfElements:\t{0}", current->_numberOfElement);
			Console->info("ElementUnknowns:\t{0}", current->_elementUnknowns);
			Console->info("TotalUnknowns:\t{0}", current->_unknowns);
			Console->info("ArrayIntervalX:\t{0}", current->_arrayBiasX);
			Console->info("ArrayIntervalY:\t{0}", current->_arrayBiasY);
			Console->info("DataSize:\t{0}", current->_data.size());
			Console->info("DataCapacity:\t{0}", current->_data.capacity());
			Console->info("{0:&^20}","ARRAYLOCATION");
			for(Vector2i loc:current->_arrayLocation)
			{
				Console->info("Location:\t({0},{1})",loc.x(),loc.y());
			}
			
			
		}

		
#pragma region Initial Current
		
		//curInfo->Reformat(SystemConfig.ImpConfig.ImpType);
		//curInfo->Current.push_back(new Solution::ArrayCurrent(ComponentList::BFvector.size(), 3.0e8, "AIMArrayTest", 2, SystemConfig.ImpConfig.ArrayIntervalX, SystemConfig.ImpConfig.ArrayIntervalY));
		//Solution::ArrayCurrent* cu = static_cast<Solution::ArrayCurrent*>(curInfo->Current.back());
		//
		//
		////SystemConfig.ImpConfig.ArrayLocation.array() = true;
		//curInfo->_numberOfConfig = curInfo->Current.size();

		//
		//cu->_arrayLocation.clear();
		//for (int rowx = 0;rowx < SystemConfig.ImpConfig.ArrayNumX;rowx++)
		//{
		//	for (int coly = 0;coly < SystemConfig.ImpConfig.ArrayNumY;coly++)
		//	{
		//		cu->_arrayLocation.push_back({ rowx,coly });
		//	}
		//}
		//Console->info("Size of ArrayLocation={0}", cu->_arrayLocation.size());
		//
		//Console->info("{0:*^40}", "New Current");
		//Console->info("Category:\t{0}", curInfo->category);
		//Console->info("NumOfConfig:\t{0}", curInfo->_numberOfConfig);
		//Console->info("Size of Current:\t{0}", curInfo->Current.size());
		//for (auto val : curInfo->Current)
		//{
		//	auto current = static_cast<Solution::ArrayCurrent*>(val);
		//	Console->info("{0:-^30}", current->Tag);
		//	Console->info("Frequency:\t{0}", current->_frequency);
		//	Console->info("NumOfElements:\t{0}", current->_numberOfElement);
		//	Console->info("ElementUnknowns:\t{0}", current->_elementUnknowns);
		//	Console->info("TotalUnknowns:\t{0}", current->_unknowns);
		//	Console->info("ArrayIntervalX:\t{0}", current->_arrayBiasX);
		//	Console->info("ArrayIntervalY:\t{0}", current->_arrayBiasY);
		//	Console->info("DataSize:\t{0}", current->_data.size());
		//	Console->info("DataCapacity:\t{0}", current->_data.capacity());
		//	Console->info("{0:&^20}", "ARRAYLOCATION");
		//	for (Vector2i loc : current->_arrayLocation)
		//	{
		//		Console->info("Location:\t({0},{1})", loc.x(), loc.y());
		//	}


		//}
#pragma endregion 
		
		ASSERT_EQ(0, Core::CalculatedMissionCentre());

		//cu->EMCParameterUpdate();
		//equation = IE::FIE(SystemConfig.IEConfig.type);
		//
		//ASSERT_EQ(0, Core::PreCalculateSelfTriangleImpedance()) << "Error in Pre-compute the SelfTriangle Impedance";
		//ASSERT_EQ(0, Core::InitialSolverAndImpedance()) << "Error in Initial the Impedance class";

		//ComponentList::ImpService->FillImpedance();

		/////*AIMArray* fillingTool = new AIMArray(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
		////auto& bf = ComponentList::BFvector;
		////fillingTool->MultipoleExpansion(bf);	
		////fillingTool->GreenMatrixSet(IGreen::GetInstance());	
		////fillingTool->TriangleFillingStrategy(*Mesh::GetInstance(), ComponentList::BFvector);
		////delete fillingTool;*/

		//ASSERT_EQ(0, Core::SetRightHand()) << "Error in Set RightHand";
		//auto info = Core::Solve();
		//ASSERT_EQ(0, info) << "Error in Solve Matrix with BicgStab";
		//

		//Request::FarField::RCS.resize(
		//	Solution::CurrentInfo::GetInstance()->_numberOfConfig, SystemConfig.PostConfig.size());
		//ASSERT_EQ(0, CalculateRequest(0));

		ASSERT_EQ(0, Core::SaveResults()) << "Error in Save Results";
		
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		FAIL();
	}
}
#endif