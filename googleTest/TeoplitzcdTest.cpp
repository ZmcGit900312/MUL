#include "stdafx.h"
#include "FFTmd.h"
#ifdef GTEST
#include "gtest/gtest.h"
#include "Teoplitzcd.h"
#include "TeoplitzAssist.h"
#include "Log.h"
using namespace Core;
using namespace Eigen;
using namespace Core::AIMAssist;
using namespace std;

//Initialization 
//Single Level
TEST(TeoplitzcdTest,SingleLevelInitial)
{
	VectorXi layerNum{ 1 };
	layerNum.fill(6);

	Teoplitzcd tp1;
	Teoplitzcd tp2(layerNum);
	Console->debug("Single Level Teoplitz Test");

	//tp1 Test
	Console->debug("Empty Construct Test:");
	EXPECT_EQ(0, tp1.rows()) << "\tError in tp1.rows()";
	EXPECT_EQ(0, tp1.cols()) << "\tError in tp1.cols()";
	EXPECT_EQ(0, tp1.Length()) << "\tError in tp1.Length()";
	EXPECT_EQ(0, tp1.layer()) << "\tError in tp1.layer()";

	//tp2 Test
	Console->debug("Single level of Teoplitz Test for 6 elements:");
	EXPECT_EQ(layerNum.x(), tp2.rows()) << "\tError in tp2.rows()";
	EXPECT_EQ(layerNum.x(), tp2.cols()) << "\tError in tp2.cols()";
	EXPECT_EQ(layerNum.x() * 2 - 1, tp2.Length()) << "\tError in tp2.length()";
	EXPECT_EQ(layerNum.size(), tp2.layer()) << "\tError in tp2.layer()";

	layerNum.fill(5);
	tp1.reset(layerNum);
	//Reset Test
	Console->debug("Reset empty Teoplitz to single level 5 elemets:");
	EXPECT_EQ(layerNum.x(), tp1.rows()) << "\tError in tp1.rows()";
	EXPECT_EQ(layerNum.x(), tp1.cols()) << "\tError in tp1.cols()";
	EXPECT_EQ(layerNum.x() * 2 - 1, tp1.Length()) << "\tError in tp1.Length()";
	EXPECT_EQ(layerNum.size(), tp1.layer()) << "\tError in tp1.layer()";
}

//Multilevel
TEST(TeoplitzcdTest,MultiLevelTeoplitzInitial)
{
	Console->debug("MultiLevel Teoplitz Test");
	Vector3i layerNum{2,3,2};
	Teoplitzcd tp(layerNum);
	const unsigned row = layerNum.prod();
	unsigned col = row;
	const unsigned length = (2 * layerNum.array() - 1).prod();
	EXPECT_EQ(row, tp.rows()) << "\tError in tp.rows()";
	EXPECT_EQ(col, tp.cols()) << "\tError in tp.cols()";
	EXPECT_EQ(length, tp.Length()) << "\tError in tp.Length()";
	EXPECT_EQ(layerNum.size(), tp.layer()) << "\tError in tp.layer()";
}

//Construction
TEST(TeoplitzcdTest,ConstrctFunction)
{
	Console->debug("Single level Construct Function Test");
	{
		const int L1 = 6;
		VectorXi layerNum{1};
		layerNum.fill(L1);
		VectorXcd v = VectorXcd::LinSpaced(L1, 1, 6);
		Console->debug("Case1: 6 Real elemets Test:");
		/*tp1=	1 2 3 4 5 6
		 *		2 1 2 3 4 5
		 *		3 2 1 2 3 4
		 *		4 3 2 1 2 3
		 *		5 4 3 2 1 2
		 *		6 5 4 3 2 1
		 */
		Teoplitzcd tp1(layerNum);
		tp1.construct(v);
		cout << "\tBase\t=\t[" << v.transpose().real()<< "]\n";
		EXPECT_DOUBLE_EQ(3.0,tp1.at(2,0).real()) << "Error in Get(2,0)";
		EXPECT_DOUBLE_EQ(5.0, tp1.at(1, 5).real()) << "Error in Get(1,5)";
		EXPECT_DOUBLE_EQ(2.0, tp1.at(5, 4).real()) << "Error in Get(5,4)";
		EXPECT_DOUBLE_EQ(1.0, tp1.at(4, 4).real()) << "Error in Get(4,4)";

		//complex test
		Console->debug("Case2: 5 Complex elemets Test:");
		v.resize(5);
		v << dcomplex(1, 1), 2i, -3i, dcomplex(4, 2), dcomplex(5, -1);
		layerNum.fill(5);
		tp1.reset(layerNum);
		tp1.construct(v);
		cout << "\tBase\t=\t[" << v.transpose() << "]\n";
		EXPECT_DOUBLE_EQ(abs(v(0)), abs(tp1.at(3, 3)))<<"Error in Get(3,3)";
		EXPECT_DOUBLE_EQ(abs(v(2)), abs(tp1.at(1, 3))) << "Error in Get(1,3)";
		EXPECT_DOUBLE_EQ(abs(v(1)), abs(tp1.at(2, 3))) << "Error in Get(2,3)";
		EXPECT_DOUBLE_EQ(abs(v(3)), abs(tp1.at(4, 1))) << "Error in Get(4,1)";
		EXPECT_DOUBLE_EQ(abs(v(4)), abs(tp1.at(4, 0))) << "Error in Get(4,0)";
	}
	Console->debug("Multilevel Construct Function Test");
	{//Multilevel
		Console->debug("Case3: Three Levels [2,2,2]=8 Real elemets Test:");
		const Vector3i layerNum{ 2,2,2 };
		Teoplitzcd tp(layerNum);
		const VectorXcd v = VectorXcd::LinSpaced(8, 1, 8);
		VectorXcd ref{ 27 };
		ref << 8, 7, 8, 6, 5, 6, 8, 7, 8, 4, 3, 4, 2, 1, 2, 4, 3, 4, 8, 7, 8, 6, 5, 6, 8, 7, 8;
		tp.construct(v);
		cout << "\tBase\t=\t[" << v.transpose().real() << "]\n";
		cout << "\tReference\t=\t[" << ref.transpose().real() << "]\n";
		for (unsigned i = 0; i < ref.size(); ++i)EXPECT_DOUBLE_EQ(ref(i).real(), tp.data()(i).real())<<"Erorr in Get term "<<i;
		//cout << tp.data() << endl;
	}
}

//Padding and UNpadding
TEST(TeoplitzcdTest,PaddingAndUnpadding)
{
	const Vector3i layerNum{ 2,2,2 };
	VectorXcd x(VectorXcd::LinSpaced(8, 1, 8));
	Teoplitzcd tp(layerNum);
	TeoplitzAssist computer(tp);
	const unsigned long fftnum = computer.Length();
	Console->debug("Three Levels [2,2,2] Teopltiz Matrix:");
	cout << "\tx\t=\t[" << x.transpose().real() << "]\n";
	//padding
	{
		VectorXcd xPad(VectorXcd::Zero(fftnum));
		computer.Padding(x, xPad);
		//tp.padding(x, xPad);
		for(unsigned long i=0;i<fftnum;++i)
		{
			switch (i)
			{
			case 0:EXPECT_DOUBLE_EQ(x(0).real(), xPad(i).real())<<"Error in xPad: "<<i; break;
			case 1:EXPECT_DOUBLE_EQ(x(1).real(), xPad(i).real()) << "Error in xPad: " <<i ; break;
			case 3:EXPECT_DOUBLE_EQ(x(2).real(), xPad(i).real()) << "Error in xPad: " <<i ; break;
			case 4:EXPECT_DOUBLE_EQ(x(3).real(), xPad(i).real()) << "Error in xPad: " <<i ; break;
			case 9:EXPECT_DOUBLE_EQ(x(4).real(), xPad(i).real()) << "Error in xPad: " <<i ; break;
			case 10:EXPECT_DOUBLE_EQ(x(5).real(), xPad(i).real()) << "Error in xPad: " <<i ; break;
			case 12:EXPECT_DOUBLE_EQ(x(6).real(), xPad(i).real()) << "Error in xPad: " <<i ; break;
			case 13:EXPECT_DOUBLE_EQ(x(7).real(), xPad(i).real()) << "Error in xPad: " <<i ; break;
			default:EXPECT_DOUBLE_EQ(0.0, xPad(i).real()); break;
			}
		}	
	}
	//unpadding
	{
		VectorXcd b(VectorXcd::Zero(fftnum));
		VectorXcd comp{ x.size() };
		unsigned bindex[8] = {18,19,21,22,27,28,30,31};
		Console->debug("Unpadding Test:");
		for (unsigned i = 0; i < x.size(); ++i)b(bindex[i]) = x(i);
		computer.Unpadding(b, comp);
		//tp.unpadding(b, comp);
		for (unsigned i = 0; i < x.size(); ++i)EXPECT_DOUBLE_EQ(x(i).real(), comp(i).real())<<"Error in Unpadding: "<<i;
	}
}

TEST(TeoplitzcdTest, Multiplication1)
{
	Console->debug("\nCase1: One Level Teoplitz for 6 Real elements");
	const int L = 6;
	VectorXi layerNum{ 1 };
	layerNum.fill(L);
	const VectorXcd v = VectorXcd::LinSpaced(L, 1, L);
	VectorXcd x(VectorXcd::Ones(L));
	VectorXcd ref{ L };
	Teoplitzcd tp(layerNum);
	TeoplitzAssist computer(tp);
	/*tp1=	1 2 3 4 5 6
	*		2 1 2 3 4 5
	*		3 2 1 2 3 4
	*		4 3 2 1 2 3
	*		5 4 3 2 1 2
	*		6 5 4 3 2 1
	*x=		1 1 1 1 1 1
	*ref=	21 17 15 15 17 21
	*/
	ref << 21, 17, 15, 15, 17, 21;
	tp.construct(v);

	//Construct Teoplitz FFT
	const VectorXcd tpfft{ computer.TeoplitzFFT(tp) };
	cout << "\tBase\t=\t[" <<v.transpose().real()<<"]\n";
	cout << "\tx\t=\t[" <<x.transpose().real()<< "]\n";
	//Multiplication
	computer.MultiplyTeoplitz(tpfft, x);
	cout << "\tref\t=\t[" <<ref.transpose().real()<< "]\n";
	cout << "\tresult\t=\t[" << x.transpose().real() << "]" << endl;
	for (int i = 0; i < L; ++i)EXPECT_DOUBLE_EQ(ref(i).real(), x(i).real())<<"Error in term: "<<i;
}

TEST(TeoplitzcdTest,Multiplication2)
{
	Console->debug("\nCase2: One Level Teoplitz for 5 real elements");
	const int L = 5;
	VectorXi layerNum{ 1 };
	layerNum.fill(L);
	const VectorXcd v = VectorXcd::LinSpaced(L, 1, L);
	VectorXcd x(VectorXcd::Ones(L));
	VectorXcd ref{ L };
	Teoplitzcd tp(layerNum);
	TeoplitzAssist computer(tp);
	/*tp1=	1 2 3 4 5
	*		2 1 2 3 4
	*		3 2 1 2 3
	*		4 3 2 1 2
	*		5 4 3 2 1
	*x=		1 1 1 1 1
	*ref=	15 12 11 12 15;
	*/
	ref << 15, 12, 11, 12, 15;
	tp.construct(v);

	//Construct Teoplitz FFT
	const VectorXcd tpfft{ computer.TeoplitzFFT(tp) };
	cout << "\tBase\t=\t[" << v.transpose().real() << "]\n";
	cout << "\tx\t=\t[" << x.transpose().real() << "]\n";
	//Multiplication
	computer.MultiplyTeoplitz(tpfft, x);

	cout << "\tref\t=\t[" << ref.transpose().real() << "]\n";
	cout << "\tresult\t=\t[" << x.transpose().real() << "]" << endl;
	for (int i = 0; i < L; ++i)EXPECT_DOUBLE_EQ(ref(i).real(), x(i).real()) << "Error in term: " << i;
}

TEST(TeoplitzcdTest,Mulitplication3)
{
	Console->debug("\nCase3: One Level Teoplitz 5 complex elements");
	const int L = 5;
	VectorXi layerNum{ 1 };
	layerNum.fill(L);
	VectorXcd v{ L }, x{ L }, ref{ L };
	Teoplitzcd tp(layerNum);
	TeoplitzAssist computer(tp);
	/*tp1=	1+i   2i   -3i    4+2i  5-i
	*		2i    1+1i  2i   -3i    4+2i
	*		-3i   2i    1+i   2i   -3i
	*		4+i  -3i     2i    1+i   2i
	*		5-i   4+2i  -3i    2i     1+i
	*x=		0 i 1+i 2-i 0
	*ref=	11-3i -6-3i 6i 4+3i 3+5i
	*/
	v << dcomplex(1, 1), 2i, -3i, dcomplex(4, 2), dcomplex(5, -1);
	x << 0, 1i, dcomplex(1, 1), dcomplex(2, -1), 0;
	ref << dcomplex(11, -3), dcomplex(-6, -3), 6i, dcomplex(4, 3), dcomplex(3, 5);
	tp.construct(v);

	//Construct Teoplitz FFT
	const VectorXcd tpfft{ computer.TeoplitzFFT(tp) };
	cout << "\tBase\t=\t[" << v.transpose() << "]\n";
	cout << "\tx\t=\t[" << x.transpose() << "]\n";
	//Multiplication
	computer.MultiplyTeoplitz(tpfft, x);

	cout << "\tref\t=\t[" << ref.transpose() << "]\n";
	cout << "\tresult\t=\t[" << x.transpose() << "]" << endl;
	for (unsigned i = 0; i < L; ++i)EXPECT_DOUBLE_EQ(abs(ref(i)), abs(x(i))) << "Error in term: " << i;
}

TEST(TeoplitzcdTest, Mulitplication4)
{
	Console->debug("\nCase4: Two Levels Teoplitz for [2,2]=4 real elements");
	const int L = 4;
	VectorXi layerNum{ 2 };
	layerNum.fill(2);
	const VectorXcd v = VectorXcd::LinSpaced(L, 1, L);
	VectorXcd x(v), ref{ L };
	Teoplitzcd tp(layerNum);
	TeoplitzAssist computer(tp);
	/*tp1=	1 2 3 4
	*		2 1 4 3
	*		3 4 1 2
	*		4 3 2 1
	*x=	1 2 3 4
	*ref=	30 28 22 20
	*/
	ref << 30, 28, 22, 20;
	tp.construct(v);

	//Construct Teoplitz FFT
	const VectorXcd tpfft{ computer.TeoplitzFFT(tp) };
	cout << "\tBase\t=\t[" << v.transpose().real() << "]\n";
	cout << "\tx\t=\t[" << x.transpose().real() << "]\n";
	//Multiplication
	computer.MultiplyTeoplitz(tpfft, x);

	cout << "\tref\t=\t[" << ref.transpose().real() << "]\n";
	cout << "\tresult\t=\t[" << x.transpose().real() << "]" << endl;
	for (unsigned i = 0; i < L; ++i)EXPECT_DOUBLE_EQ(ref(i).real(), x(i).real()) << "Error in term: " << i;
}

TEST(TeoplitzcdTest, MultiFFT)
{
	Console->debug("\nCase5: Three Levels Teoplitz for [2,2,2]=8 real elements");

	const int L = 8;
	const MKL_LONG dim = 3;
	MKL_LONG layerNum[dim] = { 2,2,2 };
	VectorXcd v = VectorXcd::LinSpaced(L, 0, L-1);
	VectorXcd x = VectorXcd::LinSpaced(L, 1, L);
	VectorXcd ref{ L };
	ref << 168 ,164 ,152, 148, 104, 100 ,88, 84;
	/*tp1=	0 1 2 3 4 5 6 7
			1 0 3 2 5 4 7 6;
			2 3 0 1 6 7 4 5;
			3 2 1 0 7 6 5 4;
			4 5 6 7 0 1 2 3;
			5 4 7 6 1 0 3 2;
			6 7 4 5 2 3 0 1;
			7 6 5 4 3 2 1 0];
	x=		1 2 3 4 5 6 7 8
	ref=	168 164 152 148 104 100 88 84
	*/
	//Construct Teoplitz FFT
	FFTmd computer(dim, layerNum);
	cout << "\tBase\t=\t[" << v.transpose().real() << "]\n";
	cout << "\tx\t=\t[" << x.transpose().real() << "]\n";
	computer.fwd(v);
	computer.fwd(x);
	cout << "After FFTmd:\n";
	cout << "\tBase\t=\t[" << v.transpose().real() << "]\n";
	cout << "\tx\t=\t[" << x.transpose().real() << "]\n";

	//Multiplication
	x = v.cwiseProduct(x);
	computer.inv(x);
	cout << "\tref\t=\t[" << ref.transpose().real() << "]\n";
	cout << "\tresult\t=\t[" << x.transpose().real() << "]" << endl;
	for (unsigned i = 0; i < L; ++i)EXPECT_DOUBLE_EQ(ref(i).real(), x(i).real()) << "Error in term: " << i;
}

TEST(TeoplitzcdTest, MultiFFT2)
{
	Console->debug("\nCase6: Two Levels Teoplitz for [2,3]=6 real elements");
	
	const MKL_LONG dim = 2;
	MKL_LONG layerNum[dim] = { 4,2 };//Outer is 4 inner is 2
	const int L = layerNum[0]*layerNum[1];
	VectorXcd v = VectorXcd::LinSpaced(L, 0, L - 1);
	VectorXcd x = VectorXcd::LinSpaced(L, 1, L);
	VectorXcd ref=VectorXcd::Zero(L);
	ref << 70,67,40,37,38,35,0,0;
	/*tp1=	0 1 2 3 4 5
			1 0 3 2 5 4
			2 3 0 1 2 3
			3 2 1 0 3 2
			4 5 2 3 0 1
			5 4 3 2 1 0
	x=		1 2 3 4 5 6
	ref=	70,67,40,37,38,35;
	*/
	//Construct Teoplitz FFT
	FFTmd computer(dim, layerNum);
	v[L - 1] = 3; v[L - 2] = 2;
	x[L - 1] = 0; x[L - 2] = 0;
	cout << "\tBase\t=\t[" << v.transpose().real() << "]\n";
	cout << "\tx\t=\t[" << x.transpose().real() << "]\n";
	computer.fwd(v);
	computer.fwd(x);
	cout << "After FFTmd:\n";
	cout << "\tBase\t=\t[" << v.transpose().real() << "]\n";
	cout << "\tx\t=\t[" << x.transpose().real() << "]\n";

	//Multiplication
	x = v.cwiseProduct(x);
	computer.inv(x);
	cout << "\tref\t=\t[" << ref.transpose().real() << "]\n";
	cout << "\tresult\t=\t[" << x.transpose().real() << "]" << endl;
	for (unsigned i = 0; i < 6; ++i)EXPECT_DOUBLE_EQ(ref(i).real(), x(i).real()) << "Error in term: " << i;
}

TEST(TeoplitzcdTest, MultiFFT3)
{
	Console->debug("\nCase7: Two Levels Teoplitz for [3,3]=9 real elements");

	const MKL_LONG dim = 2;
	MKL_LONG layerNum[dim] = { 4,4 };//Outer is 4 inner is 2
	const MKL_LONG nnz[2]={3,3};
	const int L = layerNum[0] * layerNum[1];
	VectorXcd v = VectorXcd::Zero(L);
	VectorXcd x = VectorXcd::Zero(L);
	VectorXcd ref = VectorXcd::Zero(L);
	ref << 240,219,228,0,141,120,129,0,132,111,120,0,0,0,0,0;
	/*tp1=	[0 1 2 3 4 5 6 7 8;
     1 0 1 4 3 4 7 6 7;
     2 1 0 5 4 3 8 7 6;
     3 4 5 0 1 2 3 4 5;
     4 3 4 1 0 1 4 3 4;
     5 4 3 2 1 0 5 4 3;
     6 7 8 3 4 5 0 1 2;
     7 6 7 4 3 4 1 0 1;
     8 7 6 5 4 3 2 1 0];
	x=		1 2 3 0 4 5 6 0 7 8 9 0 0 0 0 0
	ref=	240,219,228,0,141,120,129,0,132,111,120,0,0,0,0,0;
	*/
	//Construct Teoplitz FFT
	FFTmd computer(dim, layerNum);
	v << 0, 1, 2, 1, 3, 4, 5, 4, 6, 7, 8, 7, 3, 4, 5, 4;
	x << 1, 2, 3, 0, 4, 5, 6, 0, 7, 8, 9, 0, 0, 0, 0, 0;

	cout << "\tBase\t=\t[" << v.transpose().real() << "]\n";
	cout << "\tx\t=\t[" << x.transpose().real() << "]\n";
	computer.fwd(v);
	computer.fwd(x);
	cout << "After FFTmd:\n";
	cout << "\tBase\t=\t[" << v.transpose().real() << "]\n";
	cout << "\tx\t=\t[" << x.transpose().real() << "]\n";

	//Multiplication
	x = v.cwiseProduct(x);
	computer.inv(x);
	cout << "\tref\t=\t[" << ref.transpose().real() << "]\n";
	cout << "\tresult\t=\t[" << x.transpose().real() << "]" << endl;
}
#endif
