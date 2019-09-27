#include "stdafx.h"
#include "IMatrixFiller.h"
#include "ConventionalMethod.h"
#include "VirtualGrid.h"
#include "AIMPECCFIE.h"
#include "AIMArray.h"

IMatrixFiller * Core::IMatrixFiller::FMatrixFiller(
	const ImpConfiguration & configuration, 
	IImpService * impedance,
	const IEConfiguration& ieConfig)
{

	if(ieConfig.type==EFIE)
	{
		if(configuration.ImpType==AIM)
		{
			if(configuration.VirtualGridTechnique==1)return new VirtualGrid(configuration, impedance, ieConfig);
			else return new ConventionalMethod(configuration, impedance, ieConfig);
			
		}
		else if(configuration.ImpType==Array)
		{
			return new AIMArray(configuration, impedance, ieConfig);
		}
	}
	else if(ieConfig.type == CFIE)return new AIMPECCFIE(configuration, impedance, ieConfig);

	throw spd::spdlog_ex("Error IMatrixFiller.cpp choose Method!");
}
