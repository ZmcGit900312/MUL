
#ifndef CONTEXT_H
#define CONTEXT_H
#include <vector>

using namespace std;
namespace Core
{
	enum Card
	{
		BC = 0, BL, BP, BQ, BT, CB = 5, CL, CN, DD, DK,
		DP = 10, DZ, EG, EL, FA, AM = 15, FO, FP, HC, HE,
		HP = 20, HY, IM, IP, KA, KK = 25, KL, KR, KU, LA,
		MB = 30, ME, NC, NU, PB, PE = 35, PH, PM, PO, PY,
		QT = 40, QU, RM, SF, SY, TG = 45, TO, TP, UT, UZ,
		VS = 50, WA, WG, WR, ZY,

		A0 = 60, A1, A2, A3, A4, A5 = 65, A6, A7, AB, AC,
		AE = 70, AF, AI, AK, AN, AP = 75, AR, AS, AT, AV,
		AW = 80, BO, CA, CD, CF, CG = 85, CM, CO, CS, CI,
		DA = 90, DI, DL, EE, EN, FD = 95, FE, FF, FR, GF,
		KC = 100, KS, L2, L4, LC, LD = 105, LE, LF, LN, LP,
		LS = 110, LT, LZ, NW, OF, OM = 115, OS, PP, PR, PS,
		PW = 120, RA, SA, SH, SK, SP = 125, TL, TR, WD
	};

	__interface ICard
	{
		Card TYPE()const = 0;
	};

	//Configuration of IM
	struct C_IM :ICard
	{
		unsigned FileType = 3;//Nas=3
		unsigned short Patch = 0;
		std::string MeshFile ;
		Card TYPE()const override { return IM; }
	};

	//Configuration of FR
	struct C_FR :ICard
	{
		Card TYPE()const override { return FR; }
		double Frequency = 3.0e8;
	};

	struct C_CG :ICard
	{
		Card TYPE()const override { return CG; }
		unsigned SolutionType = 10;//BiCG-Stab 
		unsigned PreConditionType = 8;//Jacobi
		unsigned Maxiteration = 0;
		double Tolerance = -1;
		double MaxStopTolerance = 10;
	};
	//FarField
	struct C_FF :ICard
	{
		Card TYPE()const override { return FF; }
		unsigned FieldType = 0;//RCS
		unsigned ThetaNum = 1, PhiNum = 1;
		double ThetaStart = 0, PhiStart = 0;
		double ThetaIncrement = 1, PhiIncrement = 1;
		string FarFileName;
	};
	//Plane wave
	struct C_A0 :ICard
	{
		Card TYPE()const override { return A0; }
		//Direction
		unsigned ThetaNum = 1, PhiNum = 1;
		double ThetaStart = 0, PhiStart = 0;
		double ThetaIncrement = 1, PhiIncrement = 1;
		//E polarisation
		int Polarisation = 0;//-1:left,0:linear,1:right
		double Magnitude, Phase, Eta;
		double RotationX, RotationY, RotationZ;
		//FileName
		string ExcitationName ;
	};
	//AIM
	struct C_AM :ICard
	{
		Card TYPE()const override { return AM; }
		int Order;
		double Interval = 0.05;
		double Threshold = 0.3;
		double NearCorrectionEps = 1.0e-3;
		unsigned Dimension = 3;
	};

}

#endif