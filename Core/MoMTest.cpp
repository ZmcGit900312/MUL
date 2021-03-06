#include "stdafx.h"
#include "Data.h"
#include "IntegrationRWG.h"
#include "ImpMoM.h"
#include "Current.h"
#ifdef GTEST
#include "gtest/gtest.h"
#include "CoreAPI.h"
#include "Const.h"
#include "Current.h"
using namespace Core;
using namespace Eigen;

class MoMTest:public testing::Test
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
		
	}

	void TearDown()override
	{
		//Release IE
		if (Core::equation)
		{
			delete equation;
			equation = nullptr;
			Console->debug("Release IE");
		}

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

TEST_F(MoMTest, EFIEImpedanceTest)
{
	
	try
	{
		SystemConfig.IEConfig.type = EFIE;
		EMCParameterUpdate(3.0e8);

		equation = IE::FIE(SystemConfig.IEConfig.type);
		ASSERT_EQ(0, Core::PreCalculateSelfTriangleImpedance()) << "Error in Pre-compute the SelfTriangle Impedance";		
		ASSERT_EQ(0, Core::InitialSolverAndImpedance()) << "Error in Initial the Impedance class";
		ASSERT_EQ(0, Core::FillImpedance()) << "Error in Fill Impedance";
		
		RWGImpOperator compute(k, W4, W7, eta);
		//��֤�迹����
		MatrixXcd& Imp = static_cast<ImpMoM*>(ComponentList::ImpService)->LocalMatrix();
		auto mesh = Mesh::GetInstance();
		auto& bf = ComponentList::BFvector;
		//Ԫ�أ�153,451���Ƚ�
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
		EXPECT_NEAR(0, norm(comp - ref) / norm(ref), 1.0e-2) << "(153,451)Ԫ�ش���";

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
		Assist::LogException(ex);
		FAIL();
	}
}

TEST_F(MoMTest, MFIEImpedanceTest)
{
	try
	{
		SystemConfig.IEConfig.type = MFIE;
		EMCParameterUpdate(3.0e8);

		equation = IE::FIE(SystemConfig.IEConfig.type);
		ASSERT_EQ(0, Core::PreCalculateSelfTriangleImpedance()) << "Error in Pre-compute the SelfTriangle Impedance";
		ASSERT_EQ(0, Core::InitialSolverAndImpedance()) << "Error in Initial the Impedance class";
		ASSERT_EQ(0, Core::FillImpedance()) << "Error in Fill Impedance";

		MatrixXcd& Imp = static_cast<ImpMoM*>(ComponentList::ImpService)->LocalMatrix();
		auto& bf = ComponentList::BFvector;
		int row[6] = { 152,229,238,111,28,45 }, col[6] = { 478,95,74,1,33,41 };

		Console->debug("Non overlap Triangle Test:");
		for (int i = 0; i < 6; ++i)
		{
			RWG* field = static_cast<RWG*>(bf[row[i]]),*s= static_cast<RWG*>(bf[col[i]]);
			Console->debug("FieldRWGID:{0:8d}\tPlusT:{1:8d}\tMinusT:{2:8d}", field->GetID(),field->TrianglePlus()->ID(), field->TriangleMinus()->ID());
			Console->debug("SourceRWGID:{0:8d}\tPlusT:{1:8d}\tMinusT:{2:8d}", s->GetID(), s->TrianglePlus()->ID(), s->TriangleMinus()->ID());
			for (int j = 0; j < 6; ++j)
			{
				RWG* source = static_cast<RWG*>(bf[col[j]]);
				
				dcomplex val1 = Imp(row[i], col[j]), val2 = Imp(col[j], row[i]);
				EXPECT_NEAR(0, norm(val1 - val2) / norm(val2), 1.0e-3) << "Error in Impedance\t" << row[i] << "," << col[i];
			}
		}

		Console->debug("Diagnol Test:");
		for (int i = 0; i < 6; ++i)
		{
			RWG* field = static_cast<RWG*>(bf[row[i]]), *s = static_cast<RWG*>(bf[col[i]]);
			Console->debug("FieldRWGID:{0:5d}\tPlusT:{1:5d}\tMinusT:{2:5d}\t({3},{4})", field->GetID(), field->TrianglePlus()->ID(), field->TriangleMinus()->ID(),Imp(row[i],row[i]).real(), Imp(row[i], row[i]).imag());
		}
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		FAIL();
	}
}

TEST_F(MoMTest,Solving)
{
	try
	{
		SystemConfig.IEConfig.type = EFIE;

		auto curInfo = Solution::CurrentInfo::GetInstance();
		curInfo->Reformat();
		curInfo->Current.push_back(
			new Solution::ElementCurrent(ComponentList::BFvector.size(), 3.0e8, "MoMTest"));
		curInfo->_numberOfConfig = curInfo->Current.size();

		ASSERT_EQ(0, Core::CalculatedMissionCentre()) << "Error in MissionCentre";
		ASSERT_EQ(0, Core::SaveResults()) << "Error in Save Results";
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		FAIL();
	}
}

#endif