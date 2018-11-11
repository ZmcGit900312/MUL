//Radiation kernel
#include"stdafx.h"
#include"Const.h"
#include "Radiation.h"


RadiationKernel::return_type1 RadiationKernelp::operator()(const Vector3d pt)
{
	//Vector3cd A = 1i*Omega*Mu*_green->Scalar(pt, _observation)*_source->CurrentPlus(pt);
	//Vector3cd PHI =- 1i / (Omega*Epsilon)*_source->CurrentPlus(pt)*_green->Gradient(pt, _observation);
	return -1i*Omega*Mu*_green->Scalar(pt, _observation)*_source->CurrentPlus(pt) +
		1i / (Omega*Epsilon)*_source->ChargePlus(pt)*_green->Gradient(pt, _observation);
}

RadiationKernel::return_type1 RadiationKernelm::operator()(const Vector3d pt)
{
	return -1i*Omega*Mu*_green->Scalar(pt, _observation)*_source->CurrentMinus(pt) +
		1i / (Omega*Epsilon)*_source->ChargeMinus(pt)*_green->Gradient(pt, _observation);
}

RadiationTest::return_type1 RadiationTest::operator()(IBasicFunction * bf, const bool pm, const Vector3d ob, const Vector3d pt) const
{
	if(pm)//plus
	{
		return -1i*Omega*Mu*_green->Scalar(pt, ob)*bf->CurrentPlus(pt) +
			1i / (Omega*Epsilon)*bf->ChargePlus(pt)*_green->Gradient(pt, ob);
	}
	//minus
	{
		return -1i*Omega*Mu*_green->Scalar(pt, ob)*bf->CurrentMinus(pt) +
			1i / (Omega*Epsilon)*bf->ChargeMinus(pt)*_green->Gradient(pt, ob);
	}
}
