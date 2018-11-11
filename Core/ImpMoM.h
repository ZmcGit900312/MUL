#pragma once

#include <Eigen\Core>
#include <Eigen\IterativeLinearSolvers>
#include "IImpedance.h"
#include <Eigen/src/SparseCholesky/SimplicialCholesky.h>

using namespace Eigen;
using namespace std;
using namespace Core;

class ImpMoM :public IImpService
{
	public:
	
	ImpMoM(const size_t size) :_impSize(size)
	{
		_imp.setZero();
		_memory = 16 *double( _impSize)/1024 *double(_impSize)/1024;
	}
	~ImpMoM() {}
	
	double GetTime()const override { return _time; }
	double GetMemoryCost()const override { return _memory; }

	MatrixXcd& LocalMatrix() { return _imp; }//Impedance Matrix
	VectorXcd& GetExcitation() override{ return _rightHand; }//Storage RightHand
	
	void FillImpedance() override;
	private:
	size_t _impSize;
	double _time = 0.0, _memory = 0.0;
	MatrixXcd _imp{_impSize,_impSize};
	VectorXcd _rightHand{ _impSize,1 };
};

