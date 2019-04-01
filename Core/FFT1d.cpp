#include "stdafx.h"
#include "FFT1d.h"

Core::AIMAssist::FFT1d::FFT1d(const long length)
{
	if (length)ResetFFT(length);
}

int Core::AIMAssist::FFT1d::ResetFFT(const long length, const bool threadsDynamic, const int threadsNum)
{
	_length = length;
	_threadsDyanmic = threadsDynamic;
	_threads = threadsNum;


	long status = DftiCreateDescriptor(&_hand, DFTI_DOUBLE, DFTI_COMPLEX, 1, _length);
	if (status)
	{
		std::cout << DftiErrorMessage(status) << std::endl;
		return status;
	}

	status = DftiSetValue(_hand, DFTI_PLACEMENT, DFTI_INPLACE);
	if (status)
	{
		std::cout << DftiErrorMessage(status) << std::endl;
		return status;
	}

	status = DftiSetValue(_hand, DFTI_BACKWARD_SCALE, 1. / double(_length));
	if (status)
	{
		std::cout << DftiErrorMessage(status) << std::endl;
		return status;
	}

	status = DftiCommitDescriptor(_hand);
	if (status)
	{
		std::cout << DftiErrorMessage(status) << std::endl;
		return status;
	}
	MKL_Set_Dynamic(_threadsDyanmic);
	MKL_Domain_Set_Num_Threads(_threads, MKL_DOMAIN_FFT);
	printf("\n     Set Intel(R) MKL BLAS-N-Threads to %2d\n", _threads);
	MKL_Get_Dynamic() ? printf("\n     MKL_DYNAMIC : TRUE\n") : printf("\n     MKL_DYNAMIC : FAlSE\n");

	printf("     Intel(R) MKL Number of THREADS:  ALL  BLAS FFT PARDISO VML\n");
	printf("                                      %3d  %4d %3d %7d %3d\n",
		MKL_Get_Max_Threads(),
		MKL_Domain_Get_Max_Threads(MKL_DOMAIN_BLAS),
		MKL_Domain_Get_Max_Threads(MKL_DOMAIN_FFT),
		MKL_Domain_Get_Max_Threads(MKL_DOMAIN_PARDISO),
		MKL_Domain_Get_Max_Threads(MKL_DOMAIN_VML));
	return status;
}

//Core::AIMAssist::FFTmd::FFTmd(const MKL_LONG dim, MKL_LONG length[]) :_dim(dim), _length(length)
//{
//	_threads = MKL_Get_Max_Threads();
//	MKL_LONG scaltemp =1;
//	for (MKL_LONG i = 0; i < _dim; i++) { scaltemp *= _length[i]; }
//
//	long status = DftiCreateDescriptor(&_hand, DFTI_DOUBLE, DFTI_COMPLEX, _dim, _length);
//	if (status)
//	{
//		std::cout << DftiErrorMessage(status) << std::endl;
//	}
//
//	status = DftiSetValue(_hand, DFTI_PLACEMENT, DFTI_INPLACE);
//	if (status)
//	{
//		std::cout << DftiErrorMessage(status) << std::endl;
//	}
//
//	status = DftiSetValue(_hand, DFTI_BACKWARD_SCALE, 1. / double(scaltemp));
//	if (status)
//	{
//		std::cout << DftiErrorMessage(status) << std::endl;
//	}
//
//	status = DftiCommitDescriptor(_hand);
//	if (status)
//	{
//		std::cout << DftiErrorMessage(status) << std::endl;
//	}
//	MKL_Set_Dynamic(_threadsDyanmic);
//	MKL_Domain_Set_Num_Threads(_threads, MKL_DOMAIN_FFT);
//}
