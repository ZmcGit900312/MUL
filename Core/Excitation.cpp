#include "stdafx.h"
#include "Excitation.h"
#include "Const.h"

using namespace Core;


void Core::Source::ExcitationConfiguration::SetDirection()
{
	double theta = ThetaStart * M_PI_180;
	double phi = PhiStart * M_PI_180;

	double rX = RotationX * M_PI_180;
	double rY = RotationY * M_PI_180;
	double rz = RotationZ * M_PI_180;

	const double eta = Eta * M_PI_180;

	Ki = Vector3d{ sin(theta)*cos(phi),sin(theta)*sin(phi),-cos(theta) };
	//Original Ei=Vector3d(-sin(theta)*cos(phi),-sin(theta)*sin(phi),cos(theta))
	Ei = Vector3d(cos(theta)*cos(phi), cos(theta)*sin(phi), sin(theta));
	//_ei = Rotation(theta, phi, Vector3d{ -cos(eta),sin(eta),0.0 });
}

Vector3d Core::Source::Rotation(const double theta, const double phi, const Vector3d & v)
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
	return rot * v;
}
