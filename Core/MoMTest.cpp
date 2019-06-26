#include "stdafx.h"
#include "Data.h"
#include "IntegrationRWG.h"
#include "ImpMoM.h"
#ifdef GTEST
#include "gtest/gtest.h"
#include "CoreAPI.h"
#include "Const.h"

using namespace Core;
using namespace Eigen;

class MoMTest:public testing::Test
{
public:
	static void SetUpTestCase()
	{
		try
		{
			SystemConfig.ImpConfig.impType = MoM;
			if (Mesh::GetInstance()->IsLock())
			{
				ASSERT_EQ(0, Core::CreatMesh()) << "Error in Creat Mesh";
				ASSERT_EQ(0, Core::CreatBasicFunction(false)) << "Error in Load BasicFunction";
			}
			if (ComponentList::BFvector.size() < 1)ASSERT_EQ(0, Core::CreatBasicFunction(false)) << "Error in Load BasicFunction";
			if (!Core::IGreen::GetInstance())EXPECT_EQ(0, Core::SetGreenFunction());
			ASSERT_EQ(0, Core::PreCalculateSelfTriangleImpedance()) << "Error in Pre-compute the SelfTriangle Impedance";
			ASSERT_EQ(0, Core::CreatImpedance()) << "Error in Initial the Impedance class";
			ASSERT_EQ(0, Core::FillImpedance()) << "Error in Fill Impedance";
			
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
		if (Solver)
		{
			delete Solver;
			Solver = nullptr;
			Console->debug("Release Solver");
		}
	}
};

TEST_F(MoMTest, ImpedanceTest)
{
	
	try
	{
		RWGImpOperator compute(k, W4, W7, eta);
		//验证阻抗矩阵
		MatrixXcd& Imp = static_cast<ImpMoM*>(ComponentList::ImpService)->LocalMatrix();
		auto mesh = Mesh::GetInstance();
		auto& bf = ComponentList::BFvector;
		//元素（153,451）比较
		const int row = 152, col = 450;
		dcomplex pp, pm, mp, mm;

		auto T228 = compute.OperatorL(dynamic_cast<RWGTriangle*>(mesh->GetTriangle(227)));
		for (auto zmc = T228.begin(); zmc != T228.end(); ++zmc)
		{
			const int i = get<0>(*zmc), j = get<1>(*zmc);
			if (row + col == i + j) { mm = get<2>(*zmc); break; }
		}
		auto T66_227 = compute.SetImpedanceL(dynamic_cast<RWGTriangle*>(mesh->GetTriangle(65)), dynamic_cast<RWGTriangle*>(mesh->GetTriangle(226)));
		for (auto zmc = T66_227.begin(); zmc != T66_227.end(); ++zmc)
		{
			const int i = get<0>(*zmc), j = get<1>(*zmc);
			if (row + col == i + j&&col - row == abs(i - j)) { pp = get<2>(*zmc); break; }
		}
		auto T66_228 = compute.SetImpedanceL(dynamic_cast<RWGTriangle*>(mesh->GetTriangle(65)), dynamic_cast<RWGTriangle*>(mesh->GetTriangle(227)));
		for (auto zmc = T66_228.begin(); zmc != T66_228.end(); ++zmc)
		{
			const int i = get<0>(*zmc), j = get<1>(*zmc);
			if (row + col == i + j&&col - row == abs(i - j)) { pm = get<2>(*zmc); break; }
		}
		auto T228_227 = compute.SetImpedanceL(dynamic_cast<RWGTriangle*>(mesh->GetTriangle(227)), dynamic_cast<RWGTriangle*>(mesh->GetTriangle(226)));
		for (auto zmc = T228_227.begin(); zmc != T228_227.end(); ++zmc)
		{
			const int i = get<0>(*zmc), j = get<1>(*zmc);
			if (row + col == i + j&&col - row == abs(i - j)) { mp = get<2>(*zmc); break; }
		}
		const dcomplex ref = compute.SetImpedanceL(static_cast<RWG*>(bf[row]), static_cast<RWG*>(bf[col]));
		const dcomplex comp = pp + pm + mp + mm;
		EXPECT_NEAR(0, norm(comp - ref) / norm(ref), 1.0e-2) << "(153,451)元素错误";

		const size_t unknowns = bf.size();
		
		for (size_t count = 0; count < 10000;++count)
		{
			const size_t r = rand() % unknowns;
			const size_t c = rand() % unknowns;
			const auto source = static_cast<RWG*>(bf[r]);
			const auto field = static_cast<RWG*>(bf[c]);
			const auto reference = compute.SetImpedanceL(field, source);
			EXPECT_NEAR(0, norm(reference - Imp(r, c)) / norm(reference), 1.0e-3) << "Error in Impedance\t" << r << "," << c;
			const double temp = abs(reference);
			if (temp > 5)
				Console->warn("Impedance({0},{1})=({2},{3}) with abs={4}>5", r, c, reference.real(), reference.imag(), temp);
		}
	}
	catch (spd::spdlog_ex&ex)
	{
		Console->warn(ex.what());
		RuntimeLog->warn(ex.what());
		RuntimeLog->flush();
	}
}



TEST_F(MoMTest,SolveTest)
{
	try
	{
		ASSERT_EQ(0, Core::SetRightHand()) << "Error in Set RightHand";
		auto info = Core::Solve();
		ASSERT_EQ(0, info) << "Error in Solve Matrix with BicgStab";
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