///////////////////////////////////////////////////////////
//  FarField.cpp
//  Implementation of the Class FarField
//  Created on:      24-4月-2018 20:40:02
//  Original author: ZhengMX
///////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FarField.h"
#include "Const.h"
#include "Data.h"
#include "RWG.h"
#include "IntegrationRWG.h"
#include "Log.h"

using namespace std;
using namespace placeholders;

Request::FF::FF(vector<IBasicFunction*>*bf, Mesh *mesh) :
	_mesh(mesh), _bf(bf)
{
	Vector3d D{ ComponentList::Geometry.GetLimitationBoundary(7) - ComponentList::Geometry.GetLimitationBoundary(0) };
	Radius = 100*D.squaredNorm() /Lambda;//2D^2/lam
	_coef = 4 * M_PI*Radius*Radius;
}


//bool Request::FF::WriteTxt(char * destfile) const
//{
//	if (!destfile)return false;
//	
//	ofstream outEfieldS(destfile, ios_base::out);
//
//	outEfieldS << "This is the FarField data,include E and RCS" << endl;
//
//	int thetaStart = 0, _thetaEnd = 180;
//	int phiStart = 0, phiEnd = 360;
//	int thetaNum = ThetaNum + 2,phiNum=PhiNum+1;
//
//	//里面有些变量问题
//	outEfieldS << "Theta:\r\tfrom" << thetaStart << "\tto\t" << _thetaEnd << "\t" << thetaNum << endl;
//	outEfieldS << "Phi:\r\tfrom" << phiStart << "\tto\t" << phiEnd << "\t" << phiNum << endl;
//	outEfieldS << "Distance:" << Radius << "m" << endl;
//	outEfieldS << "Theta\tPhi\t\tEfieldx\t\tEfieldy\t\tEfieldz\t\tRCS m\t\t" << endl;
//
//	for (int iTheta =0; iTheta <thetaNum; ++iTheta)
//	{
//		for (int iPhi = 0; iPhi <phiNum; ++iPhi)
//		{
//			Vector3cd efield{GetEField(iTheta, iPhi) };
//			outEfieldS << thetaStart + Interval*iTheta << "\t" << phiStart + Interval*iPhi;
//			outEfieldS << "\t" << efield.x() << "\t\t";
//			outEfieldS << "\t" << efield.y()  << "\t\t";
//			outEfieldS << "\t" << efield.z()  << "\t\t";
//			outEfieldS << "\t" << GetRCS(iTheta, iPhi) << endl;
//		}
//	}
//	outEfieldS.close();
//	return true;
//}



void Request::FF::SetEField(FarFieldConfiguration& config, ofstream& ofs) const
{
	Console->info("Calculate Request: {}",config.FarFileName);
	ResultL->info("Calculate Request: {}", config.FarFileName);
	const int thetaNum = config.ThetaNum;
	const int phiNum = config.PhiNum;
	const double thetaS = config.ThetaStart;
	const double phiS = config.PhiStart;
	const double thetaI = config.ThetaIncrement;
	const double phiI = config.PhiIncrement;
	const double Sum = 0.01*thetaNum * phiNum;

	ofs << setw(7) << "Theta" << setw(7) << "Phi" << setw(12) << "RCS(m2)" << endl;
	for (int th = 0; th < thetaNum; ++th)
	{
		for (int ph = 0; ph < phiNum; ++ph)
		{
			const double theta = thetaS + th * thetaI;
			const double phi = phiS + ph * phiI;
			Vector3cd temp = EField(theta*M_PI_180, phi*M_PI_180);
			ofs << setw(7) << theta << setw(7) << phi << setw(12) << _coef * temp.squaredNorm() << '\n';
			cout << "Progress:" << setw(10) << (th*phiNum + ph + 1) / Sum << "%\r";
		}
	}

}


Vector3cd Request::FF::EField(const double theta, const double phi) const
{
	const Vector3d observation(Radius*cos(phi)*sin(theta),Radius*sin(phi)*sin(theta),Radius*cos(theta));
	Vector3cd efield{0,0,0};
	EFRImp compute(k, W4, W7, eta);
	for (auto zmc = _mesh->TriangleVector()->begin(), ed = _mesh->TriangleVector()->end();zmc != ed;++zmc)
	{
		dcomplex current[3] = { {0,0},{0,0},{0,0} };
		current[0] = zmc->RWGSign[0] ? static_cast<RWG*>(_bf->at(zmc->ID(0).second))->Current() : 0;
		current[1] = zmc->RWGSign[1] ? static_cast<RWG*>(_bf->at(zmc->ID(1).second))->Current() : 0;
		current[2] = zmc->RWGSign[2] ? static_cast<RWG*>(_bf->at(zmc->ID(2).second))->Current() : 0;
		efield += compute.Radiation(*zmc, observation,current);
	}
	
	return efield;
}

Vector3cd Core::Request::FF::EFieldBenchMark(const double theta, const double phi) const
{
	const Vector3d ob(Radius*cos(phi)*sin(theta), Radius*sin(phi)*sin(theta), Radius*cos(theta));
	Vector3cd efield{ 0,0,0 };
	auto _green = ComponentList::Green;
	for (auto bf = _bf->begin(),ed=_bf->end(); bf!=ed; ++bf)//迭代器部分估计会有问题
	{
		auto zmc = static_cast<RWG*>(*bf);
		const short K = 4;
		Triangle& tplus = zmc->TrianglePlus();
		Triangle& tminus = zmc->TriangleMinus();
		Vector3cd temp{ 0,0,0 };
		for (int i = 0; i < K; ++i)
		{//Source Triangle
			Vector3d pt1 = tplus.Quad4()[i], pt2 = tminus.Quad4()[i];
			Vector3cd eplus = -1i*Omega*Mu*_green->Scalar(pt1, ob)*zmc->CurrentPlus(pt1) +
				1i / (Omega*Epsilon)*zmc->ChargePlus(pt1)*_green->Gradient(pt1, ob);
			Vector3cd eminus = -1i*Omega*Mu*_green->Scalar(pt2, ob)*zmc->CurrentMinus(pt2) +
				1i / (Omega*Epsilon)*zmc->ChargeMinus(pt2)*_green->Gradient(pt2, ob);
			temp += W4[i]*eplus* tplus.Area() + W4[i]*eminus* tminus.Area();
		}
		efield += zmc->Current()*temp;
	}
	return efield;
}