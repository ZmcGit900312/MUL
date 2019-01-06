#ifndef CONST_H
#define CONST_H

#define _USE_MATH_DEFINES
#include <Eigen\Core>
//#include <math.h>
//#define M_PI_180 0.017453292519943
using namespace Eigen;
namespace Core
{
	double const M_PI = 3.1415926535897932384626;
	double const M_1_PI = 1.0 / M_PI;
	double const M_PI_180 = M_PI / 180.0;
	double const Mu0 = M_PI*4e-7;
	double const Epsilon0 = 1e-9*M_1_PI / 36;
	double const eta0 = 120 * M_PI;
	double const c0 = 1/sqrt(Mu0*Epsilon0);

	//static Vector3d Ki{ 0,0,-1 };//incident direction
	//static Vector3d Ei{ 0,1,0 };//polarization direction
	extern double Mur;
	extern double Epr;
	extern double Mu;
	extern double Epsilon;
	extern double Frequency;
	extern double Omega;
	extern double k;
	extern double Lambda;
	extern double eta;
	
	
	//Quad
#pragma region Quad
	const double Quad4[3][4] = { 0.333333333333333,0.6,0.2,0.2,
		0.333333333333333,0.2,0.6,0.2,0.333333333333333,0.2,0.2,0.6};
	double const W4[4] = { -0.5625,0.520833333333333,0.520833333333333,0.520833333333333 };

	const double Quad7[3][7] = { 0.333333333333333,0.059715871789770,0.470142064105115,0.470142064105115,0.797426985353087,0.101286507323456,0.101286507323456,
		0.333333333333333,0.470142064105115,0.059715871789770,0.470142064105115,0.101286507323456,0.797426985353087,0.101286507323456,
		0.333333333333333,0.470142064105115,0.470142064105115,0.059715871789770,0.101286507323456,0.101286507323456,0.797426985353087,};
	const double W7[7] = { 0.225, 0.132394152788506, 0.132394152788506, 0.132394152788506, 0.125939180544827, 0.125939180544827, 0.125939180544827 };

	const double Quad13[3][13] = { 0.333333333333333,0.479308067841923,0.260345966079038,0.260345966079038,0.869739794195568,0.065130102902216,0.065130102902216,
			0.638444188569809,0.048690315425315,0.312865496004875,0.638444188569809,0.048690315425315,0.312865496004875,
		0.333333333333333,0.260345966079038,0.479308067841923,0.260345966079038,0.065130102902216,0.869739794195568,0.065130102902216,
			0.312865496004875,0.638444188569809,0.048690315425315,0.048690315425315,0.312865496004875,0.638444188569809,
		0.333333333333333,0.260345966079038,0.260345966079038,0.479308067841923,0.065130102902216,0.065130102902216,0.869739794195568,
			0.048690315425315,0.312865496004875,0.638444188569809,0.312865496004875,0.638444188569809,0.048690315425315};
	const double W13[13] = { -0.149570044467670, 0.175615257433204,0.175615257433204,0.175615257433204,0.053347235608839,0.053347235608839, 0.053347235608839, 
		0.077113760890257,0.077113760890257,0.077113760890257,0.077113760890257, 0.077113760890257,0.077113760890257 };

#pragma endregion 
}

#endif