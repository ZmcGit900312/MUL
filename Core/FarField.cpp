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

using namespace std;
using namespace placeholders;

Request::FF::FF(IGreen *green, vector<IBasicFunction*>*bf, Mesh *mesh) :
	_bf(bf), _mesh(mesh)
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

/**
 * \brief 
 * \param destfile 
 * \return 
 */
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



/**
* \brief 获取指定方向电场
* \param theta from 0 to 180
* \param phi from 1 to 360
* \return 返回（theta,phi）RCS
*/
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

/**
 * \brief 获取指定方向RCS
 * \param theta from 0 to 180
 * \param phi from 1 to 360
 * \return 返回（theta,phi）RCS
 */
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

/**
 * \brief Call private function EField to Fill _efield and _rcs
 */
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

/**
 * \brief Calculate the E field of the specific direction(theta,phi)
 * \param theta rad
 * \param phi rad
 * \return Vector3cd Efield
 */
Vector3cd Request::FF::EField(const double theta, const double phi) const
{
	const Vector3d observation(Radius*cos(phi)*sin(theta),Radius*sin(phi)*sin(theta),Radius*cos(theta));
	_plus->SetObservation(observation);
	_minus->SetObservation(observation);
	Vector3cd efield{0,0,0};
	for (auto index = _bf->begin(); index != _bf->end(); ++index)//迭代器部分估计会有问题
	{
		IBasicFunction* bfptr = *index;
		_plus->SetBasicFunction(bfptr);
		_minus->SetBasicFunction(bfptr);

		efield += bfptr->Current()*
			(ITR::SingleIntegration(_plus, _mesh->GetTriangle(bfptr->LimitPlus())) +
				ITR::SingleIntegration(_minus, _mesh->GetTriangle(bfptr->LimitMinus())));
		//以下代码并没有考虑binder中的Output1和Output2
		/*typedef	_Binder<_Unforced,
		RadiationTest::Output1(RadiationTest::*)(IBasicFunction*, bool, Vector3d, Vector3d)const,
		RadiationTest*&, IBasicFunction*&, bool, Vector3d&, const _Ph<1>&> binder;
		binder *rdPlus = &bind(&RadiationTest::operator(), _ra, *index, true, observation, _1);

		cout << IntegrationTriangle<binder>::SingleIntegration(rdPlus, mesh->GetTriangle((*index)->LimitPlus())) << endl;*/

	}
	return efield;
}
