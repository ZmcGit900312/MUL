#include "stdafx.h"
#include "Mesh.h"
#include "ImpMoM.h"
#include "EquationKernel.h"
#include "IntegrationRWG.h"
#include "Data.h"
using namespace std;

void ImpMoM::FillImpedance()
{
	
	EFRImp compute(k, W4, W7, eta);

	const auto beginTriangle = Mesh::GetInstance()->TriangleMock.begin();
	const auto endTriangle = Mesh::GetInstance()->TriangleMock.cend();

	
	const clock_t start = clock();
	//Face-to-Face Set Impedance
	for (auto i =beginTriangle; i!= endTriangle; ++i)
	{
		RWGTriangle* izmc = dynamic_cast<RWGTriangle*>(*i);
		auto selfList=compute.SetImpedance(izmc);
		for (auto zmc = selfList.begin(); zmc!= selfList.end(); ++zmc)
		{
			const int row = get<0>(*zmc);
			const int col=get<1>(*zmc);
			const dcomplex val = get<2>(*zmc);
			_imp(row , col) += val;
			if (row == col)continue;
			_imp(col , row ) += val;
		}

		for(auto j= beginTriangle;j!=i;++j)
		{
			RWGTriangle* jzmc = dynamic_cast<RWGTriangle*>(*j);
			auto coupleList =compute.SetImpedance(izmc, jzmc);
			for (auto zmc = coupleList.begin(); zmc != coupleList.end(); ++zmc)
			{
				const int row = get<0>(*zmc);
				const int col = get<1>(*zmc);
				const dcomplex val = get<2>(*zmc);
				_imp(row , col ) += val;
				_imp(col , row ) += val;
			}
		}
		cout << "Progress:" << setw(8) << izmc->ID()+1 << "of " << Mesh::GetInstance()->GetTriangle() << "\r";
	}
	const clock_t end = clock();
	_time = double(end - start) / CLOCKS_PER_SEC;
}
