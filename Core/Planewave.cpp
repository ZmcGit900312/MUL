#include "stdafx.h"
#include "Planewave.h"
#include "Const.h"
#include "IntegrationRWG.h"

using namespace Core;

Core::Source::Planewave::Planewave(ExcitationConfiguration*config):_ki(config->Ki),_ei(config->Ei)
{

	E0 = config->Magnitude * exp(-1i*config->Phase*M_PI_180);
}


/**
* \brief Set Exciation(righthand)//Don't use
*/
/*
VectorXcd Source::Planewave::SetExcitation(IGreen* green,const vector<IBasicFunction*>&bfVector)const
{
	EquationKernel integral1(green, 0), integral2(green, 0), integral3(green, 0), integral4(green, 0);
	const size_t unknowns = bfVector.size();
	const size_t limit = unknowns - unknowns % 4;

	VectorXcd righthand{ unknowns };

	//Unrolling expanding
	for (size_t row = 0; row < limit; row+=4)
	{
		righthand(row) = integral1.SetRightHand(bfVector[row]);
		righthand(row+1)= integral2.SetRightHand(bfVector[row+1]);
		righthand(row+2) = integral3.SetRightHand(bfVector[row+2]);
		righthand(row + 3) = integral4.SetRightHand(bfVector[row+3]);
	}
	for (size_t row =limit ; row < unknowns; ++row)
	{
		righthand(row) = integral1.SetRightHand(bfVector[row]);
	}


	return righthand*E0;
}
*/
VectorXcd Source::Planewave::SetExcitation(const vector<IBasicFunction*>&bfVector)const
{
	EFRImp compute(k, W4, W7, eta);
	const size_t unknowns = bfVector.size();
	VectorXcd righthand{ unknowns };
	size_t zmc = 0;
	for(auto it=bfVector.begin(),ed=bfVector.end();it!=ed;++it)
	{
		righthand(zmc++) = compute.SetRightHand(static_cast<RWG*>(*it), _ki, _ei);
	}

	return righthand * E0;
	
}