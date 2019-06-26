#include "stdafx.h"
#include "Excitation.h"
#include "Const.h"
#include "IntegrationRWG.h"

using namespace Core;

//VoltageGap
Core::Source::VoltageGap::VoltageGap(string name)
{
	Name = name;
	VoltageVector.reserve(100);
}

Core::Source::VoltageGap::~VoltageGap()
{
	if (VoltageVector.size() > 0)
	{
		VoltageVector.clear();
		VoltageVector.shrink_to_fit();
	}

}
//针对RWG基函数，采用Delta函数近似Voltage,NeedMesh
VectorXcd Core::Source::VoltageGap::SetExcitation(const vector<IBasicFunction*>& bfVector) const
{
	RWGImpOperator compute(k, W4, W7, eta);
	const size_t unknowns = bfVector.size();
	VectorXcd righthand{ VectorXcd::Zero(unknowns) };
	
	for (auto val = VoltageVector.begin(), ed = VoltageVector.end();val != ed;++val)
	{

		auto ps= static_cast<RWGTriangle*>(Mesh::GetInstance()->TriangleVector[val->PositiveSide]);
		auto ns = static_cast<RWGTriangle*>(Mesh::GetInstance()->TriangleVector[val->NegativeSide]);
		
		for(int zmc=0;zmc<3;zmc++)
		{
			if (ps->Rn[zmc] == ns->Rn[0]|| ps->Rn[zmc] == ns->Rn[1]|| ps->Rn[zmc] == ns->Rn[2])
			{
				size_t id = ps->RWGID(zmc);
				righthand(id) = val->Voltage*(ps->Rn[zmc]->Length()*ps->RWGSign[zmc]);
				break;
			}
		}

	}
	return righthand;
}