///////////////////////////////////////////////////////////
//  FarField.h
//  Implementation of the Class FarField
//  Created on:      24-4��-2018 20:40:02
//  Original author: ZhengMX
///////////////////////////////////////////////////////////

#pragma once
#include "RequestConfiguration.h"
#include "Radiation.h"
#include "IBasicFunction.h"
#include "Mesh.h"

namespace Core
{
	
	namespace Request
	{
		/**
		* \brief Զ��Sphere�汾 ����Ч��̫��
		* �Ż����ã��ṩGetEField��GetRCS������RCS�͵糡
		*/
		class FarField
		{
		public:
			FarField(vector<IBasicFunction*>*, Mesh*);
			~FarField() {}
			/**
			* \brief Calculate 3D RCS
			*/
			void SetEField(FarFieldConfiguration& config,ofstream& ofs) const;
			/**
			* \brief Calculate the E field of the specific direction(theta,phi)
			* \param theta rad
			* \param phi rad
			* \return Vector3cd EfieldE
			*/
			Vector3cd EField(const double theta, const double phi) const;

			/**
			 * \brief 
			 * \param theta 
			 * \param phi 
			 * \return 
			 */
			Vector3cd EFieldBenchMark(const double theta, const double phi)const;
			
			/*bool WriteTxt(char* destfile) const;*/

			/**
			* \brief Radius of farfield observation
			* \n\b PS: It should be ajust with the size of object
			*/
			double Radius = 10000;//
		private:

			Mesh* _mesh;
			vector<IBasicFunction*>*_bf;
			//Integration Kernel and Integration
			double _coef;
			Vector3cd _theta0, _theta180;
			double _rcs0 = 0, _rcs180 = 0;
		};




	}
	
	
}

