#include "stdafx.h"
#include "Current.h"
#include "FarField.h"

#ifdef GTEST
#include "gtest/gtest.h"
#include "VirtualGrid.h"
#include "CoreAPI.h"
#include "Const.h"
#include "Data.h"

using namespace Core;
using namespace Eigen;
using namespace AIMAssist;

class VirtualGridTest:public testing::Test
{
protected:
	RWGImpOperator _compute;
	VirtualGridTest():_compute(k,W4,W7,eta){}
	~VirtualGridTest(){}

	static void SetUpTestCase()
	{
		try
		{
			SystemConfig.ImpConfig.ImpType = AIM;
			SystemConfig.IEConfig.type = EFIE;
			SystemConfig.SolverConfig.Precond = Solution::ILU;
			ASSERT_EQ(0, Core::DataInitialization()) << "Error in Initialization";
			srand(static_cast<unsigned>(time(nullptr)));

			auto curInfo = Solution::CurrentInfo::GetInstance();
			curInfo->Reformat(SystemConfig.ImpConfig.ImpType);
			curInfo->Current.push_back(
				new Solution::ElementCurrent(ComponentList::BFvector.size(), 3.0e8, "MoMTest"));
			curInfo->_numberOfConfig = curInfo->Current.size();

			curInfo->Current.back()->EMCParameterUpdate();
			equation = IE::FIE(SystemConfig.IEConfig.type);

			ASSERT_EQ(0, Core::PreCalculateSelfTriangleImpedance()) << "Error in Pre-compute the SelfTriangle Impedance";
			ASSERT_EQ(0, Core::InitialSolverAndImpedance()) << "Error in Initial the Impedance class";
			
			//Random initial
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

	dcomplex GetImpedance(size_t row,size_t col)
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
		return (tempx + tempy + tempz - tempd / (k*k))*1i*k*eta;
	}
	MulFFTMultiplicator _mvptool;
};


TEST_F(VirtualGridTest, MultipoleExpansion)
{
	VirtualGrid* fillingTool = new VirtualGrid(SystemConfig.ImpConfig, ComponentList::ImpService,SystemConfig.IEConfig);
	auto& bf = ComponentList::BFvector;
	Console->debug("Allocate the MatrixSetting oject");
	int testNumber[3] = { 1,228,675 };

	for (int i = 0; i <3; ++i)
	{
		MatrixXcd coef = fillingTool->MultiExpTool(static_cast<RWG*>(bf[testNumber[i]]));
		Console->debug("Test BasicFunction Index:{}", testNumber[i]);
		for (auto element : fillingTool->MultiExpTool.Index)
		{
			Console->debug("[{0:5d} {1:5d} {2:5d} {3:5d}]",element.x(), element.y(), element.z(), element.w());
		}
	}
	
	delete fillingTool;
}


TEST_F(VirtualGridTest, GreenBase)
{
	VirtualGrid* fillingTool = new VirtualGrid(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
	auto& bf = ComponentList::BFvector;
	IGreen* green = IGreen::GetInstance();

	Console->debug("Allocate the MatrixSetting oject");
	fillingTool->GreenMatrixSet(green);
	VectorXcd gb= fillingTool->GetGreenBase();
	//max of xyz=[27 27 27]
	//Vector3i weight={ SystemConfig.ImpConfig.xNumber,SystemConfig.ImpConfig.xNumber*SystemConfig.ImpConfig.yNumber,1 };
	Vector3i weight = { 1,SystemConfig.ImpConfig.xNumber,SystemConfig.ImpConfig.xNumber*SystemConfig.ImpConfig.yNumber };
	for(int i=1;i<=10;i++)
	{
		Vector3i greenBias{ rand() % SystemConfig.ImpConfig.xNumber,rand() % SystemConfig.ImpConfig.yNumber,rand() % SystemConfig.ImpConfig.zNumber };
		Index location = weight.dot(greenBias);
		Console->debug("Green Bias is [{0:5d},{1:5d},{2:5d}]", greenBias.x(),greenBias.y(), greenBias.z());
		dcomplex res = green->Scalar(Vector3d::Zero(), SystemConfig.ImpConfig.Interval*greenBias.cast<double>());

		EXPECT_NEAR(gb(location).real(), res.real(), 1.0e-4) << "Error in %d real",i;
		EXPECT_NEAR(gb(location).imag(), res.imag(), 1.0e-4) << "Error in %d imag",i;
	}
	
	//constructedIterated
	unsigned bias = 0;
	VectorXcd GREEN = fillingTool->constructIterated(bias, 2);
	int mid = SystemConfig.ImpConfig.xNumber - 1;
	for(int i=1;i<SystemConfig.ImpConfig.xNumber-1;++i)
	{
		Console->debug("Index{0:5d}: {1:12.7f} {2:12.7f}i", mid + i, GREEN(mid + i).real(), GREEN(mid + i).imag());
		EXPECT_NEAR(GREEN(mid + i).real(), GREEN(mid - i).real(), 1.0e-6) << "Error in %d real",mid+i;
		EXPECT_NEAR(GREEN(mid + i).imag(), GREEN(mid - i).imag(), 1.0e-6) << "Error in %d imag",mid+i;
		
	}

	delete fillingTool;
}



TEST_F(VirtualGridTest, ApproximationFarField)
{
	VirtualGrid* fillingTool = new VirtualGrid(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
	auto& bf = ComponentList::BFvector;
	IGreen* green = IGreen::GetInstance();

	Console->debug("Allocate the MatrixSetting oject");
	fillingTool->MultipoleExpansion(bf);
	fillingTool->GreenMatrixSet(green);
	VectorXcd gb = fillingTool->GetGreenBase();

	//TEST FarField

	const dcomplex refZ1 = { -0.0080060247635108418 ,-0.0019764567076286750};//Z(18£¬123)
	const dcomplex compZ1 = fillingTool->GetImpAIM(18, 123);
	EXPECT_NEAR(refZ1.real(), compZ1.real(), 1.0e-6) << "Error in (18, 123) real";
	EXPECT_NEAR(refZ1.imag(), compZ1.imag(), 1.0e-6) << "Error in (18, 123) imag";

	const dcomplex refZ2 = { -0.0027595945849962643,0.0059947329121803053 };//Z(647, 22)
	const dcomplex compZ2 = fillingTool->GetImpAIM(647, 22);
	EXPECT_NEAR(refZ2.real(), compZ2.real(), 1.0e-6) << "Error in (647, 22) real";
	EXPECT_NEAR(refZ2.imag(), compZ2.imag(), 1.0e-6) << "Error in (647, 22) imag";

	const dcomplex refZ3 = { 0.0034725828079872414, 0.0047437269001039282 };//Z(38, 123)
	const dcomplex compZ3 = fillingTool->GetImpAIM(38, 123);
	EXPECT_NEAR(refZ3.real(), compZ3.real(), 1.0e-6) << "Error in (38, 123) real";
	EXPECT_NEAR(refZ3.imag(), compZ3.imag(), 1.0e-6) << "Error in (38, 123) imag";

	const dcomplex refZ4 = { -0.0067436242889280231 , 0.0033305745353076361 };//Z(17,653)
	const dcomplex compZ4 = fillingTool->GetImpAIM(17, 653);
	EXPECT_NEAR(refZ4.real(), compZ4.real(), 1.0e-6) << "Error in (17,653) real";
	EXPECT_NEAR(refZ4.imag(), compZ4.imag(), 1.0e-6) << "Error in (17,653) imag";

	const dcomplex refZ9 = { -0.0034002845471970793 , 0.0033173618753603450 };//Z(18,654)
	const dcomplex compZ9= fillingTool->GetImpAIM(18,654);
	EXPECT_NEAR(refZ9.real(), compZ9.real(), 1.0e-6) << "Error in (18,654) real";
	EXPECT_NEAR(refZ9.imag(), compZ9.imag(), 1.0e-6) << "Error in (18,654) imag";

	delete fillingTool;
}

TEST_F(VirtualGridTest,Multiplication)
{
	VirtualGrid* fillingTool = new VirtualGrid(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
	auto& bf = ComponentList::BFvector;
	Console->debug("Allocate the MatrixSetting oject");

	fillingTool->MultipoleExpansion(bf);
	fillingTool->GreenMatrixSet(IGreen::GetInstance());
	
	//Initial multiplicator
	MKL_LONG dim = 3, layer[3] = {2* SystemConfig.ImpConfig.zNumber-2,
		2*SystemConfig.ImpConfig.yNumber-2,
		2*SystemConfig.ImpConfig.xNumber-2 };
	_mvptool.Reset(dim, layer);
		
	
	const dcomplex refZ1 = { -0.0080060247635108418 ,-0.0019764567076286750 };//Z(18£¬123)
	const dcomplex compZ1 = GetImpedance(18, 123);
	EXPECT_NEAR(refZ1.real(), compZ1.real(), 1.0e-6) << "Error in (18, 123) real";
	EXPECT_NEAR(refZ1.imag(), compZ1.imag(), 1.0e-6) << "Error in (18, 123) imag";

	const dcomplex refZ2 = { -0.0027595945849962643,0.0059947329121803053 };//Z(647, 22)
	const dcomplex compZ2 = GetImpedance(647, 22);
	EXPECT_NEAR(refZ2.real(), compZ2.real(), 1.0e-6) << "Error in (647, 22) real";
	EXPECT_NEAR(refZ2.imag(), compZ2.imag(), 1.0e-6) << "Error in (647, 22) imag";

	const dcomplex refZ3 = { 0.0034725828079872414, 0.0047437269001039282 };//Z(38, 123)
	const dcomplex compZ3 = GetImpedance(38, 123);
	EXPECT_NEAR(refZ3.real(), compZ3.real(), 1.0e-6) << "Error in (38, 123) real";
	EXPECT_NEAR(refZ3.imag(), compZ3.imag(), 1.0e-6) << "Error in (38, 123) imag";

	const dcomplex refZ4 = { -0.0067436242889280231 , 0.0033305745353076361 };//Z(17,653)
	const dcomplex compZ4 = GetImpedance(17, 653);
	EXPECT_NEAR(refZ4.real(), compZ4.real(), 1.0e-6) << "Error in (17,653) real";
	EXPECT_NEAR(refZ4.imag(), compZ4.imag(), 1.0e-6) << "Error in (17,653) imag";

	const dcomplex refZ9 = { -0.0034002845471970793 , 0.0033173618753603450 };//Z(18,654)
	const dcomplex compZ9 = GetImpedance(18, 654);
	EXPECT_NEAR(refZ9.real(), compZ9.real(), 1.0e-6) << "Error in (18,654) real";
	EXPECT_NEAR(refZ9.imag(), compZ9.imag(), 1.0e-6) << "Error in (18,654) imag";
}

TEST_F(VirtualGridTest, NearFieldCompareTest)
{
	try
	{
		VirtualGrid* fillingTool = new VirtualGrid(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
		auto& bf = ComponentList::BFvector;
		IGreen* green = IGreen::GetInstance();

		Console->debug("Allocate the MatrixSetting oject");
		fillingTool->MultipoleExpansion(bf);
		fillingTool->GreenMatrixSet(green);
		VectorXcd gb = fillingTool->GetGreenBase();

		const size_t unknowns = SystemConfig.ImpConfig.ImpSize;
		const double threshold = SystemConfig.ImpConfig.Threshold*Lambda;

		Console->debug("Matrix Far Field Random Setting Test:");

		ofstream ofs;
		string filename = SystemConfig.ProjectDir + '\\' + "NearFieldComparison.csv";
		ofs.open(filename, ios_base::out);
		if (!ofs.is_open())throw spd::spdlog_ex("Error in " + filename);

		const int row = 256;
		int count=0;
		for (int col = 0; col < unknowns;col++)
		{			
			const auto source = static_cast<RWG*>(bf[row]);
			const auto field = static_cast<RWG*>(bf[col]);
			Vector3d distance = field->Centre() - source->Centre();
			const double dnorm = distance.norm();
			if (dnorm < 1.4*Lambda)
			{
				const dcomplex ref = _compute.SetImpedanceL(field, source);
				const dcomplex comp = fillingTool->GetImpAIM(row, col);
				double error = abs(ref - comp) / abs(ref);
				if(dnorm<0.4*Lambda)Console->debug("\nNumber:\t{0}\nImpedance:\t({1},{2})\nDistance:\t{3}¦Ë\nreference:\t({4},{5})\nNear:\t\t({6},{7})\nError:\t\t{8}%",
					++count, row, col, dnorm / Lambda, ref.real(), ref.imag(), comp.real(), comp.imag(),error*100);
				
				ofs << dnorm << ',' << distance.x()<<','<< distance.y()<<','<< distance.z() << ',' << error << '\n';
			}			
		}

		ofs.flush();
		ofs.close();
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		FAIL();
	}
}
TEST_F(VirtualGridTest, Solving)
{
	try
	{
		//throw spd::spdlog_ex("AIMCalculate is not Testing");

		VirtualGrid* fillingTool = new VirtualGrid(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
		auto& bf = ComponentList::BFvector;
		Console->debug("Allocate the MatrixSetting oject");

		fillingTool->MultipoleExpansion(bf);
		fillingTool->GreenMatrixSet(IGreen::GetInstance());
		fillingTool->TriangleFillingStrategy(*Mesh::GetInstance(), ComponentList::BFvector);
		
		ASSERT_EQ(0, Core::SetRightHand()) << "Error in Set RightHand";
		auto info = Core::Solve();
		ASSERT_EQ(0, info) << "Error in Solve Matrix with BicgStab";

		Request::FarField::RCS.resize(
			Solution::CurrentInfo::GetInstance()->_numberOfConfig, SystemConfig.PostConfig.size());
		ASSERT_EQ(0, CalculateRequest(0));
		ASSERT_EQ(0, Core::SaveResults()) << "Error in Save Results";
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		FAIL();
	}
}
#endif