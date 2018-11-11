///////////////////////////////////////////////////////////
//  EquationKernel.h
//  Implementation of the Class EquationKernel
//  Created on:      20-12ÔÂ-2017 19:33:33
//  Original author: ZhengMX
///////////////////////////////////////////////////////////

#pragma once
#include <Eigen\Core>
#include <complex>
#include "IEKernel.h"
#include "Integration.h"

using namespace Eigen;
using namespace std;
using namespace Core;
namespace Core
{

	class  EquationKernel
	{
		
	public:
		typedef IntegrationTriangle<IE> ImpK;
		EquationKernel(IGreen*, int eqtype);
		~EquationKernel();
		complex<double> SetImpedance(IBasicFunction*, IBasicFunction*) const;
		complex<double> SetRightHand(IBasicFunction*)const;
	private:
		IE*pp, *pm, *mp , *mm;
	};

}


