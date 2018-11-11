#include "stdafx.h"
#include "Planewave.h"
#include "Const.h"
#include "EquationKernel.h"

using namespace Core;

void Source::Planewave::Compute()
{
	double theta = ThetaStart*M_PI_180;
	double phi = PhiStart*M_PI_180;
	
	_ki = -Vector3d{ sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta) };

	double rX = RotationX*M_PI_180;
	double rY = RotationY*M_PI_180;
	double rz = RotationZ*M_PI_180;

	const double eta = Eta*M_PI_180;
	
	_ki = -Vector3d{ sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta) };
	_ei = Rotation(theta, phi, Vector3d{ -cos(eta),sin(eta),0.0 });
	E0 = Magnitude*exp(Phase*M_PI_180);
}

Vector3d Source::Planewave::Rotation(const double theta, const double phi, const Vector3d & v)
{
	//clockwise r*=rX,rY,rZ
	/*axisX	=	1			0			0
	*			0			cos(rX)		sin(rX)
	*			0			-sin(rX)	cos(rX)
	*/
	/*axisY	=	cos(rX)		0			-sin(rX)
	*			0			1			0
	*			sin(rX)		0			cos(rX)
	*/
	/*axisZ	=	cos(rX)	    sin(rX)		0
	*			-sin(rX)	cos(rX)		0
	*			0		    0			1
	*/
	Matrix3d rot;
	rot << cos(phi)*cos(theta), -sin(phi), sin(theta)*cos(phi),
		sin(phi)*cos(theta), cos(phi), sin(theta)*sin(phi),
		-sin(theta), 0, cos(theta);
	return rot*v;
}

/**
* \brief Set Exciation(righthand)
*/
VectorXcd Source::Planewave::SetExcitation(IGreen* green,const vector<IBasicFunction*>&bfVector)const
{
	EquationKernel integral1(green, 0), integral2(green, 0), integral3(green, 0), integral4(green, 0);
	const size_t unknowns = bfVector.size();
	const size_t limit = unknowns - unknowns % 4;

	VectorXcd righthand{ unknowns };

	//Unrolling expanding
	for (size_t row = 0; row < limit; row+=4)
	{
		righthand(row) = integral1.SetRightHand(bfVector[row]);
		righthand(row+1)= integral2.SetRightHand(bfVector[row+1]);
		righthand(row+2) = integral3.SetRightHand(bfVector[row+2]);
		righthand(row + 3) = integral4.SetRightHand(bfVector[row+3]);
	}
	for (size_t row =limit ; row < unknowns; ++row)
	{
		righthand(row) = integral1.SetRightHand(bfVector[row]);
	}


	return righthand*E0;
}