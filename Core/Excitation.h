#pragma once
#include <Eigen/Core>

using namespace std;
using namespace Eigen;

namespace Core
{
	namespace Source
	{
		struct ExcitationConfiguration
		{
			int sourceType = 0;
			//Direction
			unsigned ThetaNum = 1, PhiNum = 1;
			double ThetaStart = 0, PhiStart = 0;
			double ThetaIncrement = 1, PhiIncrement = 1;
			//E polarisation
			int Polarisation = 0;//-1:left,0:linear,1:right
			double Magnitude = 1.0;
			double Phase = 0.0;
			double Eta = 0.0;
			double RotationX = 0.0, RotationY = 0.0, RotationZ = 0.0;
			//FileName
			std::string ExcitationName;
			Vector3d Ki{ 0,0,-1 }, Ei{ 1,0,0 };
			
			//Operation
			void SetDirection();
			
		};
		static Vector3d Rotation(const double theta, const double phi, const Vector3d & v);
	}
}


