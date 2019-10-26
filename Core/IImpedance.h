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
		int ArrayNumX = 6, ArrayNumY = 7;
		int NumOfElement = 1;
		double ArrayIntervalX= 0.47, ArrayIntervalY = 0.43;
		double Threshold=0.3,Interval=0.05;
		double NearCorrectionEps = 1.0e-3;
		int FillingStrategy = 1;
		int VirtualGridTechnique = 1;
		Matrix<bool, Dynamic, Dynamic> ArrayLocation;
		Vector3d Box[2];
		EImpedance ImpType=MoM ;

		inline string GetImpedanceTypeString()const
		{
			switch (ImpType)
			{
			case MoM: return "MoM";
			case AIM: return "AIM";
			case Array: return "AIM-ARRAY";
			default: throw exception("error Impdance Type");
			}
		}
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
		virtual void SetExcitation(const VectorXcd&) = 0;
	};
}