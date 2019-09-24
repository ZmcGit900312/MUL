#include "stdafx.h"
#include "ImpArrayAIM.h"
#define EIGEN_MKL_USE_ALL
#include <time.h>
#include "Data.h"

ImpArrayAIM::ImpArrayAIM(const size_t unitUnknonws, int numUnitX, int numUnitY, int numUnitZ, int order,int numArrayX, int numArrayY, double threhold):_unitSize(unitUnknonws),_gridOrder(order+1),_threshold(threhold),_gridNum({ numUnitX,numUnitY,numUnitZ }), _arrayNumX(numArrayX), _arrayNumY(numArrayY)
{
}

ImpArrayAIM::~ImpArrayAIM()
{
	if(_fftTools)
	{
		delete _fftTools;
		_fftTools = nullptr;
	}
}

void ImpArrayAIM::FillImpedance()
{
	const clock_t start = clock();
	//unfinish code
	const clock_t end = clock();
	_time = double(end - start) / CLOCKS_PER_SEC;

	const auto num = _unitSize*_unitSize + _gamax.nonZeros() + _gamay.nonZeros() + _gamaz.nonZeros() + _gamad.nonZeros() + _green.size();

	_memory = double(sizeof(dcomplex) * num) / 1048576;

	Console->info("There are total {} in GamaMatrix", _gamax.nonZeros()*(_dimension + 1));
	Console->info("There are total {} in GreenMatrix", _green.size());
	Console->info("Size of Unit Element:\t{0}", _unitSize);

	RuntimeLog->info("There are total {} in GamaMatrix", _gamax.nonZeros()*(_dimension + 1));
	RuntimeLog->info("There are total {} in GreenMatrix", _green.size());
	RuntimeLog->info("Size of Unit Element:\t{0}", _unitSize);

	ResultLog->info("There are total {} in GamaMatrix", _gamax.nonZeros()*(_dimension + 1));
	ResultLog->info("There are total {} in GreenMatrix", _green.size());
	ResultLog->info("Size of Unit Element:\t{0}", _unitSize);
}
