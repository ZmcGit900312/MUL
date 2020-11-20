#pragma once

#include <Eigen\Core>
#include <Eigen\Sparse>
#include "IImpedance.h"
#include "Data.h"
#include "Const.h"
#include "Multiplicator.h"

using namespace Eigen;
using namespace std;
using namespace Core;
class ImpAIM;

namespace Eigen
{
	namespace internal{
		// MatrixReplacement looks-like a SparseMatrix, so let's inherits its traits:
		template<>
		struct traits<ImpAIM> :  traits<SparseMatrix<dcomplex>>
		{};
	}
}


template<typename Scalar>
class AIMIncompleteLUT :public IncompleteLUT<Scalar>
{
public:

	AIMIncompleteLUT() :IncompleteLUT() {}

	template<typename MatrixType>
	explicit AIMIncompleteLUT(const MatrixType&) {}

	template<typename MatrixType>
	AIMIncompleteLUT& compute(const MatrixType&amat) {
		analyzePattern(amat.LocalMatrix());
		factorize(amat.LocalMatrix());
		return *this;
	}

	/*template<typename Rhs>
	inline const Rhs& solve(const Rhs& b) const { return b; }*/

	ComputationInfo info() { return Success; }
};

class ImpAIM :public EigenBase<ImpAIM>, public IImpService
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


		Index rows()const { return _impSize; }
		Index cols()const { return _impSize; }

		template<typename Rhs>
		Product<ImpAIM, Rhs, AliasFreeProduct> operator*(const MatrixBase<Rhs>& x) const {
			return Product<ImpAIM, Rhs, AliasFreeProduct>(*this, x.derived());
		}
#pragma endregion ForEigen
	
#pragma region Custom API:

	ImpAIM(ImpConfiguration*config):ImpAIM(config->ImpSize,config->xNumber,config->yNumber,
		config->zNumber, config->GridOrder, config->Threshold) {_fillingStrategy = config->FillingStrategy;}
	ImpAIM(size_t unknowns, int xNum, int yNum, int zNum, int order, double threhold = 0.3);
	
	~ImpAIM()override;

	double GetTime()const override { return _time; }
	double GetMemoryCost()const override { return _memory; }
	const SparseMatrix<Scalar>& LocalMatrix()const { return _imp; }
	SparseMatrix<Scalar>& GetNearFieldMatrix() { return _imp; }
	//FarFiled Visit
	VectorXcd GammaXMultiplication(const VectorXcd&val)const { return _gamax*val; }
	VectorXcd GammaYMultiplication(const VectorXcd&val)const { return _gamay*val; }
	VectorXcd GammaZMultiplication(const VectorXcd&val)const { return _gamaz*val; }
	VectorXcd GammaDMultiplication(const VectorXcd&val)const { return _gamad*val; }
	
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

	VectorXcd& GetGreenX() { return _greenGradX; }
	VectorXcd& GetGreenY() { return _greenGradY; }
	VectorXcd& GetGreenZ() { return _greenGradZ; }


	VectorXcd& GetExcitation() override{ return _rightHand; }
	void SetExcitation(const VectorXcd& val) override { _rightHand = val; }
	void FillImpedance() override;

	void MVP(VectorXcd&res)const { _fftTools->MVP(_green, res); }
	void MVPGX(VectorXcd&res)const { _fftTools->MVP(_greenGradX, res); }
	void MVPGY(VectorXcd&res)const { _fftTools->MVP(_greenGradY, res); }
	void MVPGZ(VectorXcd&res)const { _fftTools->MVP(_greenGradZ, res); }

	AIMAssist::Multiplicator* _fftTools = nullptr;
#pragma endregion Inherent
	private:
	size_t _impSize;
	int _gridOrder;
	int _fillingStrategy=1;
	double _time = 0.0, _threshold, _memory = 0.0;
	double _sparsity=1.0;
	Vector3i _gridNum;
	int _totalNum = _gridNum.prod();
	int _dimension = _gridNum[2] == 1 ? 2 : 3;
	int _cubicGridsNum = _gridNum[2] == 1 ? _gridOrder*_gridOrder : _gridOrder*_gridOrder*_gridOrder;
	SparseMatrix<Scalar> _imp{ static_cast<intptr_t>(_impSize),static_cast<intptr_t>(_impSize) };
	SparseMatrix<Scalar>_gamax{ _totalNum, static_cast<intptr_t>(_impSize) };
	SparseMatrix<Scalar>_gamay{ _totalNum, static_cast<intptr_t>(_impSize) };
	SparseMatrix<Scalar>_gamaz{ _totalNum, static_cast<intptr_t>(_impSize) };
	SparseMatrix<Scalar>_gamad{ _totalNum, static_cast<intptr_t>(_impSize) };
	VectorXcd _green,_greenGradX,_greenGradY,_greenGradZ;
	VectorXcd _rightHand{ _impSize,1 };
};



namespace Eigen
{
	namespace internal{
		
		// Implementation of MatrixReplacement * Eigen::DenseVector though a specialization of internal::generic_product_impl:
		template<typename Rhs>
		struct generic_product_impl<ImpAIM, Rhs, SparseShape, DenseShape, GemvProduct> // GEMV stands for matrix-vector
			: generic_product_impl_base<ImpAIM, Rhs, generic_product_impl<ImpAIM, Rhs> >
		{
			typedef typename Product<ImpAIM, Rhs>::Scalar Scalar;
			template<typename Dest>
			static void scaleAndAddTo(Dest& dst, const ImpAIM& lhs, const Rhs& rhs, const Scalar& alpha)
			{
				// TODO 在此设计远场矩阵近似代码

				// This method should implement "dst += alpha * lhs * rhs" inplace,
				// however, for iterative solvers, alpha is always equal to 1, so let's not bother about it.
				//assert(alpha == Scalar(1) && "scaling is not implemented");
				// MV to be done?
				//for (Index i = 0; i<lhs.cols(); ++i)
				//	dst += rhs(i) * lhs.LocalMatrix().col(i);
				if (SystemConfig.IEConfig.type == EFIE)
				{
					VectorXcd Lx{ lhs.GammaXMultiplication(rhs) };
					VectorXcd Ly{ lhs.GammaYMultiplication(rhs) };
					VectorXcd Lz{ lhs.GammaZMultiplication(rhs) };
					VectorXcd Ld{ lhs.GammaDMultiplication(rhs) };

					/*Tools::TeoplitzMultiplicator->MultiplyTeoplitz(lhs.CGetGreen(),tempx);
					Tools::TeoplitzMultiplicator->MultiplyTeoplitz(lhs.CGetGreen(), tempy);
					Tools::TeoplitzMultiplicator->MultiplyTeoplitz(lhs.CGetGreen(), tempz);
					Tools::TeoplitzMultiplicator->MultiplyTeoplitz(lhs.CGetGreen(), tempd);*/

					lhs.MVP(Lx);
					lhs.MVP(Ly);
					lhs.MVP(Lz);
					lhs.MVP(Ld);

					Lx = lhs.GammaXMultiplicationT(Lx);
					Ly = lhs.GammaYMultiplicationT(Ly);
					Lz = lhs.GammaZMultiplicationT(Lz);
					Ld = lhs.GammaDMultiplicationT(Ld) / (k*k);
					VectorXcd L = 1i*k*eta*(Lx + Ly + Lz - Ld);
					//const clock_t start = clock();
					dst.noalias() += lhs.LocalMatrix()*rhs + L;
					//const clock_t end = clock();
					//double time = double(end - start) / CLOCKS_PER_SEC;
					//cout << time << endl;
				}				
				else
				{
					VectorXcd Lx{ lhs.GammaXMultiplication(rhs) };
					VectorXcd Ly{ lhs.GammaYMultiplication(rhs) };
					VectorXcd Lz{ lhs.GammaZMultiplication(rhs) };
					VectorXcd Ld{ lhs.GammaDMultiplication(rhs) };

					VectorXcd Lxz{ Lx }, Lxy{ Lx }, Lyx{ Ly }, Lyz{ Ly }, Lzy{ Lz }, Lzx{ Lz };
					/*Tools::TeoplitzMultiplicator->MultiplyTeoplitz(lhs.CGetGreen(),tempx);
					Tools::TeoplitzMultiplicator->MultiplyTeoplitz(lhs.CGetGreen(), tempy);
					Tools::TeoplitzMultiplicator->MultiplyTeoplitz(lhs.CGetGreen(), tempz);
					Tools::TeoplitzMultiplicator->MultiplyTeoplitz(lhs.CGetGreen(), tempd);*/

					lhs.MVP(Lx);
					lhs.MVP(Ly);
					lhs.MVP(Lz);
					lhs.MVP(Ld);

					Lx = lhs.GammaXMultiplicationT(Lx);
					Ly = lhs.GammaYMultiplicationT(Ly);
					Lz = lhs.GammaZMultiplicationT(Lz);
					Ld = lhs.GammaDMultiplicationT(Ld) / (k*k);
					VectorXcd L = 1i*k*eta*(Lx + Ly + Lz - Ld);			

					lhs.MVPGY(Lzy);
					lhs.MVPGY(Lxy);
					lhs.MVPGX(Lyx);
					lhs.MVPGX(Lzx);
					lhs.MVPGZ(Lyz);
					lhs.MVPGZ(Lxz);


					VectorXcd Kx = lhs.GammaXMultiplicationT(Lzy - Lyz);
					VectorXcd Ky = lhs.GammaYMultiplicationT(Lxz - Lzx);
					VectorXcd Kz = lhs.GammaZMultiplicationT(Lyx - Lxy);

					VectorXcd K = Kx + Ky + Kz;
					dst.noalias() += lhs.LocalMatrix()*rhs + L * SystemConfig.IEConfig.Alpha + (1 - SystemConfig.IEConfig.Alpha)*SystemConfig.IEConfig.Eta*K;
				}
				//dst.noalias() += lhs.FarMatrix().transpose()*(lhs.FarMatrix()*rhs);
				// dst+=lhs.FarMatrix.triansopse*IFFT(Green.*FFT(lhs.FarMatrix*rhs))
			}
		};
	}
}

