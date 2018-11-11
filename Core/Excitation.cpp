#include "stdafx.h"
#include "Excitation.h"
#include "Integral.h"
using namespace Core;
using namespace Core::Internal;

Excitation::T4 Excitation::PlaneWave = {EIGEN_PI / 2,0.0,1.0,0.0};
map<Index, Excitation::T4> Excitation::Radiation{{0,PlaneWave}};

void Excitation::SetExcitation(VectorXcd &righthand)
{
	const Index length = righthand.rows();
	for (Index i = 0; i < length; ++i)
	{
		righthand[i] = internal::random<VectorXcd::Scalar>();
		//righthand[i]=TrianlgeIntegral(PlaneWave)
	}
}
