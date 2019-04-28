#include "stdafx.h"
#include "Excitation.h"
#include "Const.h"
#include "IntegrationRWG.h"
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
	//_ei = Rotation(theta, phi, Vector3d{ -cos(eta),sin(eta),0.0 });
	E0 = Magnitude * exp(1i*Phase*M_PI_180);
	//E=(E0+1i*ell*E0.cross(Ki))

}

VectorXcd Core::Source::PlaneWaveLinear::SetExcitation(const vector<IBasicFunction*>& bfVector) const
{
	EFRImp compute(k, W4, W7, eta);
	const size_t unknowns = bfVector.size();
	VectorXcd righthand{ unknowns };
	size_t zmc = 0;
	for (auto it = bfVector.begin(), ed = bfVector.end();it != ed;++it)
	{
		righthand(zmc++) = compute.SetRightHand(static_cast<RWG*>(*it), Ki, Ei);
	}

	return righthand * E0;
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