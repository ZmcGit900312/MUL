#include "stdafx.h"
#include "IMatrixFiller.h"
#include "ConventionalMethod.h"
#include "VirtualGrid.h"

Core::IMatrixFiller::IMatrixFiller(const ImpConfiguration & configuration, IImpService * impedance):
	_layerNum(Vector3i{ configuration.xNumber,configuration.yNumber,configuration.zNumber }),
	_compute(k, W4, W7, eta),_interval(configuration.Interval),_unknowns(configuration.ImpSize),
_threshold(configuration.Threshold*Lambda),_eps(configuration.NearCorrectionEps),
_imp(static_cast<ImpAIM*>(impedance)){}

IMatrixFiller * Core::IMatrixFiller::FMatrixFiller(const ImpConfiguration & configuration, IImpService * impedance)
{
	if (configuration.VirtualGridTechnique == 1) return new VirtualGrid(configuration, impedance);

	return new ConventionalMethod(configuration, impedance);
}
