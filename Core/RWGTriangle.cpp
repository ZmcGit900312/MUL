#include "stdafx.h"
#include "RWG.h"

Core::RWGTriangle::~RWGTriangle()
{
	Rn[0] = nullptr;
	Rn[1] = nullptr;
	Rn[2] = nullptr;
}

dcomplex  Core::RWGTriangle::Z(const size_t i, const size_t j)const
{
	if (i == j)return Z(i);
	return i+j == _selfImp[3].first ? _selfImp[3].second : i+j == _selfImp[4].first ? _selfImp[4].second : _selfImp[5].second;
}

dcomplex  Core::RWGTriangle::Z(const size_t val)const
{
	return val == _selfImp[0].first ? _selfImp[0].second : val == _selfImp[1].first ? _selfImp[1].second : _selfImp[2].second;
}

void Core::RWGTriangle::SetZ(const dcomplex val, const short index)
{
	_selfImp[index] = Rn[index] ? pair<size_t,dcomplex>(Rn[index]->GetID(),val): pair<size_t, dcomplex>(-1,0);
}

void Core::RWGTriangle::SetZ(const dcomplex val, const short i, const short j)
{
	_selfImp[i+j+2] = Rn[i]&&Rn[j] ? pair<size_t, dcomplex>(Rn[i]->GetID()+ Rn[j]->GetID(), val) : pair<size_t, dcomplex>(-1, 0);
}

size_t Core::RWGTriangle::RWGID(const int val) const
{
	return Rn[val]?Rn[val]->GetID():-1;
}