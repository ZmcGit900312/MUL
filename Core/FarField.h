///////////////////////////////////////////////////////////
//  FarField.h
//  Implementation of the Class FarField
//  Created on:      24-4��-2018 20:40:02
//  Original author: ZhengMX
///////////////////////////////////////////////////////////

#pragma once
#include "Green.h"
#include "RequestConfiguration.h"
#include "Radiation.h"
#include "IBasicFunction.h"
#include "Mesh.h"

namespace Core
{
	
	namespace Request
	{
		/**
		* \brief Զ��Sphere�汾
		* �Ż����ã��ṩGetEField��GetRCS������RCS�͵糡
		*/
		class FF
		{
		public:
			FF(IGreen*, vector<IBasicFunction*>*, Mesh*);
			~FF() { delete _plus; delete _minus; }
			/**
			* \brief ��ȡָ������糡
			* \param theta from 0 to 180
			* \param phi from 1 to 360
			* \return ���أ�theta,phi��RCS
			*/
			Vector3cd GetEField(const int theta, const int phi)const;
			/**
			* \brief ��ȡָ������RCS
			* \param theta from 0 to 180
			* \param phi from 1 to 360
			* \return ���أ�theta,phi��RCS
			*/
			double GetRCS(const int theta, const int phi)const;
			/**
			* \brief Calculate 3D RCS
			*/
			void SetEField();
			/**
			* \brief Calculate the E field of the specific direction(theta,phi)
			* \param theta rad
			* \param phi rad
			* \return Vector3cd Efield
			*/
			Vector3cd EField(const double theta, const double phi) const;


			bool WriteTxt(char* destfile) const;
			/**
			* \brief Radius of farfield observation
			* \n\b PS: It should be ajust with the size of object
			*/
			double Radius = 10000;//
		private:
			typedef IntegrationTriangle<RadiationKernel> ITR;//���ֶ���

			Mesh* _mesh;
			vector<IBasicFunction*>*_bf;
			//Integration Kernel and Integration
			const double Interval = 1;
			const int ThetaNum = 179, PhiNum = 360;
			Vector3cd _theta0, _theta180;
			double _rcs0 = 0, _rcs180 = 0;
			Matrix<Vector3cd, Dynamic, Dynamic> _efield;
			MatrixXd _rcs;
			RadiationKernel* _plus = nullptr, *_minus = nullptr;
		};
	}
	
	
}

