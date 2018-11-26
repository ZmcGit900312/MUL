#include "stdafx.h"

#ifdef GTEST
#include"CoreAPI.h"
#include"Data.h"
#include <Eigen/Core>
#include "Const.h"
#include "IntegrationRWG.h"
#include <array>
#include "ImpMoM.h"
#include "Log.h"
using namespace Eigen;
using namespace std;
using namespace Core;

class CalculateImpedanceTest :public testing::Test
{
public:

	static void SetUpTestCase()
	{
		ASSERT_EQ(0, Core::CreatMesh()) << "Error in Creat Mesh";
		ASSERT_EQ(0, Core::CreatBasicFunction(false)) << "Error in Load BasicFunction";
		ASSERT_EQ(0, Core::SetGreenFunction()) << "Error in set Green Function";
		ASSERT_EQ(0, Core::PreCalculateSelfTriangleImpedance()) << "Error in Pre-compute the SelfTriangle Impedance";
	}

	static void TearDownTestCase()
	{
		if (ComponentList::BFvector.size() > 0)
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
	}
};


TEST_F(CalculateImpedanceTest, SelfTriangleTest)
{
	EFRImp compute(k, W4, W7, eta);
	int triangleID[3] = { 0,119,123 };
	array<element, 6> refselfT[3] = {
		{ {
			{ 3,3, -0.40610444308869070 - 2.4321203227602006i },
			{ 2,2, -0.29435679937246328 - 1.7621187089235479i },
			{ 1,1, -0.48205860851652144 - 2.8872573271382298i },
			{ 3,2, -0.37625213256250623 - 2.2524085844018567i },
			{ 3,1, -0.46655643366357696 - 2.7943876110592059i },
			{ 2,1, -0.40483188967367428 - 2.4247142672652791i },
			} } ,
			{ {
				{ 16 ,16 , -0.28055598009260246 - 1.8802373068007523i },
				{ 265,265, -0.24011903259809975 - 1.6088874255580505i },
				{ 268,268, -0.48925691331177507 - 3.2796348336879828i },
				{ 16 ,265, -0.28230635070843585 - 1.8919741073236442i },
				{ 16 ,268,  0.38668842714676666 + 2.5915023239591477i },
				{ 265,268,  0.36022571653303154 + 2.4136542231521689i }
				} },
				{ {
					{ 276,276, -0.44874634509152000 - 2.9280643356592653i },
					{ 19,19, -0.30626386126106492 - 1.9979815482381693i },
					{ 275,275, -0.26477291903144085 - 1.7269901483972330i },
					{ 276,19,  0.38857384284579027 + 2.5349363466305150i },
					{ 276,275, -0.36385554149741872 - 2.3732659991246021i },
					{ 19,275,  0.30774369042187227 + 2.0076421255612980i }
					} } };
	array<element, 9> refcoupleT[3] = {
		{ {
			{ 1, 16,  -0.066998895003645670 + 0.032369332163060212i },
			{ 2, 16,  -0.052684894036465188 + 0.025474517713309597i },
			{ 3, 16,  -0.061766906751645777 + 0.029900944282967724i },
			{ 1, 265, -0.060904595220634891 + 0.029424231732806425i },
			{ 2, 265, -0.048630666057770111 + 0.023514138765176468i },
			{ 3, 265, -0.058674740433128744 + 0.028403537881754190i },
			{ 1, 268,  0.088784957882400692 - 0.042938553751288043i },
			{ 2, 268,  0.069714191157277616 - 0.033742685911085486i },
			{ 3, 268,  0.078170738646220470 - 0.037882310047711545i }
			} } ,
			{ {
				{ 16 ,19 , -0.24466697477347241 - 0.12076007598243699i },
				{ 19 ,265, -0.22161219686545691 - 0.10863740523734622i },
				{ 19 ,268,  0.32984367587957336 + 0.16179598186801419i },
				{ 16 ,275,  0.23690203370891230 + 0.11721455022920350i },
				{ 265,275,  0.21044213088750804 + 0.10345237138612327i },
				{ 268,275, -0.29095724532057560 - 0.14312013944266930i },
				{ 16 ,276,  0.28580374242240453 + 0.14029623632293975i },
				{ 265,276,  0.28238730524949412 + 0.13771005413427864i },
				{ 268,276, -0.38242396846258797 - 0.18655321970584757i }
				} },
				{ {
					{ 1, 19, -0.083118902556617677 - 0.037503468779982348i },
					{ 2, 19, -0.065902904871389736 - 0.029659711205863214i },
					{ 3, 19, -0.081635053359256957 - 0.036879263993914203i },
					{ 1, 275, 0.079788664434739504 + 0.035850615843666132i },
					{ 2, 275, 0.064302938805268342 + 0.028823388033868969i },
					{ 3, 275, 0.070701186764250218 + 0.031810482552729738i },
					{ 1, 276, 0.103740845047302370 + 0.046797053178043196i },
					{ 2, 276, 0.080111896918630549 + 0.036048617473095033i },
					{ 3, 276, 0.093242830248270292 + 0.042122505040126094i }
					} }
	};
	cout << "\n";
	Console->debug("Self Triangle Test");
	for (int zmc = 0; zmc < 3; ++zmc)
	{
		Console->debug("Triangle ID is:\t{}", triangleID[zmc]);
		EFRImp::SetSelfImpedanceTriangle(Mesh::GetInstance()->GetTriangle(triangleID[zmc]), W13, k, eta);
		auto resultList = compute.SetImpedance(Mesh::GetInstance()->GetTriangle(triangleID[zmc]));
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
		auto resultList = compute.SetImpedance(Mesh::GetInstance()->GetTriangle(t1),
			Mesh::GetInstance()->GetTriangle(t2));
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

TEST_F(CalculateImpedanceTest, RWGImpedanceTest)
{

	EFRImp compute(k, W4, W7, eta);
	auto& bf = ComponentList::BFvector;
	//RWG ���Խǲ��ֵĲ���
	Console->debug("Set RWG based Impedance test");
	const dcomplex refZ1(0.050380879757177512 - 3.1201994796628441i);//Z11
	const dcomplex compZ1 = compute.SetImpedance(static_cast<RWG*>(bf[0]), static_cast<RWG*>(bf[0]));
	EXPECT_NEAR(1, abs(compZ1.real() / refZ1.real()), 1.e-2) << "Error in real part of Z(0,0)";
	EXPECT_NEAR(1, abs(compZ1.imag() / refZ1.imag()), 1.e-2) << "Error in image part of Z(0,0)";

	const dcomplex refZ2(0.035819305121422902 - 2.1523538356376211i);//Z16
	const dcomplex compZ2 = compute.SetImpedance(static_cast<RWG*>(bf[15]), static_cast<RWG*>(bf[15]));
	EXPECT_NEAR(1, abs(compZ2.real() / refZ2.real()), 1.e-2) << "Error in real part of Z(15,15)";
	EXPECT_NEAR(1, abs(compZ2.imag() / refZ2.imag()), 1.e-2) << "Error in image part of Z(15,15)";

	const dcomplex refZ3(0.040074771057483694 - 3.0232018452095484i);//Z276
	const dcomplex compZ3 = compute.SetImpedance(static_cast<RWG*>(bf[275]), static_cast<RWG*>(bf[275]));
	EXPECT_NEAR(1, abs(compZ3.real() / refZ3.real()), 1.e-2) << "Error in real part of Z(275,275)";
	EXPECT_NEAR(1, abs(compZ3.imag() / refZ3.imag()), 1.e-2) << "Error in image part of Z(275,275)";

	const dcomplex refZ6 = { 0.039863516156603218 - 2.5074317522116827i };
	const dcomplex compZ6 = compute.SetImpedance(static_cast<RWG*>(bf[277]), static_cast<RWG*>(bf[277]));
	EXPECT_NEAR(1, abs(compZ6.real() / refZ6.real()), 2.e-2) << "Error in real part of Z(277,277)";
	EXPECT_NEAR(1, abs(compZ6.imag() / refZ6.imag()), 2.e-2) << "Error in image part of Z(277,277)";

	const dcomplex refZ7 = { 0.053908453082725565 - 2.7066454401072435i };
	const dcomplex compZ7 = compute.SetImpedance(static_cast<RWG*>(bf[14]), static_cast<RWG*>(bf[14]));
	EXPECT_NEAR(1, abs(compZ7.real() / refZ7.real()), 2.e-2) << "Error in real part of Z(14,14)";
	EXPECT_NEAR(1, abs(compZ7.imag() / refZ7.imag()), 2.e-2) << "Error in image part of Z(14,14)";
	//RWG ���ǶԽ�Ԫ�صĲ���
	const dcomplex refZ4{ -0.0068251459795533580415805019470099, 0.0034167934888881430766094382533993 };
	const dcomplex compZ4 = compute.SetImpedance(static_cast<RWG*>(bf[17]), static_cast<RWG*>(bf[653]));
	EXPECT_NEAR(1, abs(compZ4.real() / refZ4.real()), 2.e-2) << "Error in real part of Z(17,653)";
	EXPECT_NEAR(1, abs(compZ4.imag() / refZ4.imag()), 3.e-2) << "Error in image part of Z(17,653)";

	const dcomplex refZ5 = { 0.0029760469268167982076489508358463, 0.0071462200783299874642939819580079 };
	const dcomplex compZ5 = compute.SetImpedance(static_cast<RWG*>(bf[37]), static_cast<RWG*>(bf[122]));
	EXPECT_NEAR(1, abs(compZ5.real() / refZ5.real()), 2.e-2) << "Error in real part of Z(37,122)";
	EXPECT_NEAR(1, abs(compZ5.imag() / refZ5.imag()), 2.e-2) << "Error in image part of Z(37,122)";

	const dcomplex refZ8 = { 0.0013340388881334160 + 0.0057339624124270426i };
	const dcomplex compZ8 = compute.SetImpedance(static_cast<RWG*>(bf[598]), static_cast<RWG*>(bf[277]));
	EXPECT_NEAR(1, abs(compZ8.real() / refZ8.real()), 2.e-2) << "Error in real part of Z(598,277)";
	EXPECT_NEAR(1, abs(compZ8.imag() / refZ8.imag()), 2.e-2) << "Error in image part of Z(598,277)";

	const dcomplex refZ9 = { 0.021841512386819802310489180285913 + 1.1653357138384266011854606404086i };
	const dcomplex compZ9 = compute.SetImpedance(static_cast<RWG*>(bf[95]), static_cast<RWG*>(bf[996]));
	EXPECT_NEAR(1, abs(compZ9.real() / refZ9.real()), 8.e-2) << "Error in real part of Z(95,996)";
	EXPECT_NEAR(1, abs(compZ9.imag() / refZ9.imag()), 8.e-2) << "Error in image part of Z(95,996)";

	const dcomplex refZ10 = { 0.011278380255329754910942341439295 + 1.1043627455689275507211277727038i };
	const dcomplex compZ10 = compute.SetImpedance(static_cast<RWG*>(bf[177]), static_cast<RWG*>(bf[182]));
	EXPECT_NEAR(1, abs(compZ10.real() / refZ10.real()), 7.e-2) << "Error in real part of Z(177,182)";
	EXPECT_NEAR(1, abs(compZ10.imag() / refZ10.imag()), 7.e-2) << "Error in image part of Z(177,182)";
}
#endif