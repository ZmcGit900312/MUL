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
	static bool NotMoM;
	static void SetUpTestCase()
	{
		try
		{
			if (SystemConfiguration::ImpConfig.impType != MoM)throw spd::spdlog_ex("MoMTest is not Running");
			ASSERT_EQ(0, Core::CreatMesh()) << "Error in Creat Mesh";
			ASSERT_EQ(0, Core::CreatBasicFunction(false)) << "Error in Load BasicFunction";
			ASSERT_EQ(0, Core::SetGreenFunction()) << "Error in set Green Function";
			ASSERT_EQ(0, Core::PreCalculateSelfTriangleImpedance()) << "Error in Pre-compute the SelfTriangle Impedance";
			ASSERT_EQ(0, Core::CreatImpedance()) << "Error in Initial the Impedance class";
			ASSERT_EQ(0, Core::FillImpedance()) << "Error in Fill Impedance";
			ASSERT_EQ(0, Core::SetRightHand()) << "Error in Set RightHand";
			srand(static_cast<unsigned>(time(nullptr)));
			NotMoM = false;
		}
		catch (spd::spdlog_ex&ex)
		{
			NotMoM = true;
			Console->warn(ex.what());
			RuntimeL->warn(ex.what());
			RuntimeL->flush();
		}
	}

	static void TearDownTestCase()
	{
		if (ComponentList::MeshService) {
			delete ComponentList::MeshService; ComponentList::MeshService = nullptr;
			Console->debug("Release Mesh");
		}
		if (ComponentList::BFvector.size()>0)
		{
			for (auto element : ComponentList::BFvector) { delete element; }
			ComponentList::BFvector.clear();
			ComponentList::BFvector.shrink_to_fit();
			Console->debug("Release BasicFunction");
		}
		if (ComponentList::ImpService)
		{
			delete ComponentList::ImpService;
			ComponentList::ImpService = nullptr;
			Console->debug("Release Matrix");
		}
		if (ComponentList::Green)
		{
			delete ComponentList::Green;
			ComponentList::Green = nullptr;
			Console->debug("Release GreenFunction");
		}
	}
};
bool MoMTest::NotMoM=false;

TEST_F(MoMTest, ImpedanceTest)
{
	
	try
	{
		if (NotMoM)throw spd::spdlog_ex("ImpedanceTest is not Testing");
		//ASSERT_EQ(0, Core::FillImpedance()) << "Error in Fill Impedance";
		EFRImp compute(k, W4, W7, eta);
		//验证阻抗矩阵
		MatrixXcd& Imp = static_cast<ImpMoM*>(ComponentList::ImpService)->LocalMatrix();
		auto mesh = ComponentList::MeshService;
		auto& bf = ComponentList::BFvector;
		//元素（153,451）比较
		const int row = 152, col = 450;
		dcomplex pp, pm, mp, mm;

		auto T228 = compute.SetImpedance(mesh->GetTriangle(227));
		for (auto zmc = T228.begin(); zmc != T228.end(); ++zmc)
		{
			const int i = get<0>(*zmc), j = get<1>(*zmc);
			if (row + col == i + j) { mm = get<2>(*zmc); break; }
		}
		auto T66_227 = compute.SetImpedance(mesh->GetTriangle(65), mesh->GetTriangle(226));
		for (auto zmc = T66_227.begin(); zmc != T66_227.end(); ++zmc)
		{
			const int i = get<0>(*zmc), j = get<1>(*zmc);
			if (row + col == i + j&&col - row == abs(i - j)) { pp = get<2>(*zmc); break; }
		}
		auto T66_228 = compute.SetImpedance(mesh->GetTriangle(65), mesh->GetTriangle(227));
		for (auto zmc = T66_228.begin(); zmc != T66_228.end(); ++zmc)
		{
			const int i = get<0>(*zmc), j = get<1>(*zmc);
			if (row + col == i + j&&col - row == abs(i - j)) { pm = get<2>(*zmc); break; }
		}
		auto T228_227 = compute.SetImpedance(mesh->GetTriangle(227), mesh->GetTriangle(226));
		for (auto zmc = T228_227.begin(); zmc != T228_227.end(); ++zmc)
		{
			const int i = get<0>(*zmc), j = get<1>(*zmc);
			if (row + col == i + j&&col - row == abs(i - j)) { mp = get<2>(*zmc); break; }
		}
		const dcomplex ref = compute.SetImpedance(static_cast<RWG*>(bf[row]), static_cast<RWG*>(bf[col]));
		const dcomplex comp = pp + pm + mp + mm;
		EXPECT_NEAR(0, norm(comp - ref) / norm(ref), 1.0e-2) << "(153,451)元素错误";

		const size_t unknowns = bf.size();
		
		for (size_t count = 0; count < 10000;++count)
		{
			const size_t r = rand() % unknowns;
			const size_t c = rand() % unknowns;
			const auto source = static_cast<RWG*>(bf[r]);
			const auto field = static_cast<RWG*>(bf[c]);
			const auto reference = compute.SetImpedance(field, source);
			EXPECT_NEAR(0, norm(reference - Imp(r, c)) / norm(reference), 1.0e-3) << "Error in Impedance\t" << r << "," << c;
			const double temp = abs(reference);
			if (temp > 5)
				Console->warn("Impedance({0},{1})=({2},{3}) with abs={4}>5", r, c, reference.real(), reference.imag(), temp);
		}
	}
	catch (spd::spdlog_ex&ex)
	{
		Console->warn(ex.what());
		RuntimeL->warn(ex.what());
		RuntimeL->flush();
	}
}

TEST_F(MoMTest,SolveTest)
{
	try
	{
		if (NotMoM)throw spd::spdlog_ex("SolveTest is not Testing");
		auto info = Core::Solve();
		ASSERT_EQ(0, info) << "Error in Solve Matrix with BicgStab";
		if (info == 0)
		{
			EXPECT_EQ(0, Core::SaveBasicFunction(SystemConfiguration::BasicFunctionFileName.c_str())) << "Error in save BasicFunction";
			EXPECT_EQ(0, Core::CalculateRequest()) << "Error in Calculate the FarField";
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