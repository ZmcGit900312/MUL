//Planewave.h

#ifndef PLANEWAVE_H
#define PLANEWAVE_H
#include <Eigen/Core>
#include "IBasicFunction.h"
#include "Excitation.h"
#include <vector>


using namespace Eigen;
namespace Core
{
	namespace Source
	{

		class Planewave
		{
		public:
			Planewave(ExcitationConfiguration*);
			~Planewave(){};

			dcomplex E0;

			/**
			 * \brief Set Exciation(righthand)
			 * \param bfVector 
			 * \return Excitation
			 */
			VectorXcd SetExcitation(const vector<IBasicFunction*>&bfVector)const;
		private:
			Vector3d _ki{0,0,-1};
			Vector3d _ei{1,0,0};
		};
	}
}

#endif
