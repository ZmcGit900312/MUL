#include "stdafx.h"
#include "AIMPECCFIE.h"
#include "ImpMoM.h"

#ifdef GTEST
#include "gtest/gtest.h"
#include "CoreAPI.h"
#include "Const.h"
#include "Data.h"

using namespace Core;
using namespace Eigen;
using namespace AIMAssist;

class AIMPECCFIETest :public testing::Test
{
protected:
	RWGImpOperator _compute;
	AIMPECCFIETest() :_compute(k, W4, W7, eta) {}
	~AIMPECCFIETest() {}

	static void SetUpTestCase()
	{
		try
		{
			SystemConfig.ImpConfig.ImpType = AIM;

			SystemConfig.IEConfig.type = CFIE;
			SystemConfig.IEConfig.Alpha = 0.25;
			SystemConfig.IEConfig.Eta = 120 * M_PI;

			//SystemConfig.SolverConfig.Precond = Solution::ILU;
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

		//L
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

		dcomplex Lx = vrowx.dot(vcolx);
		dcomplex Ly = vrowy.dot(vcoly);
		dcomplex Lz = vrowz.dot(vcolz);
		dcomplex Ld = vrowd.dot(vcold);

		dcomplex L= (Lx + Ly + Lz - Ld / (k*k) )* 1i*k*eta;
		
		//K
		VectorXcd vsourcezy= imp->GetGammaZ().col(col),vsourcezx=vsourcezy;
		VectorXcd vsourceyx = imp->GetGammaY().col(col), vsourceyz = vsourceyx;
		VectorXcd vsourcexz = imp->GetGammaX().col(col), vsourcexy = vsourcexz;

		_mvptool.MVP(imp->GetGreenY(), vsourcezy);
		_mvptool.MVP(imp->GetGreenY(), vsourcexy);
		_mvptool.MVP(imp->GetGreenX(), vsourceyx);
		_mvptool.MVP(imp->GetGreenX(), vsourcezx);
		_mvptool.MVP(imp->GetGreenZ(), vsourceyz);
		_mvptool.MVP(imp->GetGreenZ(), vsourcexz);


		dcomplex Kx = vrowx.dot(vsourcezy- vsourceyz);
		dcomplex Ky = vrowy.dot(vsourcexz - vsourcezx);
		dcomplex Kz = vrowz.dot(vsourceyx - vsourcexy);
		
		dcomplex K = Kx + Ky + Kz;
		return L * SystemConfig.IEConfig.Alpha - (1 - SystemConfig.IEConfig.Alpha)*SystemConfig.IEConfig.Eta*K;
	}
	MulFFTMultiplicator _mvptool;
};


TEST_F(AIMPECCFIETest, MultipoleExpansion)
{

	AIMPECCFIE* fillingTool = new AIMPECCFIE(SystemConfig.ImpConfig, ComponentList::ImpService,SystemConfig.IEConfig);
	auto& bf = ComponentList::BFvector;
	Console->debug("Allocate the MatrixSetting oject");
	int testNumber[3] = { 1,228,675 };

	for (int i = 0; i < 3; ++i)
	{
		MatrixXcd coef = fillingTool->MultiExpTool(static_cast<RWG*>(bf[testNumber[i]]));
		Console->debug("Test BasicFunction Index:{}", testNumber[i]);
		for (auto element : fillingTool->MultiExpTool.Index)
		{
			Console->debug("[{0:5d} {1:5d} {2:5d} {3:5d}]", element.x(), element.y(), element.z(), element.w());
		}
	}

	delete fillingTool;
}


TEST_F(AIMPECCFIETest, GreenBase)
{
	AIMPECCFIE* fillingTool = new AIMPECCFIE(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
	auto& bf = ComponentList::BFvector;
	IGreen* green = IGreen::GetInstance();

	Console->debug("Allocate the MatrixSetting oject");
	fillingTool->GreenMatrixSet(green);
	VectorXcd g = fillingTool->GetGreenBase(0);
	VectorXcd gdx= fillingTool->GetGreenBase(1);
	VectorXcd gdy = fillingTool->GetGreenBase(2);
	VectorXcd gdz = fillingTool->GetGreenBase(3);
	//max of xyz=[27 27 27]
	//Vector3i weight={ SystemConfig.ImpConfig.xNumber,SystemConfig.ImpConfig.xNumber*SystemConfig.ImpConfig.yNumber,1 };
	Vector3i weight = { 1,SystemConfig.ImpConfig.xNumber,SystemConfig.ImpConfig.xNumber*SystemConfig.ImpConfig.yNumber };
	for (int i = 1;i <= 10;i++)
	{
		Vector3i greenBias{ rand() % SystemConfig.ImpConfig.xNumber,rand() % SystemConfig.ImpConfig.yNumber,rand() % SystemConfig.ImpConfig.zNumber };
		Index location = weight.dot(greenBias);
		Console->debug("Green Bias is [{0:5d},{1:5d},{2:5d}]", greenBias.x(), greenBias.y(), greenBias.z());
		dcomplex res = green->Scalar(SystemConfig.ImpConfig.Interval*greenBias.cast<double>(), Vector3d::Zero());
		Vector3cd resd = green->Gradient(SystemConfig.ImpConfig.Interval*greenBias.cast<double>(),Vector3d::Zero() );
		EXPECT_NEAR(g(location).real(), res.real(), 1.0e-4) << "Error in "<< i;
		EXPECT_NEAR(g(location).imag(), res.imag(), 1.0e-4) << "Error in "<<i;
		EXPECT_NEAR(gdx(location).real(), resd.x().real(), 1.0e-4) << "Error in " << i;
		EXPECT_NEAR(gdx(location).imag(), resd.x().imag(), 1.0e-4) << "Error in " << i;
		EXPECT_NEAR(gdy(location).real(), resd.y().real(), 1.0e-4) << "Error in " << i;
		EXPECT_NEAR(gdy(location).imag(), resd.y().imag(), 1.0e-4) << "Error in " << i;
		EXPECT_NEAR(gdz(location).real(), resd.z().real(), 1.0e-4) << "Error in " << i;
		EXPECT_NEAR(gdz(location).imag(), resd.z().imag(), 1.0e-4) << "Error in " << i;
	}

	//construct
	int layerx = 2 * SystemConfig.ImpConfig.xNumber - 1, layery = 2 * SystemConfig.ImpConfig.yNumber - 1, layerz = 2 * SystemConfig.ImpConfig.zNumber - 1;

	VectorXcd G = fillingTool->construct(0);
	//x component Test
	VectorXcd Gdx = fillingTool->construct(1);
	int biasXstart = layerx*(rand() % (layerx*layery)),biasXend = biasXstart + layerx;
	Console->info("Green Gradient X component Test");
	for (int i = 1;i < SystemConfig.ImpConfig.xNumber;++i)
	{
		int bias = biasXstart + i;
		Console->debug("Index{0:5d}: {1:12.7f} {2:12.7f}i", bias, Gdx(bias).real(), Gdx(bias).imag());
		Console->debug("Index{0:5d}: {1:12.7f} {2:12.7f}i", biasXend - i, Gdx(biasXend - i).real(), Gdx(biasXend - i).imag());
		EXPECT_NEAR(Gdx(bias).real(),-Gdx(biasXend -i).real(),1.0e-6)<< "Error in "<< bias;
		EXPECT_NEAR(Gdx(bias).imag(), -Gdx(biasXend - i).imag(), 1.0e-6) << "Error in "<< bias;
	}
	//y component Test
	VectorXcd Gdy = fillingTool->construct(2);
	int lengthy = (2 * SystemConfig.ImpConfig.xNumber - 1)*(2 * SystemConfig.ImpConfig.yNumber - 1);
	Console->info("Green Gradient y component Test");
	for (int i = 0; i < SystemConfig.ImpConfig.xNumber; ++i)
	{
		dcomplex baseValue = Gdy(2*SystemConfig.ImpConfig.xNumber -1+ i);
		int biasy = lengthy - 2*SystemConfig.ImpConfig.xNumber+1 + i;
		dcomplex ref = Gdy(biasy);
		Console->debug("Index{0:5d}: {1:12.7f} {2:12.7f}i", 2 * SystemConfig.ImpConfig.xNumber - 1 + i, baseValue.real(), baseValue.imag());
		Console->debug("Index{0:5d}: {1:12.7f} {2:12.7f}i", biasy, ref.real(), ref.imag());
		EXPECT_NEAR(baseValue.real(), -ref.real(), 1.0e-6) << "Error in "<< biasy;
		EXPECT_NEAR(baseValue.imag(), -ref.imag(), 1.0e-6) << "Error in "<< biasy;
	}
	//z component Test
	VectorXcd Gdz = fillingTool->construct(3);
	int lengthz = Gdz.size();
	Console->info("Green Gradient Z component Test");
	for(int i=0;i<SystemConfig.ImpConfig.xNumber-1;++i)
	{
		int basez = 2*lengthy + i;
		int biasz = lengthz - 2*lengthy + i;
		dcomplex baseValue = Gdz(basez),ref=Gdz(biasz);
		Console->debug("Index{0:5d}: {1:12.7f} {2:12.7f}i", basez, baseValue.real(), baseValue.imag());
		Console->debug("Index{0:5d}: {1:12.7f} {2:12.7f}i", biasz, ref.real(), ref.imag());
		EXPECT_NEAR(baseValue.real(), -ref.real(), 1.0e-6) << "Error in "<< biasz;
		EXPECT_NEAR(baseValue.imag(), -ref.imag(), 1.0e-6) << "Error in "<< biasz;
	}

	for(int i=0;i<Gdx.size();++i)
	{
		if (abs(G(i)) > 100)
		{
			Console->error("Error value={0}", abs(G(i)));
			Console->error("An error in Green({0})=({1},{2})", i, G(i).real(), G(i).imag());
			FAIL();
			//break;
		}

		if(abs(Gdx(i)) >100)
		{
			Console->error("Error value={0}",abs(Gdx(i)));
			Console->error("An error in GradientX({0})=({1},{2})", i,Gdx(i).real(),Gdx(i).imag());
			FAIL();
			break;
		}
		if (abs(Gdy(i)) > 100)
		{
			Console->error("Error value={0}", abs(Gdy(i)));
			Console->error("An error in GradientY({0})=({1},{2})", i, Gdy(i).real(), Gdy(i).imag());
			FAIL();
			break;
		}
		if (abs(Gdz(i)) > 100)
		{
			Console->error("Error value={0}", abs(Gdz(i)));
			Console->error("An error in GradientZ({0})=({1},{2})", i, Gdz(i).real(), Gdz(i).imag());
			FAIL();
			break;
		}
	}

	delete fillingTool;
}


TEST_F(AIMPECCFIETest, ApproximateFarFieldCFIE)
{
	AIMPECCFIE* fillingTool = new AIMPECCFIE(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
	auto& bf = ComponentList::BFvector;
	IGreen* green = IGreen::GetInstance();

	Console->debug("Allocate the MatrixSetting oject");
	fillingTool->MultipoleExpansion(bf);
	fillingTool->GreenMatrixSet(green);	

	//TEST FarField
	RWGImpOperator computeCore(Core::k, Core::W4, Core::W7, Core::eta);
	for (size_t count = 0, unknowns = bf.size(); count < 100;)
	{
		size_t row = rand() % unknowns;
		size_t col = rand() % unknowns;

		RWG* field = static_cast<RWG*>(bf[row]);
		RWG* source = static_cast<RWG*>(bf[col]);
		Vector3d edgeDistance = field->Centre() - source->Centre();
		if (edgeDistance.norm() > 0.3)
		{
			dcomplex refK = computeCore.SetImpedanceK(field, source);
			dcomplex refL = computeCore.SetImpedanceL(field, source);
			dcomplex comp = fillingTool->GetImpAIM(row, col);
			dcomplex ref = refL * SystemConfig.IEConfig.Alpha - (1 - SystemConfig.IEConfig.Alpha)*SystemConfig.IEConfig.Eta*refK;
			Console->debug("RWG:({0:5d},{1:5d})\tDis={2}\tRef=({3},{4})\tComp=({5},{6})", row, col, edgeDistance.norm(), ref.real(), ref.imag(), comp.real(), comp.imag());
			double val = abs(comp - ref) / abs(ref);
			if (val > 1.0e-1)EXPECT_FALSE(true) << "large than 0.1";
			if (val > 1.0e-2)Console->warn("RWG:({0},{1}) > 0.01", row, col);
			++count;
		}
	}

	delete fillingTool;
}


TEST_F(AIMPECCFIETest, MultiplicationCFIE)
{
	AIMPECCFIE* fillingTool = new AIMPECCFIE(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
	auto& bf = ComponentList::BFvector;
	IGreen* green = IGreen::GetInstance();

	Console->debug("Allocate the MatrixSetting oject");
	fillingTool->MultipoleExpansion(bf);
	fillingTool->GreenMatrixSet(green);

	//Initial multiplicator
	MKL_LONG dim = 3, layer[3] = { 2 * SystemConfig.ImpConfig.zNumber - 1,
		2 * SystemConfig.ImpConfig.yNumber - 1,
		2 * SystemConfig.ImpConfig.xNumber - 1 };
	_mvptool.Reset(dim, layer);
	RWGImpOperator computeCore(Core::k, Core::W4, Core::W7, Core::eta);


	for (size_t count = 0, unknowns = bf.size(); count < 100;++count)
	{
		size_t row = rand() % unknowns;
		size_t col = rand() % unknowns;
		
		/*row = 271, col = 422;*/

		RWG* field = static_cast<RWG*>(bf[row]);
		RWG* source = static_cast<RWG*>(bf[col]);
		Vector3d edgeDistance = field->Centre() - source->Centre();
			
		
		dcomplex ref = fillingTool->GetImpAIM(row, col);
		dcomplex comp = GetImpedance(row, col);
			
		Console->debug("RWG:({0:5d},{1:5d})\tDis={2}\tRef=({3},{4})\tComp=({5},{6})", row, col, edgeDistance.norm(), ref.real(), ref.imag(), comp.real(), comp.imag());
		EXPECT_NEAR(0, abs(comp - ref) / abs(ref), 1.0e-2);
			
	}

}

TEST_F(AIMPECCFIETest, Solving)
{
	try
	{
		//throw spd::spdlog_ex("AIMCalculate is not Testing");

		AIMPECCFIE* fillingTool = new AIMPECCFIE(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
		auto& bf = ComponentList::BFvector;
		Console->debug("Allocate the MatrixSetting oject");

		fillingTool->MultipoleExpansion(bf);
		fillingTool->GreenMatrixSet(IGreen::GetInstance());
		fillingTool->TriangleFillingStrategy(*Mesh::GetInstance(), ComponentList::BFvector);

		ASSERT_EQ(0, Core::SetRightHand()) << "Error in Set RightHand";
		auto info = Core::Solve();
		EXPECT_EQ(0, info) << "Error in Solve Matrix with BicgStab";
		if (info == 0)
		{
			EXPECT_EQ(0, Core::SaveBasicFunction(SystemConfig.BasicFunctionFileName.c_str())) << "Error in save BasicFunction";
			EXPECT_EQ(0, Core::CalculateRequest()) << "Error in Calculate the FarField";
		}
	}
	catch (spd::spdlog_ex&ex)
	{
		Console->warn(ex.what());
		RuntimeLog->warn(ex.what());
		RuntimeLog->flush();
	}
}
#endif