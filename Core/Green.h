///////////////////////////////////////////////////////////
//  Green.h
//  Implementation of the Class FreeSpaceGreenKernal
//  Created on:      20-12月-2017 16:00:46
//  Original author: ZhengMX
///////////////////////////////////////////////////////////
#pragma once
#include <Eigen\Core>
#include <complex>

using namespace Eigen;
using namespace std;

namespace Core
{
	struct GreenConfiguration
	{
		int GreenType;
	};

	__interface IGreen
	{
		
		virtual complex<double> SingleSingularity(Vector3d SingularPoint) =0;
		virtual complex<double> HyperSingularity(Vector3d SingularPoint) =0;
		virtual complex<double> SuperHyperSingularity(Vector3d SingularPoint) =0;
		virtual complex<double> Scalar(Vector3d pt1, Vector3d pt2) =0;
		virtual Vector3cd Gradient(Vector3d pt1, Vector3d pt2) =0;

	};

#pragma region FreeSpace
	class FreeSpaceKernel:public IGreen
	{
	public:
		FreeSpaceKernel(){}
		~FreeSpaceKernel(){}
		complex<double> SingleSingularity(Vector3d SingularPoint) override;//未完成
		complex<double> HyperSingularity(Vector3d SingularPoint) override;//未完成
		complex<double> SuperHyperSingularity(Vector3d SingularPoint) override;//未完成
		complex<double> Scalar(Vector3d pt1, Vector3d pt2) override;
		Vector3cd Gradient(Vector3d pt1, Vector3d pt2) override;
	};
#pragma endregion

}


