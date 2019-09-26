///////////////////////////////////////////////////////////
//  FarField.h
//  Implementation of the Class FarField
//  Created on:      24-4月-2018 20:40:02
//  Original author: ZhengMX
///////////////////////////////////////////////////////////

#pragma once
#include "RequestConfiguration.h"
#include "IBasicFunction.h"
#include "Mesh.h"
#include "Current.h"
namespace Core
{
	
	namespace Request
	{
		/**
		* \brief 远场Sphere版本 计算效率太低
		* 优化更好，提供GetEField和GetRCS来给出RCS和电场
		*/
		class FarField
		{
		public:
			FarField(vector<IBasicFunction*>*, Mesh*);
			~FarField() {}
			/**
			* \brief Calculate RCS of Configuration
			*/
			void CalculateRCS(FarFieldConfiguration& config,ofstream& ofs) const;
			/**
			* \brief Calculate the E field of the specific direction(theta,phi)
			* \param theta rad
			* \param phi rad
			* \return Vector3cd Efield
			*/
			Vector3cd EField(const double theta, const double phi) const;

			/**
			 * \brief 
			 * \param theta 
			 * \param phi 
			 * \return 
			 */
			Vector3cd EFieldBenchMark(const double theta, const double phi)const;
			

			/**
			* \brief Calculate the E field of the specific direction(theta,phi)
			* \param theta rad
			* \param phi rad
			* \return Vector3cd Efield
			*/
			Vector3cd EFieldArray(const double theta, const double phi) const;
			Vector3cd EFieldArrayBenchMark(const double theta, const double phi)const;
			/**
			* \brief Radius of farfield observation
			* \n\b PS: It should be ajust with the size of object
			*/
			double Radius = 10000;//
			double Coef;
			Solution::CurrentInfo* _current = nullptr;
		private:

			Mesh* _mesh;
			vector<IBasicFunction*>*_bf;
			//Integration Kernel and Integration
			
			Vector3cd _theta0, _theta180;
			double _rcs0 = 0, _rcs180 = 0;
		};




	}
	
	
}

