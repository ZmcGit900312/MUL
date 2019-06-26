#include "stdafx.h"

#ifdef GTEST
#include"CoreAPI.h"
#include "gtest/gtest.h"
#include"Data.h"
#include "Const.h"
#include "IEKernel.h"
using namespace Eigen;
using namespace std;
using namespace Core;

class IETest :public testing::Test
{
public:

	static void SetUpTestCase()
	{
		if (Mesh::GetInstance()->IsLock())ASSERT_EQ(0, Core::CreatMesh()) << "Error in Creat Mesh";
		if (ComponentList::BFvector.size() < 1)ASSERT_EQ(0, Core::CreatBasicFunction(false)) << "Error in Load BasicFunction";
		if (!Core::IGreen::GetInstance())EXPECT_EQ(0, Core::SetGreenFunction());
		ASSERT_EQ(0, Core::PreCalculateSelfTriangleImpedance()) << "Error in Pre-compute the SelfTriangle Impedance";

		efie = IE::FIE(EFIE);
		mfie = IE::FIE(MFIE);
		cfie = IE::FIE(CFIE);

	}

	static void TearDownTestCase()
	{
		if (efie)
		{
			delete efie;
			efie = nullptr;
		}

		if (mfie)
		{
			delete mfie;
			mfie = nullptr;
		}

		if (cfie)
		{
			delete cfie;
			cfie = nullptr;
		}
	}

protected:
	void OperatorKCompare(RWGTriangle* field, RWGTriangle* source)
	{
		//Reference
		dcomplex ZK[9] = { 0 };
		for (int i = 0; i < 4; ++i)
		{//Field Triangle
			Vector3d rhof_0(field->Quad4()[i] - field->Node(0)),
				rhof_1(field->Quad4()[i] - field->Node(1)),
				rhof_2(field->Quad4()[i] - field->Node(2));//场三角形的RHO向量
			for (int j = 0; j < 4; ++j)
			{//Source Triangled
				Vector3d rhos_0(source->Quad4()[j] - source->Node(0)),
					rhos_1(source->Quad4()[j] - source->Node(1)),
					rhos_2(source->Quad4()[j] - source->Node(2));//源三角形的RHO向量
				Vector3cd gij = W4[i] * W4[j] * IGreen::GetInstance()->Gradient(field->Quad4()[i], source->Quad4()[j]);

				ZK[0] += rhos_0.cross(rhof_0).dot(gij);
				ZK[1] += rhos_1.cross(rhof_0).dot(gij);
				ZK[2] += rhos_2.cross(rhof_0).dot(gij);
				ZK[3] += rhos_0.cross(rhof_1).dot(gij);
				ZK[4] += rhos_1.cross(rhof_1).dot(gij);
				ZK[5] += rhos_2.cross(rhof_1).dot(gij);
				ZK[6] += rhos_0.cross(rhof_2).dot(gij);
				ZK[7] += rhos_1.cross(rhof_2).dot(gij);
				ZK[8] += rhos_2.cross(rhof_2).dot(gij);
			}
		}

		//Console->info("Reference between T{0} and T{1}", field->ID(), source->ID());
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				ZK[3 * i + j] *= 0.25*field->Edge(i).second*source->Edge(j).second*field->RWGSign[i] * source->RWGSign[j];
				//Console->info("({0}, {1})= {2} + {3}i", field->RWGID(i), source->RWGID(j), ZK[3 * i + j].real(), ZK[3 * i + j].imag());
			}
		}
		//OperatorK
		vector<element> Z;
		for (short i = 0; i < 3; i++)
		{
			if (!field->RWGSign[i])continue;
			for (short j = 0; j < 3; j++)
			{
				if (!source->RWGSign[j])continue;
				Z.push_back({ i, j, field->RWGSign[i] * source->RWGSign[j] });
			}
		}

		mfie->Set(field, source, Z);
		//computeCore->OperatorK(field, source, Z);

		int count = 0;
		for (auto val : Z)
		{
			//int s = std::get<0>(val), f = std::get<1>(val);
			dcomplex value = std::get<2>(val), refe = -ZK[count++];
			EXPECT_LE(abs((value.real() - refe.real()) / refe.real()), 1.e-2) << "Ref: " << refe.real() << "value: " << value.real();
			EXPECT_LE(abs((value.imag() - refe.imag()) / refe.imag()), 1.e-2) << "Ref: " << refe.real() << "value: " << value.real();
		}
	}

	static IE* efie,*mfie,*cfie;
};

IE* IETest::efie = nullptr;
IE* IETest::mfie = nullptr;
IE* IETest::cfie = nullptr;

TEST_F(IETest, IECalculateTest)
{
	try
	{
		auto mesh = Mesh::GetInstance();
		auto& bf = ComponentList::BFvector;
		//EFIE
		{
			RWGImpOperator compute(k, W4, W7, eta);
			
			//元素（153,451）比较
			const int row = 152, col = 450;
			dcomplex pp, pm, mp, mm;
			Console->info("EFIE OperatorL Test");
			auto T228 = efie->Set(dynamic_cast<RWGTriangle*>(mesh->GetTriangle(227)));
			for (auto zmc = T228.begin(); zmc != T228.end(); ++zmc)
			{
				const int i = get<0>(*zmc), j = get<1>(*zmc);
				if (row + col == i + j) { mm = get<2>(*zmc); break; }
			}
			auto T66_227 = compute.SetImpedanceL(dynamic_cast<RWGTriangle*>(mesh->GetTriangle(65)), dynamic_cast<RWGTriangle*>(mesh->GetTriangle(226)));
			for (auto zmc = T66_227.begin(); zmc != T66_227.end(); ++zmc)
			{
				const int i = get<0>(*zmc), j = get<1>(*zmc);
				if (row + col == i + j && col - row == abs(i - j)) { pp = get<2>(*zmc); break; }
			}
			auto T66_228 = compute.SetImpedanceL(dynamic_cast<RWGTriangle*>(mesh->GetTriangle(65)), dynamic_cast<RWGTriangle*>(mesh->GetTriangle(227)));
			for (auto zmc = T66_228.begin(); zmc != T66_228.end(); ++zmc)
			{
				const int i = get<0>(*zmc), j = get<1>(*zmc);
				if (row + col == i + j && col - row == abs(i - j)) { pm = get<2>(*zmc); break; }
			}
			auto T228_227 = compute.SetImpedanceL(dynamic_cast<RWGTriangle*>(mesh->GetTriangle(227)), dynamic_cast<RWGTriangle*>(mesh->GetTriangle(226)));
			for (auto zmc = T228_227.begin(); zmc != T228_227.end(); ++zmc)
			{
				const int i = get<0>(*zmc), j = get<1>(*zmc);
				if (row + col == i + j && col - row == abs(i - j)) { mp = get<2>(*zmc); break; }
			}
			const dcomplex ref = compute.SetImpedanceL(static_cast<RWG*>(bf[row]), static_cast<RWG*>(bf[col]));
			const dcomplex comp = pp + pm + mp + mm;
			EXPECT_NEAR(0, norm(comp - ref) / norm(ref), 1.0e-2) << "(153,451)元素错误";
		}		
		
		//OperatorKTest
		int r[6] = { 152,229,238,111,2,45 }, c[6] = { 450,95,74,1,33,41 };

		Console->info("MFIE OperatorK Test");
		for (int i = 0; i < 6; ++i) {
			auto field = dynamic_cast<RWGTriangle*>(mesh->GetTriangle(r[i]));
			for (int j = 0; j < 6; ++j){
				auto source = dynamic_cast<RWGTriangle*>(mesh->GetTriangle(c[j]));
				Console->debug("Test Couple between T{0} and T{1}", field->ID(), source->ID());
				OperatorKCompare(field, source);		
			}
			
		}				

		//Identity
		int Tri[12] = { 152,229,238,111,2,45,450,95,74,1,33,41 };
		double Ref[3][12] = { 7.30173378194571e-06,-1.27005523304619e-05,-1.10387570063216e-05,-1.07514424294795e-05,9.05976989258280e-06,1.38597894777092e-05,2.05752456726542e-05,-8.74271853371352e-06,1.29003710532479e-05,1.32595371148482e-05,1.50881602388806e-05,8.98318002252817e-06,
			9.68416383872031e-06,1.14990228010353e-05,9.52911872511644e-06,1.59552361339938e-05,-9.61971313260261e-06 ,-1.17280951262029e-05,1.90320939800705e-05,1.07369745790113e-05,-1.44802810552448e-05,1.60989571265442e-05,-2.03917915075270e-05,1.12277278512948e-05,
			-8.93344897993791e-06,8.08358881193640e-06,8.87727809580084e-06,1.26107151757301e-05,1.08950766842177e-05,1.17473151251773e-05,-1.86240278373169e-05,1.30789066293079e-05,1.25005597843793e-05,-1.41672123877951e-05,1.51961705806829e-05,-1.10131374134875e-05
		};
		Console->info("MFIE OperatorIdentity Test");
		for (int i = 0; i < 12; ++i)
		{
			RWGTriangle* tp = dynamic_cast<RWGTriangle*>(mesh->GetTriangle(Tri[i]));
			auto Zself = mfie->Set(tp);
			Console->debug("Test T{} self:", tp->ID());
			int count = 0;
			for (auto val = Zself.cbegin();val != Zself.cend();++val)
			{
				//auto s = std::get<0>(*val), f = std::get<1>(*val);
				const auto value = std::get<2>(*val);
				double ref = 0.5*Ref[count++][i];
				EXPECT_LE(abs((value - ref) / ref), 1.e-2) << "Ref: " << ref << "value: " << value;
			}
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