///////////////////////////////////////////////////////////
//  Green.cpp
//  Implementation of the Class FreeSpaceGreenKernal
//  Created on:      20-12月-2017 16:00:46
//  Original author: ZhengMX
///////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Green.h"
#include "Const.h"

using namespace Core;
using namespace std;
using namespace Eigen;

/**
* 标量函数
*/
complex<double> FreeSpaceKernel::Scalar(const Vector3d SourcePoint, const Vector3d fieldPoint){

	const double dis = (fieldPoint - SourcePoint).norm();
	return 0.25*M_1_PI/ dis*exp(-1i*k*dis);
}


/**
* 复矢量函数
* 通常为梯度函数
*/
Vector3cd FreeSpaceKernel::Gradient(const Vector3d SourcePoint, const Vector3d fieldPoint){
	Vector3d R(fieldPoint - SourcePoint);
	const double dis = R.norm(),trans=dis*k;
	const complex<double> re = complex<double>(1, trans)*exp(-1i*trans)*0.25*M_1_PI;
	return  Vector3cd(R*(re/(dis*dis*dis)));
}


complex<double> FreeSpaceKernel::SingleSingularity(Vector3d SingularPoint){

	return  NULL;
}


complex<double> FreeSpaceKernel::HyperSingularity(Vector3d SingularPoint){

	return  NULL;
}


complex<double> FreeSpaceKernel::SuperHyperSingularity(Vector3d SingularPoint){

	return  NULL;
}
