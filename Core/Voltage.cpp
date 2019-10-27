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
VectorXcd Core::Source::VoltageGap::SetExcitation(const vector<IBasisFunction*>& bfVector, 
	ImpConfiguration& impconfig) const
{
	if (impconfig.ImpType == Core::Array)
	{
		size_t elementUnknowns = impconfig.ImpSize;
		VectorXcd righthand{ impconfig.NumOfElement *elementUnknowns };
		righthand.setZero();
		
		for (auto val = VoltageVector.begin(), ed = VoltageVector.end();val != ed;++val)
		{

			auto ps = static_cast<RWGTriangle*>(Mesh::GetInstance()->TriangleVector[val->PositiveSide]);
			auto ns = static_cast<RWGTriangle*>(Mesh::GetInstance()->TriangleVector[val->NegativeSide]);
			//Find RWG ID
			for (int zmc = 0;zmc < 3;zmc++)
			{
				if (ps->Rn[zmc] == ns->Rn[0] || ps->Rn[zmc] == ns->Rn[1] || ps->Rn[zmc] == ns->Rn[2])
				{
					size_t id = ps->RWGID(zmc);
					size_t arrayindex = 0;
					//Recycle Array
					for (int zmcy = 0; zmcy < impconfig.ArrayNumY; ++zmcy)
					{
						for (int zmcx = 0; zmcx < impconfig.ArrayNumX; ++zmcx)
						{
							if (impconfig.ArrayLocation(zmcx, zmcy))
							{
								dcomplex vol = val->Voltage;//Here could modify to pattern systhesis
								righthand(id+elementUnknowns* arrayindex++) = vol*(ps->Rn[zmc]->Length()*ps->RWGSign[zmc]);
							}

						}
					}
					break;
				}
			}

		}
		return righthand;
	}
	else return SetElementExcitation(bfVector);
	
}

VectorXcd Core::Source::VoltageGap::SetElementExcitation(const vector<IBasisFunction*>& bfVector) const
{
	
	VectorXcd righthand{ VectorXcd::Zero(bfVector.size()) };

	for (auto val = VoltageVector.begin(), ed = VoltageVector.end();val != ed;++val)
	{

		auto ps = static_cast<RWGTriangle*>(Mesh::GetInstance()->TriangleVector[val->PositiveSide]);
		auto ns = static_cast<RWGTriangle*>(Mesh::GetInstance()->TriangleVector[val->NegativeSide]);

		for (int zmc = 0;zmc < 3;zmc++)
		{
			if (ps->Rn[zmc] == ns->Rn[0] || ps->Rn[zmc] == ns->Rn[1] || ps->Rn[zmc] == ns->Rn[2])
			{
				size_t id = ps->RWGID(zmc);
				righthand(id) = val->Voltage*(ps->Rn[zmc]->Length()*ps->RWGSign[zmc]);
				break;
			}
		}

	}
	return righthand;
}
