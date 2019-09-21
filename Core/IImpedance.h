///////////////////////////////////////////////////////////
//  IImpConfiguration.h
//  Implementation of the Interface IImpConfiguration
//  Created on:      15-12ÔÂ-2017 16:04:04
//  Original author: ZhengMX
///////////////////////////////////////////////////////////

#pragma once
#include <Eigen/Core>
#include "Log.h"
using namespace Eigen;

namespace Core
{
	enum EImpedance { MoM, AIM, Array };
	struct ImpConfiguration
	{
		size_t ImpSize;
		int GridOrder=2;
		int Dimension=3;
		int xNumber, yNumber, zNumber;
		int numArrayX = 6, numArrayY = 7;
		double distanceBiasX= 0.47, distanceBiasY = 0.43;
		double Threshold=0.3,Interval=0.05;
		double NearCorrectionEps = 1.0e-3;
		int FillingStrategy = 1;
		int VirtualGridTechnique = 1;
		Vector3d Box[2];
		EImpedance impType=MoM ;
	};

	class IImpService
	{
	public:

		IImpService() {}
		virtual ~IImpService() {}

		virtual void FillImpedance() = 0;
		virtual double GetTime()const = 0;
		virtual double GetMemoryCost()const = 0;
		virtual VectorXcd& GetExcitation() = 0;
	};
}