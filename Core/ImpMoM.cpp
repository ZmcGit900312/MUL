#include "stdafx.h"
#include "Mesh.h"
#include "ImpMoM.h"
#include "IntegrationRWG.h"
#include "IEKernel.h"
#include "Data.h"
#include "Const.h"
using namespace std;

void ImpMoM::FillImpedance()
{
	
	//RWGImpOperator compute(k, W4, W7, eta);

	vector<element> coupleList;
	coupleList.reserve(9);

	const auto beginTriangle = Mesh::GetInstance()->TriangleVector.begin();
	const auto endTriangle = Mesh::GetInstance()->TriangleVector.cend();

	
	const clock_t start = clock();
	//Face-to-Face Set Impedance
	for (auto i =beginTriangle; i!= endTriangle; ++i)
	{
		RWGTriangle* izmc = dynamic_cast<RWGTriangle*>(*i);

		//auto selfList=compute.OperatorL(izmc);
		auto selfList = equation->Set(izmc);

		for (auto zmc = selfList.begin(); zmc!= selfList.end(); ++zmc)
		{
			const int row = get<0>(*zmc);
			const int col=get<1>(*zmc);
			const dcomplex val = get<2>(*zmc);
			_imp(row , col) += val;
			if(equation->GetType()==EFIE&&row!=col)_imp(col, row) += val;
			//if (row == col)continue;
		}

		for(auto j= beginTriangle;j!=i;++j)
		{
			RWGTriangle* jzmc = dynamic_cast<RWGTriangle*>(*j);

			coupleList.clear();
			for (short ilocal = 0; ilocal < 3; ilocal++)
			{							
				for (short jlocal = 0; jlocal < 3; jlocal++)
				{					
					coupleList.push_back({ ilocal, jlocal, izmc->RWGSign[ilocal] * jzmc->RWGSign[jlocal] });
				}
			}

			//auto coupleList =compute.SetImpedanceL(izmc, jzmc);
			equation->Set(izmc, jzmc, coupleList);
			for (auto zmc = coupleList.begin(); zmc != coupleList.end(); ++zmc)
			{
				const int row = get<0>(*zmc);
				const int col = get<1>(*zmc);
				const dcomplex val = get<2>(*zmc);
				_imp(row , col ) += val;
				_imp(col , row ) += val;
			}

			
		}
		cout << "Progress:" << setw(8) << izmc->ID()+1 << "of " << Mesh::GetInstance()->TriangleSize() << "\r";
	}
	const clock_t end = clock();
	_time = double(end - start) / CLOCKS_PER_SEC;
	//Preconditioning
	Solver->Precondition(this);
}
