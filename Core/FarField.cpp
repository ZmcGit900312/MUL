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
#include <fstream>
#include <sstream>
#include<functional>
#include <ctime>
#include <iomanip>
#include "RWG.h"
#include "IntegrationRWG.h"

using namespace std;
using namespace placeholders;

Request::FF::FF(IGreen *green, vector<IBasicFunction*>*bf, Mesh *mesh) :
	_mesh(mesh), _bf(bf),_green(green)
{
	_plus = new RadiationKernelp(green);
	_minus = new RadiationKernelm(green);
	//_ra = new RadiationTest(green);
	_efield.resize(ThetaNum, PhiNum);
	_rcs.resize(ThetaNum, PhiNum);
	Vector3d D{ ComponentList::Geometry.GetLimitationBoundary(7) - ComponentList::Geometry.GetLimitationBoundary(0) };
	Radius = 100*D.squaredNorm() /Lambda;//2D^2/lam
	ITR::SetQuad(4);

}


bool Request::FF::WriteTxt(char * destfile) const
{
	if (!destfile)return false;
	if (!_efield.rows()*_efield.cols())return false;
	
	ofstream outEfieldS(destfile, ios_base::out);

	outEfieldS << "This is the FarField data,include E and RCS" << endl;

	int thetaStart = 0, _thetaEnd = 180;
	int phiStart = 0, phiEnd = 360;
	int thetaNum = ThetaNum + 2,phiNum=PhiNum+1;

	//里面有些变量问题
	outEfieldS << "Theta:\r\tfrom" << thetaStart << "\tto\t" << _thetaEnd << "\t" << thetaNum << endl;
	outEfieldS << "Phi:\r\tfrom" << phiStart << "\tto\t" << phiEnd << "\t" << phiNum << endl;
	outEfieldS << "Distance:" << Radius << "m" << endl;
	outEfieldS << "Theta\tPhi\t\tEfieldx\t\tEfieldy\t\tEfieldz\t\tRCS m\t\t" << endl;

	for (int iTheta =0; iTheta <thetaNum; ++iTheta)
	{
		for (int iPhi = 0; iPhi <phiNum; ++iPhi)
		{
			Vector3cd efield{GetEField(iTheta, iPhi) };
			outEfieldS << thetaStart + Interval*iTheta << "\t" << phiStart + Interval*iPhi;
			outEfieldS << "\t" << efield.x() << "\t\t";
			outEfieldS << "\t" << efield.y()  << "\t\t";
			outEfieldS << "\t" << efield.z()  << "\t\t";
			outEfieldS << "\t" << GetRCS(iTheta, iPhi) << endl;
		}
	}
	outEfieldS.close();
	return true;
}


Vector3cd Request::FF::GetEField(const int theta, const int phi)const
{
	switch (theta)
	{
	case 0:
		return _theta0;
	case 180:
		return _theta180;
	default:
		return _efield(theta-1, phi%360);
	}
}

double Request::FF::GetRCS(const int theta, const int phi)const
{
	switch (theta)
	{
	case 0:
		return _rcs0;
	case 180:
		return _rcs180;
	default:
		return _rcs(theta-1, phi%360);
	}
}


void Request::FF::SetEField()
{
	cout << "\nBegin to Calculate the 3D pattern...\n";

	const clock_t start = clock();

	_theta0 = EField(0, 0);
	_theta180 = EField(M_PI, 0);
	_rcs0 = 4 * M_PI*Radius*Radius*_theta0.squaredNorm();
	_rcs180 = 4 * M_PI*Radius*Radius*_theta180.squaredNorm();
	for(int iTheta=1;iTheta<=ThetaNum;++iTheta)
	{
		for (int iPhi = 1; iPhi <= PhiNum; ++iPhi)
		{
			const Vector3cd temp(EField(double(iTheta*Interval* M_PI_180),double(iPhi*Interval*M_PI_180)));
			_efield(iTheta - 1, iPhi - 1) = temp;
			_rcs(iTheta-1, iPhi-1)= 4 * M_PI*Radius*Radius*temp.squaredNorm();
		}
		cout << "Progress:" << setw(10) << double(iTheta)/ ThetaNum << "%\r";
	}
	const clock_t end = clock();
	const double time = double(end - start) / CLOCKS_PER_SEC;
	cout << "\n3D FarField cost\t=" << time << " s\nFinish!\n";
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

Vector3cd Core::Request::FF::EFieldModify(const double theta, const double phi) const
{
	const Vector3d ob(Radius*cos(phi)*sin(theta), Radius*sin(phi)*sin(theta), Radius*cos(theta));
	Vector3cd efield{ 0,0,0 };

	for (auto bf = _bf->begin(),ed=_bf->end(); bf!=ed; ++bf)//迭代器部分估计会有问题
	{
		auto zmc = static_cast<RWG*>(*bf);
		const short K = 4;
		complex<double> plus(0), minus(0);
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