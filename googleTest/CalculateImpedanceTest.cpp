#include "stdafx.h"

#ifdef GTEST
#include"CoreAPI.h"
#include"Data.h"
#include <Eigen/Core>
#include "Const.h"
#include "IntegrationRWG.h"
#include <array>
#include "Log.h"
using namespace Eigen;
using namespace std;
using namespace Core;

class CalculateImpedanceTest :public testing::Test
{
public:

	static void SetUpTestCase()
	{
		if (Mesh::GetInstance()->IsLock())ASSERT_EQ(0, Core::CreatMesh()) << "Error in Creat Mesh";
		if (ComponentList::BFvector.size() < 1)ASSERT_EQ(0, Core::CreatBasicFunction(false)) << "Error in Load BasicFunction";
		if (!Core::IGreen::GetInstance())EXPECT_EQ(0, Core::SetGreenFunction());
		ASSERT_EQ(0, Core::PreCalculateSelfTriangleImpedance()) << "Error in Pre-compute the SelfTriangle Impedance";

		computeCore = new RWGImpOperator(Core::k, Core::W4, Core::W7, Core::eta);
	}

	static void TearDownTestCase()
	{
		if (computeCore)
		{
			delete computeCore;
			computeCore = nullptr;
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

		computeCore->OperatorK(field, source, Z);

		int count = 0;
		for (auto i = Z.cbegin();i != Z.cend();++i)
		{
			//int s = std::get<0>(*i), f = std::get<1>(*i);
			dcomplex value = std::get<2>(*i), ref = ZK[count++];
			EXPECT_LE(abs((value.real() - ref.real()) / ref.real()), 1.e-2) << "Ref: " << ref.real() << "value: " << value.real();
			EXPECT_LE(abs((value.imag() - ref.imag()) / ref.imag()), 1.e-2) << "Ref: " << ref.real() << "value: " << value.real();
		}
	}

	static RWGImpOperator *computeCore;
};

RWGImpOperator* CalculateImpedanceTest::computeCore = nullptr;

TEST_F(CalculateImpedanceTest, EFIESelfTest)
{
	//RWGImpOperator compute(k, W4, W7, eta);
	int triangleID[3] = { 0,119,123 };
	array<element, 6> refselfT[3] = {
		{ {
			{ 3,3, -0.40610444308869070 - 1i*2.4321203227602006 },
			{ 2,2, -0.29435679937246328 - 1.7621187089235479 * 1i },
			{ 1,1, -0.48205860851652144 - 2.8872573271382298 * 1i },
			{ 3,2, -0.37625213256250623 - 2.2524085844018567 * 1i },
			{ 3,1, -0.46655643366357696 - 2.7943876110592059 * 1i },
			{ 2,1, -0.40483188967367428 - 2.4247142672652791 * 1i },
			} } ,
			{ {
				{ 16 ,16 , -0.28055598009260246 - 1.8802373068007523 * 1i },
				{ 265,265, -0.24011903259809975 - 1.6088874255580505 * 1i },
				{ 268,268, -0.48925691331177507 - 3.2796348336879828 * 1i },
				{ 16 ,265, -0.28230635070843585 - 1.8919741073236442 * 1i },
				{ 16 ,268,  0.38668842714676666 + 2.5915023239591477 * 1i },
				{ 265,268,  0.36022571653303154 + 2.4136542231521689 * 1i }
				} },
				{ {
					{ 276,276, -0.44874634509152000 - 2.9280643356592653 * 1i },
					{ 19,19, -0.30626386126106492 - 1.9979815482381693 * 1i },
					{ 275,275, -0.26477291903144085 - 1.7269901483972330 * 1i },
					{ 276,19,  0.38857384284579027 + 2.5349363466305150 * 1i },
					{ 276,275, -0.36385554149741872 - 2.3732659991246021 * 1i },
					{ 19,275,  0.30774369042187227 + 2.0076421255612980 * 1i }
					} } };
	array<element, 9> refcoupleT[3] = {
		{ {
			{ 1, 16,  -0.066998895003645670 + 0.032369332163060212 * 1i },
			{ 2, 16,  -0.052684894036465188 + 0.025474517713309597 * 1i },
			{ 3, 16,  -0.061766906751645777 + 0.029900944282967724 * 1i },
			{ 1, 265, -0.060904595220634891 + 0.029424231732806425 * 1i },
			{ 2, 265, -0.048630666057770111 + 0.023514138765176468 * 1i },
			{ 3, 265, -0.058674740433128744 + 0.028403537881754190 * 1i },
			{ 1, 268,  0.088784957882400692 - 0.042938553751288043 * 1i },
			{ 2, 268,  0.069714191157277616 - 0.033742685911085486 * 1i },
			{ 3, 268,  0.078170738646220470 - 0.037882310047711545 * 1i }
			} } ,
			{ {
				{ 16 ,19 , -0.24466697477347241 - 0.12076007598243699 * 1i },
				{ 19 ,265, -0.22161219686545691 - 0.10863740523734622 * 1i },
				{ 19 ,268,  0.32984367587957336 + 0.16179598186801419 * 1i },
				{ 16 ,275,  0.23690203370891230 + 0.11721455022920350 * 1i },
				{ 265,275,  0.21044213088750804 + 0.10345237138612327 * 1i },
				{ 268,275, -0.29095724532057560 - 0.14312013944266930 * 1i },
				{ 16 ,276,  0.28580374242240453 + 0.14029623632293975 * 1i },
				{ 265,276,  0.28238730524949412 + 0.13771005413427864 * 1i },
				{ 268,276, -0.38242396846258797 - 0.18655321970584757 * 1i }
				} },
				{ {
					{ 1, 19, -0.083118902556617677 - 0.037503468779982348 * 1i },
					{ 2, 19, -0.065902904871389736 - 0.029659711205863214 * 1i },
					{ 3, 19, -0.081635053359256957 - 0.036879263993914203 * 1i },
					{ 1, 275, 0.079788664434739504 + 0.035850615843666132 * 1i },
					{ 2, 275, 0.064302938805268342 + 0.028823388033868969 * 1i },
					{ 3, 275, 0.070701186764250218 + 0.031810482552729738 * 1i },
					{ 1, 276, 0.103740845047302370 + 0.046797053178043196 * 1i },
					{ 2, 276, 0.080111896918630549 + 0.036048617473095033 * 1i },
					{ 3, 276, 0.093242830248270292 + 0.042122505040126094 * 1i }
					} }
	};
	cout << "\n";
	Console->debug("Self Triangle Test");
	for (int zmc = 0; zmc < 3; ++zmc)
	{
		Console->debug("Triangle ID is:\t{}", triangleID[zmc]);
		RWGImpOperator::SetSelfTriangleOperatorL(dynamic_cast<RWGTriangle*>(Mesh::GetInstance()->GetTriangle(triangleID[zmc])), W13, k, eta);
		auto resultList = computeCore->OperatorL(dynamic_cast<RWGTriangle*>(Mesh::GetInstance()->GetTriangle(triangleID[zmc])));
		for (auto i = resultList.cbegin(); i != resultList.cend(); ++i)
		{
			const int rowi = std::get<0>(*i), coli = std::get<1>(*i);
			dcomplex vali = std::get<2>(*i);
			for (auto j = refselfT[zmc].cbegin(); j != refselfT[zmc].cend(); ++j)
			{
				const int rowj = std::get<0>(*j) - 1, colj = std::get<1>(*j) - 1;
				dcomplex valj = std::get<2>(*j);
				if (rowi + coli == rowj + colj && abs(rowi - coli) == abs(rowj - colj))
				{
					EXPECT_NEAR(1, abs(valj.real() / vali.real()), 1.e-2);
					EXPECT_NEAR(1, abs(valj.imag() / vali.imag()), 1.e-2);
					break;
				}

				EXPECT_FALSE(j + 1 == refselfT[zmc].cend()) << "Error in Triangle ID:\t" <<
					triangleID[zmc] << "\nCannot find match selfImpedance\n";
			}
		}
	}

	Console->debug("Couple Triangle Test");
	for (int zmc = 0; zmc < 3; ++zmc)
	{
		const int t1 = triangleID[zmc], t2 = triangleID[(zmc + 1) % 3];
		Console->debug("Two Triangles ID are\t{0} and {1}", t1, t2);
		auto resultList = computeCore->SetImpedanceL(dynamic_cast<RWGTriangle*>(Mesh::GetInstance()->GetTriangle(t1)),
			dynamic_cast<RWGTriangle*>(Mesh::GetInstance()->GetTriangle(t2)));
		for (auto i = resultList.cbegin(); i != resultList.cend(); ++i)
		{
			const int rowi = std::get<0>(*i), coli = std::get<1>(*i);
			dcomplex vali = std::get<2>(*i);
			for (auto j = refcoupleT[zmc].cbegin(); j != refcoupleT[zmc].cend(); ++j)
			{
				const int rowj = std::get<0>(*j) - 1, colj = std::get<1>(*j) - 1;
				dcomplex valj = std::get<2>(*j);
				if (rowi + coli == rowj + colj && abs(rowi - coli) == abs(rowj - colj))
				{
					EXPECT_NEAR(1, abs(valj.real() / vali.real()), 1.e-2);
					EXPECT_NEAR(1, abs(valj.imag() / vali.imag()), 1.e-2);
					break;
				}
				EXPECT_FALSE(j + 1 == refcoupleT[zmc].cend()) << "Error in Two Triangle ID:\t" << t1 << "," << t2
					<< "\nCannot find match CoupleImpedance\n";
			}
		}
	}

}

TEST_F(CalculateImpedanceTest, EFIEUnselfTest)
{

	//RWGImpOperator compute(k, W4, W7, eta);
	auto& bf = ComponentList::BFvector;
	//RWG 填充对角部分的测试
	Console->debug("Set RWG based Impedance test");
	const dcomplex refZ1{ 0.050380879757177512, -3.1201994796628441 };//Z11
	const dcomplex compZ1 = computeCore->SetImpedanceL(static_cast<RWG*>(bf[0]), static_cast<RWG*>(bf[0]));
	EXPECT_NEAR(1, abs(compZ1.real() / refZ1.real()), 1.e-2) << "Error in real part of Z(0,0)";
	EXPECT_NEAR(1, abs(compZ1.imag() / refZ1.imag()), 1.e-2) << "Error in image part of Z(0,0)";

	const dcomplex refZ2{ 0.035819305121422902, -2.1523538356376211 };//Z16
	const dcomplex compZ2 = computeCore->SetImpedanceL(static_cast<RWG*>(bf[15]), static_cast<RWG*>(bf[15]));
	EXPECT_NEAR(1, abs(compZ2.real() / refZ2.real()), 1.e-2) << "Error in real part of Z(15,15)";
	EXPECT_NEAR(1, abs(compZ2.imag() / refZ2.imag()), 1.e-2) << "Error in image part of Z(15,15)";

	const dcomplex refZ3{ 0.040074771057483694, -3.0232018452095484 };//Z276
	const dcomplex compZ3 = computeCore->SetImpedanceL(static_cast<RWG*>(bf[275]), static_cast<RWG*>(bf[275]));
	EXPECT_NEAR(1, abs(compZ3.real() / refZ3.real()), 1.e-2) << "Error in real part of Z(275,275)";
	EXPECT_NEAR(1, abs(compZ3.imag() / refZ3.imag()), 1.e-2) << "Error in image part of Z(275,275)";

	const dcomplex refZ6 = { 0.039863516156603218, -2.5074317522116827 };
	const dcomplex compZ6 = computeCore->SetImpedanceL(static_cast<RWG*>(bf[277]), static_cast<RWG*>(bf[277]));
	EXPECT_NEAR(1, abs(compZ6.real() / refZ6.real()), 2.e-2) << "Error in real part of Z(277,277)";
	EXPECT_NEAR(1, abs(compZ6.imag() / refZ6.imag()), 2.e-2) << "Error in image part of Z(277,277)";

	const dcomplex refZ7 = { 0.053908453082725565, -2.7066454401072435 };
	const dcomplex compZ7 = computeCore->SetImpedanceL(static_cast<RWG*>(bf[14]), static_cast<RWG*>(bf[14]));
	EXPECT_NEAR(1, abs(compZ7.real() / refZ7.real()), 2.e-2) << "Error in real part of Z(14,14)";
	EXPECT_NEAR(1, abs(compZ7.imag() / refZ7.imag()), 2.e-2) << "Error in image part of Z(14,14)";
	//RWG 填充非对角元素的测试
	const dcomplex refZ4{ -0.0068251459795533580415805019470099, 0.0034167934888881430766094382533993 };
	const dcomplex compZ4 = computeCore->SetImpedanceL(static_cast<RWG*>(bf[17]), static_cast<RWG*>(bf[653]));
	EXPECT_NEAR(1, abs(compZ4.real() / refZ4.real()), 2.e-2) << "Error in real part of Z(17,653)";
	EXPECT_NEAR(1, abs(compZ4.imag() / refZ4.imag()), 3.e-2) << "Error in image part of Z(17,653)";

	const dcomplex refZ5 = { 0.0029760469268167982076489508358463, 0.0071462200783299874642939819580079 };
	const dcomplex compZ5 = computeCore->SetImpedanceL(static_cast<RWG*>(bf[37]), static_cast<RWG*>(bf[122]));
	EXPECT_NEAR(1, abs(compZ5.real() / refZ5.real()), 2.e-2) << "Error in real part of Z(37,122)";
	EXPECT_NEAR(1, abs(compZ5.imag() / refZ5.imag()), 2.e-2) << "Error in image part of Z(37,122)";

	const dcomplex refZ8 = { 0.0013340388881334160, 0.0057339624124270426 };
	const dcomplex compZ8 = computeCore->SetImpedanceL(static_cast<RWG*>(bf[598]), static_cast<RWG*>(bf[277]));
	EXPECT_NEAR(1, abs(compZ8.real() / refZ8.real()), 2.e-2) << "Error in real part of Z(598,277)";
	EXPECT_NEAR(1, abs(compZ8.imag() / refZ8.imag()), 2.e-2) << "Error in image part of Z(598,277)";

	const dcomplex refZ9 = { 0.021841512386819802310489180285913, 1.1653357138384266011854606404086 };
	const dcomplex compZ9 = computeCore->SetImpedanceL(static_cast<RWG*>(bf[95]), static_cast<RWG*>(bf[996]));
	EXPECT_NEAR(1, abs(compZ9.real() / refZ9.real()), 8.e-2) << "Error in real part of Z(95,996)";
	EXPECT_NEAR(1, abs(compZ9.imag() / refZ9.imag()), 8.e-2) << "Error in image part of Z(95,996)";

	const dcomplex refZ10 = { 0.011278380255329754910942341439295,1.1043627455689275507211277727038 };
	const dcomplex compZ10 = computeCore->SetImpedanceL(static_cast<RWG*>(bf[177]), static_cast<RWG*>(bf[182]));
	EXPECT_NEAR(1, abs(compZ10.real() / refZ10.real()), 7.e-2) << "Error in real part of Z(177,182)";
	EXPECT_NEAR(1, abs(compZ10.imag() / refZ10.imag()), 7.e-2) << "Error in image part of Z(177,182)";
}

TEST_F(CalculateImpedanceTest, MFIETest)
{
	try
	{

		//验证阻抗矩阵
		auto mesh = Mesh::GetInstance();

		//OperatorKTest
		int row[6] = { 152,229,238,111,2,45 }, col[6] = { 450,95,74,1,33,41 };
		Console->info("OperatorK Test");
		for (int i = 0; i < 6; ++i) {
			auto field = dynamic_cast<RWGTriangle*>(mesh->GetTriangle(row[i]));
			for (int j = 0; j < 6; ++j)
			{
				auto source = dynamic_cast<RWGTriangle*>(mesh->GetTriangle(col[j]));
				Console->debug("Test Couple between T{0} and T{1}", field->ID(), source->ID());
				OperatorKCompare(field, source);
			}
		}
		//Identity
		int Tri[12] = { 152,229,238,111,2,45,450,95,74,1,33,41 };
		double Ref[3][12] = {
			0.00157480639720404,-0.00258728365825092,-0.00220722312235337,-0.00190501834353101,
			0.00180267873509308,0.00245391421235691,0.00290361437918257,-0.00171148893910253,
			0.00220520613453045,0.00217637497322464,0.00232324748090940,0.00175497320424492,
	0.00208863861929583,0.00234251495563825,0.00190536771247727,0.00282706415534408,
	-0.00191409412241646,-0.00207649180821613,0.00268584213407149,0.00210188777787376,
	-0.00247527799634194,0.00264242764145906,-0.00313989098081097,0.00219347285417433,
			-0.00192672768178537,0.00164674233757378,0.00177503078158090,0.00223445773833473,
			0.00216786113650934,0.00207989476240389,-0.00262825513177719,0.00256034824240274,
			0.00213686187845252,-0.00232535768134982,0.00233987871696016,-0.00215155000866812
		};
		Console->info("OperatorIdentity Test");
		for (int i = 0; i < 12; ++i)
		{
			RWGTriangle* tp = dynamic_cast<RWGTriangle*>(mesh->GetTriangle(Tri[i]));
			auto Zself = computeCore->OperatorIdentity(tp);
			Console->debug("Test T{} self:", tp->ID());
			int count = 0;
			for (auto val = Zself.cbegin();val != Zself.cend();++val)
			{
				//auto s = std::get<0>(*val), f = std::get<1>(*val);
				const auto value = std::get<2>(*val);
				double ref =0.5* Ref[count++][i];
				++val;
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

TEST_F(CalculateImpedanceTest, DataTest)
{
	auto mesh = Mesh::GetInstance();

	//OperatorKTest
	int row[6] = { 65,102,105,49,11,18 }, col[6] = { 227,791,335,358,140,21 }, RWGID[6] = { 152,229,238,111,28,45 };
	vector<element> Z;
	for (int i = 0; i < 6; ++i) {
		auto field = dynamic_cast<RWGTriangle*>(mesh->GetTriangle(row[i]));
		auto source = dynamic_cast<RWGTriangle*>(mesh->GetTriangle(col[i]));
		Console->debug("Test Couple between T{0} and T{1}", field->ID(), source->ID());

		Z.clear();
		for (short i1 = 0; i1 < 3; i1++)
		{
			if (!field->RWGSign[i1])continue;
			for (short j1 = 0; j1 < 3; j1++)
			{
				if (!source->RWGSign[j1])continue;
				Z.push_back({ i1, j1, field->RWGSign[i1] * source->RWGSign[j1] });
			}
		}

		computeCore->OperatorK(field, source, Z);

		for (auto val = Z.cbegin();val != Z.cend();++val)
		{
			int s = std::get<0>(*val), f = std::get<1>(*val);
			dcomplex value = std::get<2>(*val);
			if (s == RWGID[i] && f == RWGID[i])Console->debug("({0},{1})",2*value.real(),2*value.imag());				
		}
	}
}
#endif