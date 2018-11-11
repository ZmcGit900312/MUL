///////////////////////////////////////////////////////////
//  EquationKernel.cpp
//  Implementation of the Class EquationKernel
//  Created on:      20-12ÔÂ-2017 19:33:33
//  Original author: ZhengMX
///////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Const.h"
#include "IEKernel.h"
using namespace Core;

#pragma region EFIE

IE::return_type1 EFIEpp::operator()(const Vector3d pt)
{
	return exp(-1i*k*pt.dot(Ki))*Ei.dot(_source->CurrentPlus(pt));
}

IE::return_type2 EFIEpp::operator()(const Vector3d pt1,const Vector3d pt2)
{
	return (_source->CurrentPlus(pt2).dot(_field->CurrentPlus(pt1)) -
		_source->ChargePlus(pt2)*_field->ChargePlus(pt1) / k/k)*_green->Scalar(pt1, pt2);
}

IE::return_type1 EFIEpm::operator()(const Vector3d pt)
{
	return exp(-1i*k*pt.dot(Ki))*Ei.dot(_source->CurrentPlus(pt));
}

IE::return_type2 EFIEpm::operator()(const Vector3d pt1, const Vector3d pt2)
{
	return (_field->CurrentPlus(pt1).dot(_source->CurrentMinus(pt2)) -
		_field->ChargePlus(pt1)*_source->ChargeMinus(pt2) / k / k)*_green->Scalar(pt1, pt2);
}

IE::return_type1 EFIEmp::operator()(const Vector3d pt)
{
	return exp(-1i*k*pt.dot(Ki))*Ei.dot(_source->CurrentMinus(pt));
}

IE::return_type2 EFIEmp::operator()(const Vector3d pt1, const Vector3d pt2)
{
	return (_field->CurrentMinus(pt1).dot(_source->CurrentPlus(pt2)) -
		_field->ChargeMinus(pt1)*_source->ChargePlus(pt2) / k / k)*_green->Scalar(pt1, pt2);
}

IE::return_type1 EFIEmm::operator()(const Vector3d pt)
{
	return exp(-1i*k*pt.dot(Ki))*Ei.dot(_source->CurrentMinus(pt));
}

IE::return_type2 EFIEmm::operator()(const Vector3d pt1, const Vector3d pt2)
{
	return (_field->CurrentMinus(pt1).dot(_source->CurrentMinus(pt2)) -
		_field->ChargeMinus(pt1)*_source->ChargeMinus(pt2) / k / k)*_green->Scalar(pt1, pt2);
}

#pragma endregion EFIE

#pragma region MFIE
IE::return_type2 MFIEpp::operator()(const Vector3d pt1, const Vector3d pt2)
{
	/*return (_source->CurrentPlus(pt2).cross(_green->Gradient(pt1, pt2))).dot
	(_field->CurrentPlus(pt2).cross(_source->LimitPlus().normal()));*/
	return 1i;
}


#pragma endregion MFIE