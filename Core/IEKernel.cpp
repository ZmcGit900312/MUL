///////////////////////////////////////////////////////////
//  EquationKernel.cpp
//  Implementation of the Class EquationKernel
//  Created on:      20-12ÔÂ-2017 19:33:33
//  Original author: ZhengMX
///////////////////////////////////////////////////////////
#include "stdafx.h"
//#include "Const.h"
#include "IEKernel.h"
#include "Const.h"

using namespace Core;
using namespace std;

IE * Core::IE::FIE(IETYPE ty)
{
	switch (ty)
	{
	case EFIE: return new EFIEPEC(k,W4,W7,eta);
	case MFIE: return new MFIEPEC(k, W4, W7, eta);
	case CFIE: return new CFIEPEC(alpha,k, W4, W7, eta);
	case IBCEFIE: return nullptr;
	case IBCMFIE: return nullptr;
	case IBCCFIE: return nullptr;
	default: return nullptr;
	}
	
}


Core::CFIEPEC::CFIEPEC(const double alpha,const double k, double const w4[], double const w7[], const double eta):IE(k, w4, w7, eta),Eta(eta), Alpha(alpha){}

Core::CFIEPEC::~CFIEPEC()
{
}

inline vector<element> Core::CFIEPEC::Set(RWGTriangle * t)
{
	vector<element> res;
	auto opL = _computerCore.OperatorL(t), opI = _computerCore.OperatorIdentity(t);
	for(auto valL=opL.cbegin(),valI=opI.cbegin(),edL=opL.cend();valL!=edL;++valL,++valI)
	{
		int local1 = get<0>(*valL), local2 = get<1>(*valL);
		dcomplex selfL = get<2>(*valL), selfI = get<2>(*valI);
		dcomplex value = alpha * selfL + 0.5*(1 - alpha)*Eta*selfI;
		res.push_back({ local1,local2, value});
	}

	return res;
}

inline void Core::CFIEPEC::Set(RWGTriangle * field, RWGTriangle * source, vector<element>& val)
{
	auto valcp = val;
	_computerCore.OperatorL(field, source, val);
	_computerCore.OperatorK(field, source, valcp);
	for (auto valL = val.begin(), valK = valcp.begin(), edL = val.end();valL != edL;++valL, ++valK)
	{
		dcomplex& selfL = get<2>(*valL), &selfK = get<2>(*valK);
		selfL = alpha * selfL - (1 - alpha)*Eta*selfK;
	}

	
}

dcomplex Core::CFIEPEC::SetRight(RWG * source, Vector3d ki, Vector3d efield, Vector3d hfield) const
{
	dcomplex VE = _computerCore.SetIncidentFieldVector(source,ki,efield)*alpha,
	VH=Eta*(1-alpha)*_computerCore.SetIncidentFieldVector(source, ki, hfield);
	return VE+VH;
}
