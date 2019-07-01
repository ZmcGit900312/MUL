///////////////////////////////////////////////////////////
//  EquationKernel.cpp
//  Implementation of the Class EquationKernel
//  Created on:      20-12ÔÂ-2017 19:33:33
//  Original author: ZhengMX
///////////////////////////////////////////////////////////
#include "stdafx.h"
#include "IEKernel.h"
#include "Data.h"
#include "Const.h"

using namespace Core;
using namespace std;

IE * Core::IE::FIE(IETYPE ty)
{
	switch (ty)
	{
	case EFIE: return new EFIEPEC(k,W4,W7, SystemConfig.IEConfig.Eta);
	case MFIE: return new MFIEPEC(k, W4, W7, SystemConfig.IEConfig.Eta);
	case CFIE: return new CFIEPEC(SystemConfig.IEConfig.Alpha,k, W4, W7, SystemConfig.IEConfig.Eta);
	case IBCEFIE: return nullptr;
	case IBCMFIE: return nullptr;
	case IBCCFIE: return nullptr;
	default: throw exception("error equation");
	}
	
}


Core::CFIEPEC::CFIEPEC(const double alpha,const double k, double const w4[], double const w7[], const double eta):IE(k, w4, w7, eta),Eta(eta), Alpha(alpha){}

Core::CFIEPEC::~CFIEPEC()
{
}

inline vector<element> Core::CFIEPEC::Set(RWGTriangle * t)
{
	return _computerCore.OperatorCPEC(t, Alpha, Eta);
	vector<element> res;
	auto opL = _computerCore.OperatorL(t), opI = _computerCore.OperatorIdentity(t);
	auto valL = opL.cbegin(), valI = opI.cbegin();

	for (int i = 0; i < 3; ++i)
	{
		int local1 = get<0>(*valL), local2 = get<1>(*valL);
		dcomplex value = Alpha * get<2>(*valL++);
		res.push_back({ local1,local2, value });
	}

	for (int i = 0; i < 3; ++i)
	{
		int local1 = get<0>(*valI), local2 = get<1>(*valI);
		dcomplex selfL = get<2>(*valL++), selfI = get<2>(*valI++);
		dcomplex value = Alpha * selfL + (1 - Alpha)*Eta*selfI;
		res.push_back({ local1,local2, value });
		local1 = get<0>(*valI), local2 = get<1>(*valI);
		selfI = get<2>(*valI++);
		value = Alpha * selfL + (1 - Alpha)*Eta*selfI;
		res.push_back({ local1,local2, value });
	}	

	return res;
}

inline void Core::CFIEPEC::Set(RWGTriangle * field, RWGTriangle * source, vector<element>& val)
{
	return _computerCore.OperatorCPEC(field, source, val, Alpha, Eta);

	auto valcp = val;
	_computerCore.OperatorL(field, source, val);
	_computerCore.OperatorK(field, source, valcp);
	for (auto valL = val.begin(), valK = valcp.begin(), edL = val.end();valL != edL;++valL, ++valK)
	{
		dcomplex& selfL = get<2>(*valL), &selfK = get<2>(*valK);
		selfL = Alpha * selfL - (1 - Alpha)*Eta*selfK;
	}

	
}

dcomplex Core::CFIEPEC::SetRight(RWG * source, Vector3d ki, Vector3cd efield, Vector3cd hfield) const
{
	return _computerCore.SetIncidentFieldVector(source, ki, efield, Alpha, Eta);
	dcomplex VE = _computerCore.SetIncidentFieldVector(source,ki,efield)*Alpha,
	VH=Eta*(1- Alpha)*_computerCore.SetIncidentFieldVector(source, ki, hfield);
	return VE+VH;
}
