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
		double k;
	};

	class IGreen
	{
	public:
		virtual ~IGreen() = default;

		virtual complex<double> SingleSingularity(Vector3d SingularPoint) =0;
		virtual complex<double> HyperSingularity(Vector3d SingularPoint) =0;
		virtual complex<double> SuperHyperSingularity(Vector3d SingularPoint) =0;
		virtual complex<double> Scalar(Vector3d pt1, Vector3d pt2) =0;
		virtual Vector3cd Gradient(Vector3d pt1, Vector3d pt2) =0;
		
		static void FactoryGreen(GreenConfiguration*config);
		static IGreen* GetInstance() { return _green; }
	protected:
		IGreen() = default;
	private:
		
		//Garbage Collection
		class GC
		{
		public:
			GC() = default;
			~GC() { if (_green)delete _green; }
		};

		static GC _gc;
		static IGreen* _green;
		
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
		Vector3cd Gradient(const Vector3d SourcePoint, const Vector3d fieldPoint) override;
	};
#pragma endregion

}


