#ifndef IMPARRAYAIM_H
#define IMPARRAYAIM_H

#include <Eigen\Core>
#include <Eigen\Sparse>
#include "IImpedance.h"
#include "Data.h"
#include "Const.h"
#include "Multiplicator.h"

using namespace Eigen;
using namespace std;
using namespace Core;

class ImpArrayAIM;

namespace Eigen
{
	namespace internal {
		// MatrixReplacement looks-like a SparseMatrix, so let's inherits its traits:
		template<>
		struct traits<ImpArrayAIM> : traits<SparseMatrix<dcomplex>>
		{};
	}
}

class ImpArrayAIM :public EigenBase<ImpArrayAIM>,public IImpService
{
	public:
#pragma region Product
	// Required typedefs, constants, and method:
	typedef dcomplex Scalar;
	typedef double RealScalar;
	typedef int StorageIndex;

	enum
	{
		ColsAtCompileTime = Dynamic,
		MaxColsAtCompileTime = Dynamic,
		IsRowMajor = false
	};

	Index unitGamaSize() const{ return _totalNum; }
	Index unitSize()const { return _unitSize; }
	Index rows()const { return _wholeSize; }
	Index cols()const { return _wholeSize; }

	template<typename Rhs>
	Product<ImpArrayAIM, Rhs, AliasFreeProduct> operator*(const MatrixBase<Rhs>& x) const {
		return Product<ImpArrayAIM, Rhs, AliasFreeProduct>(*this, x.derived());
	}
#pragma endregion ForEigen

#pragma region Custom API
	ImpArrayAIM(ImpConfiguration*config) :
	ImpArrayAIM(config->ImpSize, config->xNumber, config->yNumber,config->zNumber, config->GridOrder, 
		config->numArrayX,config->numArrayY,config->Threshold) {
		_fillingStrategy = config->FillingStrategy;
	}
	
	ImpArrayAIM(const size_t unitUnknonws, int numUnitX, int numUnitY, int numUnitZ, int order,int numArrayX = 6, int numArrayY = 7, double threhold = 0.3);
	~ImpArrayAIM()override;

	double GetTime()const override { return _time; }
	double GetMemoryCost()const override { return _memory; }

	MatrixXcd& LocalMatrix() { return _nearMatrix; }//Impedance Matrix
	const MatrixXcd& LocalMatrix()const { return _nearMatrix;}
	VectorXcd& GetExcitation() override { return _rightHand; }//Storage RightHand

	VectorXcd FarFieldMultiplication(const VectorXcd&val)const;
	VectorXcd NearFieldMultiplication(const VectorXcd&val)const;

	//FarFiled Visit
	VectorXcd GammaXMultiplication(const VectorXcd&val)const { return _gamax * val; }
	VectorXcd GammaYMultiplication(const VectorXcd&val)const { return _gamay * val; }
	VectorXcd GammaZMultiplication(const VectorXcd&val)const { return _gamaz * val; }
	VectorXcd GammaDMultiplication(const VectorXcd&val)const { return _gamad * val; }

	VectorXcd GammaXMultiplicationT(const VectorXcd&val)const { return _gamax.transpose()*val; }
	VectorXcd GammaYMultiplicationT(const VectorXcd&val)const { return _gamay.transpose()*val; }
	VectorXcd GammaZMultiplicationT(const VectorXcd&val)const { return _gamaz.transpose()*val; }
	VectorXcd GammaDMultiplicationT(const VectorXcd&val)const { return _gamad.transpose()*val; }

	SparseMatrix<Scalar>& GetGammaX() { return _gamax; }
	SparseMatrix<Scalar>& GetGammaY() { return _gamay; }
	SparseMatrix<Scalar>& GetGammaZ() { return _gamaz; }
	SparseMatrix<Scalar>& GetGammaD() { return _gamad; }

	//Green Visit
	VectorXcd& GetGreen() { return _green; }
	const VectorXcd& CGetGreen() const { return _green; }
	void FillImpedance() override;

	void MVP(VectorXcd&res)const { _fftTools->MVP(_green, res); }
	AIMAssist::Multiplicator* _fftTools = nullptr;
#pragma endregion
	private:
	
	size_t _unitSize;
	int _gridOrder=0,_fillingStrategy=1;
	double _time = 0.0, _memory = 0.0,_threshold;
	Vector3i _gridNum;
	int _totalNum = _gridNum.prod();
	int _dimension = _gridNum[2] == 1 ? 2 : 3;
	int _cubicGridsNum = _gridNum[2] == 1 ? _gridOrder * _gridOrder : _gridOrder * _gridOrder*_gridOrder;
	int _arrayNumX = 6, _arrayNumY = 7, _wholeSize = _unitSize * _arrayNumX*_arrayNumY;
	//Gama
	SparseMatrix<Scalar>_gamax;
	SparseMatrix<Scalar>_gamay;
	SparseMatrix<Scalar>_gamaz;
	SparseMatrix<Scalar>_gamad;
	
	//Near Matrix
	MatrixXcd _nearMatrix{ _unitSize,_unitSize };

	VectorXcd _green, _greenGradX, _greenGradY, _greenGradZ;
	VectorXcd _rightHand{ _wholeSize,1 };
};


namespace Eigen
{
	namespace internal {

		// Implementation of MatrixReplacement * Eigen::DenseVector though a specialization of internal::generic_product_impl:
		template<typename Rhs>
		struct generic_product_impl<ImpArrayAIM, Rhs, SparseShape, DenseShape, GemvProduct> // GEMV stands for matrix-vector
			: generic_product_impl_base<ImpArrayAIM, Rhs, generic_product_impl<ImpArrayAIM, Rhs> >
		{
			typedef typename Product<ImpArrayAIM, Rhs>::Scalar Scalar;
			template<typename Dest>
			static void scaleAndAddTo(Dest& dst, const ImpArrayAIM& lhs, const Rhs& rhs, const Scalar& alpha)
			{
				// TODO 在此设计远场矩阵近似代码

				// This method should implement "dst += alpha * lhs * rhs" inplace,
				// however, for iterative solvers, alpha is always equal to 1, so let's not bother about it.
				//assert(alpha == Scalar(1) && "scaling is not implemented");
				// MV to be done?
				//for (Index i = 0; i<lhs.cols(); ++i)
				//	dst += rhs(i) * lhs.LocalMatrix().col(i);
				dst.noalias() += lhs.NearFieldMultiplication(rhs)+lhs.FarFieldMultiplication(rhs);			
			}
		};
	}
}

#endif