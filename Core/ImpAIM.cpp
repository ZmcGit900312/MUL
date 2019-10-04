#include "stdafx.h"

#define EIGEN_MKL_USE_ALL
#include <time.h>
#include "ImpAIM.h"
#include "Data.h"
#include "ConventionalMethod.h"

using namespace std;

ImpAIM::ImpAIM(const size_t unknowns,const int xNum, const int yNum,
	const int zNum, const int order, const double threhold ):
	_impSize(unknowns), _gridOrder(order), _threshold(threhold), _gridNum({xNum,yNum,zNum})
{
}

ImpAIM::~ImpAIM()
{
	if (_fftTools)
	{
		delete _fftTools;
		_fftTools = nullptr;
	}
}

void ImpAIM::FillImpedance()
{ 
	const clock_t start = clock();
	/*ConventionalMethod filler(SystemConfig.ImpConfig, ComponentList::ImpService);	
	Console->info("Mulitpole Expansion");
	filler.MultipoleExpansion(ComponentList::BFvector);
	Console->info("Construct Teoplitz Matrix");
	filler.GreenMatrixSet(IGreen::GetInstance());
	switch (_fillingStrategy)
	{
		case 0:
			filler.NearCorrection(ComponentList::BFvector);
			break;
		default:
			filler.TriangleFillingStrategy(*Mesh::GetInstance(), ComponentList::BFvector);
	}*/

	IMatrixFiller* filler = IMatrixFiller::FMatrixFiller(SystemConfig.ImpConfig, this,SystemConfig.IEConfig);
	Console->info("Mulitpole Expansion");
	filler->MultipoleExpansion(ComponentList::BFvector);
	Console->info("Generate the Green Matrix");
	filler->GreenMatrixSet(IGreen::GetInstance());
	switch (_fillingStrategy)
	{
	case 0:
		filler->NearCorrection(ComponentList::BFvector);
		break;
	default:
		filler->TriangleFillingStrategy(*Mesh::GetInstance(), ComponentList::BFvector);
	}
	delete filler;

	const clock_t end = clock();
	_time = double(end - start) / CLOCKS_PER_SEC;
	_sparsity = double(_imp.nonZeros()) / _impSize/_impSize;
	const auto num = _imp.nonZeros() + _gamax.nonZeros() + _gamay.nonZeros() + _gamaz.nonZeros() + _gamad.nonZeros() + _green.size();
	_memory = double(sizeof(dcomplex) * num) / 1048576;
	Console->info("There are total {} in GamaMatrix", _gamax.nonZeros()*(_dimension + 1));
	Console->info("There are total {} in GreenMatrix", _green.size());
	Console->info("Nonzeros of NearField Element:\t{0} in {1}", _imp.nonZeros(), _impSize*_impSize);
	Console->info("Impedance Sparsity:\t{:f}%",100*_sparsity);

	Runtime->info("There are total {} in GamaMatrix", _gamax.nonZeros()*(_dimension + 1));
	Runtime->info("There are total {} in GreenMatrix", _green.size());
	Runtime->info("Nonzeros of NearField Element:\t{0} in {1}", _imp.nonZeros(), _impSize*_impSize);
	Runtime->info("Impedance Sparsity:\t{:f}%", 100 * _sparsity);

	ResultLog->info("There are total {} in GamaMatrix", _gamax.nonZeros()*(_dimension + 1));
	ResultLog->info("There are total {} in GreenMatrix", _green.size());
	ResultLog->info("Nonzeros of NearField Element:\t{0} in {1}", _imp.nonZeros(), _impSize*_impSize);
	ResultLog->info("Impedance Sparsity:\t{:f}%", 100 * _sparsity);
}
