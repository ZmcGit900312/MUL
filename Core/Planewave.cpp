#include "stdafx.h"
#include "Excitation.h"
#include "Const.h"
#include "Data.h"
#include <Eigen/Geometry>//包含cross操作
using namespace Core;

Core::Source::Planewave::Planewave(std::string name, unsigned thn, unsigned phn, double ths, double phs, double thi, double phi, double mag, double phase, double eta, double rx, double ry, double rz) :ThetaNum(thn), PhiNum(phn), ThetaStart(ths), PhiStart(phs), ThetaIncrement(thi), PhiIncrement(phi), Magnitude(mag), Phase(phase), Eta(eta), RotationX(rx), RotationY(ry), RotationZ(rz)
{
	Name = name;
	//对于FEKO中的phi方向不用+180度
	double Theta = ThetaStart * M_PI_180, Phi = PhiStart * M_PI_180;

	RotationX *= M_PI_180;
	RotationY *= M_PI_180;
	RotationZ *= M_PI_180;

	Eta *= M_PI_180;

	Ki = Vector3d{ -sin(Theta)*cos(Phi),-sin(Theta)*sin(Phi),-cos(Theta) };
	//Original Ei=Vector3d(-sin(theta)*cos(phi),-sin(theta)*sin(phi),cos(theta))
	Ei = Vector3d(-cos(Theta)*cos(Phi), -cos(Theta)*sin(Phi), sin(Theta));
	Hi = Vector3d(-sin(Phi), cos(Phi), 0);
	//_ei = Rotation(theta, phi, Vector3d{ -cos(eta),sin(eta),0.0 });
	E0 = Magnitude * exp(1i*Phase*M_PI_180);
	H0 = E0 / eta0;
	//E=(E0+1i*ell*E0.cross(Ki))

}

VectorXcd Core::Source::PlaneWaveLinear::SetExcitation(const vector<IBasicFunction*>& bfVector) const
{
	//RWGImpOperator compute(k, W4, W7, eta);

	const size_t unknowns = bfVector.size();
	VectorXcd righthand{ unknowns };
	size_t zmc = 0;

	switch (equation->GetType())
	{
	case EFIE:
		for (auto val : bfVector)
		{			
			righthand(zmc++) = equation->SetRight(static_cast<RWG*>(val), Ki, E0*Ei);
		}break;
	case MFIE:
		for (auto val : bfVector)
		{			
			righthand(zmc++) = equation->SetRight(static_cast<RWG*>(val), Ki, H0*Hi);
		}break;
	case CFIE: 
		for (auto val : bfVector)
		{			
			righthand(zmc++) = equation->SetRight(static_cast<RWG*>(val), Ki, E0*Ei,H0*Hi);
		}break;
	case IBCEFIE:  throw spd::spdlog_ex("IBCEFIE is not developmented");
	case IBCMFIE:  throw spd::spdlog_ex("IBCMFIE is not developmented");
	case IBCCFIE:  throw spd::spdlog_ex("IBCCFIE is not developmented");
	default: throw spd::spdlog_ex("Error in PlanewaveLinear");
	}

	return righthand;
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
		righthand(row) = integral1.SetIncidentFieldVector(bfVector[row]);
		righthand(row+1)= integral2.SetIncidentFieldVector(bfVector[row+1]);
		righthand(row+2) = integral3.SetIncidentFieldVector(bfVector[row+2]);
		righthand(row + 3) = integral4.SetIncidentFieldVector(bfVector[row+3]);
	}
	for (size_t row =limit ; row < unknowns; ++row)
	{
		righthand(row) = integral1.SetIncidentFieldVector(bfVector[row]);
	}


	return righthand*E0;
}
*/