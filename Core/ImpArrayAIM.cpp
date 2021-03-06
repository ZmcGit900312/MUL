#include "stdafx.h"
#include "ImpArrayAIM.h"
#define EIGEN_MKL_USE_ALL
#include <time.h>
#include "Data.h"
#include "AIMArray.h"

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

void ImpArrayAIM::SetExcitation(const VectorXcd & val)
{
	if(diagflag)_rightHand = PrecondDiagnoalMultiplication(val);
	else _rightHand = val;
}

VectorXcd ImpArrayAIM::FarFieldMultiplication(const VectorXcd & val) const
{
	VectorXcd L;
	if (SystemConfig.IEConfig.type == EFIE)
	{
		//并行尝试，需要修改的部分
		VectorXcd Lx{ _gamax*val };
		VectorXcd Ly{ _gamay*val };
		VectorXcd Lz{ _gamaz*val };
		VectorXcd Ld{ _gamad*val };

		_fftTools->MVP(_green, Lx);
		_fftTools->MVP(_green, Ly);
		_fftTools->MVP(_green, Lz);
		_fftTools->MVP(_green, Ld);
		
		Lx = _gamax.transpose()*Lx;
		Ly = _gamay.transpose()*Ly;
		Lz = _gamaz.transpose()*Lz;
		Ld = _gamad.transpose()*Ld / (k*k);
		
		
		L=1i*k*eta*(Lx + Ly + Lz - Ld);
	}

	return L;
}

VectorXcd ImpArrayAIM::NearFieldMultiplication(const VectorXcd & val) const
{
	VectorXcd Near{ _wholeSize };
	for (Index i = 0;i < _wholeSize / _unitSize;++i)
	{
		Near.segment(_unitSize*i, _unitSize) = _nearMatrix * val.segment(_unitSize*i, _unitSize);
	}
	return Near;
}

VectorXcd ImpArrayAIM::PrecondDiagnoalMultiplication(const VectorXcd & val) const
{
	VectorXcd diag{ _wholeSize };
	for (Index i = 0;i < _wholeSize / _unitSize;++i)
	{
		diag.segment(_unitSize*i, _unitSize) = _nearMatrix*val.segment(_unitSize*i, _unitSize);
	}
	return diag;
}

void ImpArrayAIM::FillImpedance()
{
	const clock_t start = clock();
	//unfinish code
	AIMArray* fillingTool = new AIMArray(SystemConfig.ImpConfig, ComponentList::ImpService, SystemConfig.IEConfig);
	auto& bf = ComponentList::BFvector;
	fillingTool->MultipoleExpansion(bf);
	fillingTool->GreenMatrixSet(IGreen::GetInstance());

	fillingTool->TriangleFillingStrategy(*Mesh::GetInstance(), ComponentList::BFvector);

	const clock_t end = clock();
	_time = double(end - start) / CLOCKS_PER_SEC;

	const auto numGamma = (_dimension + 1)*_gamax.nonZeros() / (_wholeSize / _unitSize);
	const auto num = _unitSize*_unitSize   + _green.size();

	_memory = double(sizeof(dcomplex) * num+ sizeof(double) * numGamma) / 1048576;

	delete fillingTool;
	Console->info("There are total {} in GamaMatrix", numGamma);
	Console->info("There are total {} in GreenMatrix", _green.size());
	Console->info("Size of Unit Element:\t{0}", _unitSize);

	ResultLog->info("There are total {} in GamaMatrix", numGamma);
	ResultLog->info("There are total {} in GreenMatrix", _green.size());
	ResultLog->info("Size of Unit Element:\t{0}", _unitSize);
}
