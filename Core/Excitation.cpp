#include "stdafx.h"
#include "Excitation.h"
#include "Const.h"
#include "IntegrationRWG.h"

using namespace Core;

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



