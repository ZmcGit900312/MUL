//Planewave.h

#ifndef PLANEWAVE_H
#define PLANEWAVE_H
#include <string>
#include <Eigen/Core>
#include "IBasicFunction.h"

namespace Core {
	__interface IGreen;
}

using namespace Eigen;
namespace Core
{
	namespace Source
	{
		class Planewave
		{
		public:
			Planewave(){};
			~Planewave(){};
			//Direction
			unsigned ThetaNum = 1, PhiNum = 1;
			double ThetaStart = 0, PhiStart = 0;
			double ThetaIncrement = 1, PhiIncrement = 1;
			//E polarisation
			int Polarisation = 0;//-1:left,0:linear,1:right
			double Magnitude = 1.0;
			double Phase = 0.0;
			double Eta=0.0;
			double RotationX=0.0, RotationY=0.0, RotationZ=0.0;

			dcomplex E0;
			//FileName
			std::string ExcitationName;

			/**
			 * \brief Compute the k direction and E direction
			 */
			void Compute();
			Vector3d GetKi()const { return _ki; }
			Vector3d GetEi()const {return _ei; }

			/**
			 * \brief Set Exciation(righthand)
			 * \param bfVector 
			 * \return Excitation
			 */
			VectorXcd SetExcitation(const vector<IBasicFunction*>&bfVector)const;
		private:
			/**
			 * \brief Rotate the vector with axis Y by theta firstly and with axix Z by phi
			 * \param theta Clockwise rotate by axis Y (rad)
			 * \param phi Clockwise rotate by axis Z (rad)
			 * \param v The vector should be rotated
			 * \return The rotated vector
			 */
			static Vector3d Rotation(const double theta, const double phi, const Vector3d& v);
			Vector3d _ki;
			Vector3d _ei;
		};
	}
}

#endif
