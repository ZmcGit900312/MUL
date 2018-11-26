#include "stdafx.h"
#include "EquationKernel.h"
#include "Const.h"
#include "Data.h"

using namespace Core;
Core::EquationKernel::EquationKernel(IGreen *green, int eqtype)
{
	switch(eqtype)
	{
	case 1:
		break;
	case 2:
		break;
	default:
		pp = new EFIEpp(green);
		pm = new EFIEpm(green);
		mp = new EFIEmp(green);
		mm = new EFIEmm(green);
	}
	ImpK::SetQuad(13);
}

Core::EquationKernel::~EquationKernel()
{
	delete pp;
	delete pm;
	delete mp;
	delete mm;
}

complex<double> Core::EquationKernel::SetImpedance(IBasicFunction *source, IBasicFunction *field) const
{
	pp->ReSetBasicFunction(source, field);
	pm->ReSetBasicFunction(source, field);
	mp->ReSetBasicFunction(source, field);
	mm->ReSetBasicFunction(source, field);


	dcomplex d1 = ImpK::DoubleIntegration(pp,
		Mesh::GetInstance()->GetTriangle(source->LimitPlus()),
		Mesh::GetInstance()->GetTriangle(field->LimitPlus()));
	dcomplex d2 = ImpK::DoubleIntegration(pm,
		Mesh::GetInstance()->GetTriangle(source->LimitMinus()),
		Mesh::GetInstance()->GetTriangle(field->LimitPlus()));
	dcomplex d3 = ImpK::DoubleIntegration(mp,
		Mesh::GetInstance()->GetTriangle(source->LimitPlus()),
		Mesh::GetInstance()->GetTriangle(field->LimitMinus()));
	dcomplex d4 = ImpK::DoubleIntegration(mm,
		Mesh::GetInstance()->GetTriangle(source->LimitMinus()),
		Mesh::GetInstance()->GetTriangle(field->LimitMinus()));
	return 1i*k*eta*(d1+d2+d3+d4);
}

complex<double> Core::EquationKernel::SetRightHand(IBasicFunction *source) const
{
	pp->ReSetBasicFunction(source);
	mm->ReSetBasicFunction(source);
	return ImpK::SingleIntegration(pp,Mesh::GetInstance()->GetTriangle(source->LimitPlus()))
	+ ImpK::SingleIntegration(mm,Mesh::GetInstance()->GetTriangle(source->LimitMinus())) ;
}

