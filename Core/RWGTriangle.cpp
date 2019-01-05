#include "stdafx.h"
#include "RWG.h"

Core::RWGTriangle::~RWGTriangle()
{
	Rn[0] = nullptr;
	Rn[1] = nullptr;
	Rn[2] = nullptr;
}

dcomplex & Core::RWGTriangle::Z(const size_t i, const size_t j)
{
#ifdef TTT
	int p1 = -1, p2 = -1;
	for (int zmc = 0; zmc < 3; ++zmc)
	{
		if (Rn[zmc]->GetID() == i)p1 = zmc;
		if (Rn[zmc]->GetID() == j)p2 = zmc;
	}
	return p1 == p2 ? _selfImp[p1] : _selfImp[p1 + p2 + 2];
#else
	if (i == j)return Z(i);
	return i+j == _selfImp[3].first ? _selfImp[3].second : i+j == _selfImp[4].first ? _selfImp[4].second : _selfImp[5].second;


#endif
}

dcomplex & Core::RWGTriangle::Z(const size_t val)
{
	return val == _selfImp[0].first ? _selfImp[0].second : val == _selfImp[1].first ? _selfImp[1].second : _selfImp[2].second;
}

size_t Core::RWGTriangle::RWGID(const int val) const
{
	return Rn[val]?Rn[val]->GetID():-1;
}