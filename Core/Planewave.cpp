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

VectorXcd Core::Source::PlaneWaveLinear::SetExcitation(const vector<IBasicFunction*>& bfVector, ImpConfiguration& impconfig) const
{
	if(impconfig.ImpType==Core::Array)
	{
		size_t elementUnknowns = impconfig.ImpSize;
		int arrayX = impconfig.numArrayX, arrayY = impconfig.numArrayY;
		int numOfElement = arrayX * arrayY;
		VectorXcd righthand{ numOfElement *elementUnknowns };
		righthand.setZero();

		size_t zmc = 0;
		for (int zmcy = 0; zmcy < SystemConfig.ImpConfig.numArrayY; ++zmcy)
		{
			for (int zmcx = 0; zmcx < SystemConfig.ImpConfig.numArrayX; ++zmcx)
			{
				Vector3d bias{ zmcx*impconfig.distanceBiasX,zmcy*impconfig.distanceBiasY,0 };
				righthand.segment(elementUnknowns*zmc++,elementUnknowns) = 
					SetElementExcitation(bfVector, bias);
			}
		}
		return righthand;
	}
	else
	{
		return SetElementExcitation(bfVector, Vector3d::Zero());
	}

}

VectorXcd Core::Source::PlaneWaveLinear::SetElementExcitation(
	const vector<IBasicFunction*>& bfVector, Vector3d bias) const
{
	VectorXcd righthand{ bfVector.size() };
	size_t zmc = 0;
	auto ty = equation->GetType();

	if (ty == IBCEFIE)throw spd::spdlog_ex("IBCEFIE is not developmented");
	if (ty == IBCMFIE)throw spd::spdlog_ex("IBCMFIE is not developmented");
	if (ty == IBCCFIE)throw spd::spdlog_ex("IBCCFIE is not developmented");

	if(ty==EFIE||ty==MFIE||ty==CFIE)
	{
		for (auto val : bfVector)
		{
			righthand(zmc++) = equation->SetRight(static_cast<RWG*>(val), Ki, E0*Ei, bias);
		}
		return righthand;
	}

	throw spd::spdlog_ex("Error in PlanewaveLinear");


	/*switch (equation->GetType())
	{
	case EFIE:
		for (auto val : bfVector)
		{
			righthand(zmc++) = equation->SetRight(static_cast<RWG*>(val), Ki, E0*Ei, bias);
		}break;
	case MFIE:
		for (auto val : bfVector)
		{
			righthand(zmc++) = equation->SetRight(static_cast<RWG*>(val), Ki, H0*Hi, bias);
		}break;
	case CFIE:
		for (auto val : bfVector)
		{
			righthand(zmc++) = equation->SetRight(static_cast<RWG*>(val), Ki, E0*Ei, bias);
		}break;
	case IBCEFIE:  throw spd::spdlog_ex("IBCEFIE is not developmented");
	case IBCMFIE:  throw spd::spdlog_ex("IBCMFIE is not developmented");
	case IBCCFIE:  throw spd::spdlog_ex("IBCCFIE is not developmented");
	default: throw spd::spdlog_ex("Error in PlanewaveLinear");
	}
	return righthand;*/
}
