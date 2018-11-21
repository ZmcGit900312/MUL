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

	const auto beginTriangle = ComponentList::MeshService->TriangleVector()->begin();
	const auto endTriangle = ComponentList::MeshService->TriangleVector()->cend();

	
	const clock_t start = clock();
	//Face-to-Face Set Impedance
	for (auto i =beginTriangle; i!= endTriangle; ++i)
	{
		auto selfList=compute.SetImpedance(*i);
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
			auto coupleList =compute.SetImpedance(*i, *j);
			for (auto zmc = coupleList.begin(); zmc != coupleList.end(); ++zmc)
			{
				const int row = get<0>(*zmc);
				const int col = get<1>(*zmc);
				const dcomplex val = get<2>(*zmc);
				_imp(row , col ) += val;
				_imp(col , row ) += val;
			}
		}
		cout << "Progress:" << setw(8) << i->ID()+1 << "of " << ComponentList::MeshService->GetTriangle() << "\r";
	}
	const clock_t end = clock();
	_time = double(end - start) / CLOCKS_PER_SEC;
}
