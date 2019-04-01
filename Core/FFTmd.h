#pragma once
#include <mkl_dfti.h>
#include <mkl.h>
#include <Eigen/Core>

using namespace Eigen;

namespace Core
{
	namespace AIMAssist
	{
		/*class FFT1d
		{
		public:
			FFT1d(long length = 0);
			~FFT1d() { if (_hand)DftiFreeDescriptor(&_hand); };
			int ResetFFT(long length, bool threadsDynamic = true, int threadsNum = MKL_Get_Max_Threads());
			int fwd(dcomplex*x) const { return DftiComputeForward(_hand, x); }
			int inv(dcomplex*x)const { return DftiComputeBackward(_hand, x); }

			void fwd(VectorXcd& x)const { fwd(x.data()); }
			void inv(VectorXcd& x)const { inv(x.data()); }
		private:
			DFTI_DESCRIPTOR_HANDLE _hand = nullptr;
			long _length = 0;
			bool _threadsDyanmic = true;
			int _threads = 1;
		};*/

		//Single and Multi-levels FFT tools
		class FFTmd
		{
		public:
			FFTmd(){}
			//Layer may be from outter to inner: z-->y-->x layer[3]={0,12,9}
			FFTmd(const MKL_LONG dim, MKL_LONG layer[]) { Reset(dim, layer); }
			~FFTmd() { if (_hand)DftiFreeDescriptor(&_hand); }

			int Reset(const MKL_LONG dim, MKL_LONG length[]);

			void SetStatus(const bool threadsDynamic, const int threadsNum);
			int fwd(dcomplex*x) const { return DftiComputeForward(_hand, x); }
			int inv(dcomplex*x)const { return DftiComputeBackward(_hand, x); }

			void fwd(VectorXcd& x)const { fwd(x.data()); }
			void inv(VectorXcd& x)const { inv(x.data()); }
			MKL_LONG Dim()const { return _dim; }
			MKL_LONG Length()const { return _length; }
		private:
			DFTI_DESCRIPTOR_HANDLE _hand = nullptr;
			MKL_LONG _dim = 1,_length=1;
			MKL_LONG *_layer=nullptr;	
			bool _threadsDyanmic = true;
			int _threads = 1;
		};
	}
}