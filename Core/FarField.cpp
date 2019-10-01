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
#include "ModalGeometry.h"

using namespace std;
using namespace placeholders;

Request::FarField::FarField(vector<IBasicFunction*>*bf, Mesh *mesh, Solution::CurrentInfo* current) :
	_mesh(mesh), _bf(bf), _current(current)
{
	Vector3d D{ Assist::ModalGeometry::GetInstance()->GetLimitationBoundary(7) - Assist::ModalGeometry::GetInstance()->GetLimitationBoundary(0) };
	Radius = 100*D.squaredNorm() /Lambda;//2D^2/lam
	Coef = 4 * M_PI*Radius*Radius;
}

Core::Request::FarField::~FarField()
{
	_mesh = nullptr;
	_current = nullptr;
	_bf = nullptr;
}




//void Request::FarField::CalculateRCS(FarFieldConfiguration& config, ofstream& ofs) const
//{
//	
//	const int thetaNum = config.ThetaNum;
//	const int phiNum = config.PhiNum;
//	const double thetaS = config.ThetaStart;
//	const double phiS = config.PhiStart;
//	const double thetaI = config.ThetaIncrement;
//	const double phiI = config.PhiIncrement;
//	const double Sum = 0.01*thetaNum * phiNum;
//
//	ofs << "Theta" <<',' << "Phi" <<','<< "RCS(m2)" << '\n';
//	for (int th = 0; th < thetaNum; ++th)
//	{
//		for (int ph = 0; ph < phiNum; ++ph)
//		{
//			const double theta = thetaS + th * thetaI;
//			const double phi = phiS + ph * phiI;
//			Vector3cd temp = EField(theta*M_PI_180, phi*M_PI_180);
//			//ofs << setw(7) << theta << setw(7) << phi << setw(12) << _coef * temp.squaredNorm() << '\n';
//			ofs <<theta << ',' << phi << ','<< Coef * temp.squaredNorm() << '\n';
//			cout << "Progress:" << setw(10) << (th*phiNum + ph + 1) / Sum << "%\r";
//		}
//	}
//
//}


void Request::FarField::CalculateRCS(FarFieldConfiguration& config, ofstream& ofs) const
{

	const int thetaNum = config.ThetaNum;
	const int phiNum = config.PhiNum;
	const double thetaS = config.ThetaStart;
	const double phiS = config.PhiStart;
	const double thetaI = config.ThetaIncrement;
	const double phiI = config.PhiIncrement;
	const double Sum = 0.01*thetaNum * phiNum;


	VectorXd Theta{ VectorXd::LinSpaced(thetaNum,thetaS,thetaS + thetaI * (thetaNum - 1)) };
	VectorXd Phi{ VectorXd::LinSpaced(phiNum,phiS,phiS + phiI * (phiNum - 1)) };

	vector<MatrixXd> RCS;
	RCS.reserve(_current->_numberOfConfig);
	for (int zmc = 0; zmc < _current->_numberOfConfig; ++zmc)
	{
		MatrixXd rcs{ thetaNum, phiNum };
		//Update Const
		_current->Current[zmc]->EMCParameterUpdate();
		if(_current->category==Core::Array)
		{
			for (int th = 0; th < thetaNum; ++th)
			{
				for (int ph = 0; ph < phiNum; ++ph)
				{
					Vector3cd temp{ EField(Theta(th)*M_PI_180, Phi(ph)*M_PI_180,
						static_cast<Solution::ArrayCurrent*>(_current->Current[zmc])) };
					rcs(th, ph) = Coef * temp.squaredNorm();
					cout << "Progress:" << setw(10) << (th*phiNum + ph + 1) / Sum<< "%"<<setw(10)
					<< zmc + 1 << '/' << _current->_numberOfConfig << "\r";
				}
			}
		}
		else
		{
			for (int th = 0; th < thetaNum; ++th)
			{
				for (int ph = 0; ph < phiNum; ++ph)
				{
					Vector3cd temp{ EField(Theta(th)*M_PI_180, Phi(ph)*M_PI_180, 
						_current->Current[zmc]) };
					rcs(th, ph) = Coef * temp.squaredNorm();
					cout << "Progress:" << setw(10) << (th*phiNum + ph + 1) / Sum << "%" << setw(10)
						<< zmc + 1 << '/' << _current->_numberOfConfig << "\r";
				}
			}
		}				
		RCS.push_back(rcs);
	}


	//Write Part
	//Title
	ofs << "Theta" << ',' << "Phi"; 
	for(auto val:_current->Current)ofs << ',' << val->Tag + " RCS(m2)"; 
	ofs << '\n';

	for (int th = 0; th < thetaNum; ++th)
	{
		for (int ph = 0; ph < phiNum; ++ph)
		{
			ofs << Theta(th) << ',' << Phi(ph);
			for(auto mx:RCS)
			{
				ofs << ',' << mx(th, ph);
			}			
			ofs << '\n';
		}
	}

}

Vector3cd Request::FarField::EField(const double theta, const double phi, 
	Solution::ElementCurrent* sol) const
{
	const Vector3d observation(Radius*cos(phi)*sin(theta),Radius*sin(phi)*sin(theta),Radius*cos(theta));
	Vector3cd efield{0,0,0};
	
	//for (auto zmc = _mesh->TriangleVector.begin(), ed = _mesh->TriangleVector.end();zmc != ed;++zmc)
	//{
	//	dcomplex current[3] = { {0,0},{0,0},{0,0} };
	//	RWGTriangle* temp = dynamic_cast<RWGTriangle*>(*zmc);
	//	current[0] = temp->RWGSign[0] ? static_cast<RWG*>(_bf[temp->RWGID(0)])->Current() : 0;
	//	current[1] = temp->RWGSign[1] ? static_cast<RWG*>(_bf[temp->RWGID(1)])->Current() : 0;
	//	current[2] = temp->RWGSign[2] ? static_cast<RWG*>(_bf[temp->RWGID(2)])->Current() : 0;
	//	//efield += compute.OperatorLScatter(temp, observation,current);
	//	efield += equation->LScatter(temp, observation, current);
	//}
	
	for (auto zmc = _mesh->TriangleVector.begin(), ed = _mesh->TriangleVector.end();zmc != ed;++zmc)
	{
		dcomplex current[3] = { {0,0},{0,0},{0,0} };
		RWGTriangle* temp = dynamic_cast<RWGTriangle*>(*zmc);
		current[0] = temp->RWGSign[0] ? sol->GetCurrent(temp->RWGID(0)) : 0;
		current[1] = temp->RWGSign[1] ? sol->GetCurrent(temp->RWGID(1)) : 0;
		current[2] = temp->RWGSign[2] ? sol->GetCurrent(temp->RWGID(2)) : 0;

		efield += equation->LScatter(temp, observation, current);
	}

	return efield;
}

Vector3cd Core::Request::FarField::EField(const double theta, const double phi, Solution::ArrayCurrent* sol) const
{
	const Vector3d observation(Radius*cos(phi)*sin(theta), Radius*sin(phi)*sin(theta), Radius*cos(theta));
	Vector3cd efield{ 0,0,0 };

	for (auto zmc = _mesh->TriangleVector.begin(), ed = _mesh->TriangleVector.end();zmc != ed;++zmc)
	{
		dcomplex current[3] = { {0,0},{0,0},{0,0} };
		Vector3cd subEfield{ 0,0,0 };
		RWGTriangle* temp = dynamic_cast<RWGTriangle*>(*zmc);

		for (Matrix<int, 2, 1, 0> loc : sol->_arrayLocation)
		{
			Vector3d Rb{ loc.x()*sol->_arrayBiasX,loc.y()*sol->_arrayBiasY,0 };
			current[0] = temp->RWGSign[0] ? sol->GetCurrent(temp->RWGID(0), loc.x(), loc.y()) : 0;
			current[1] = temp->RWGSign[1] ? sol->GetCurrent(temp->RWGID(1), loc.x(), loc.y()) : 0;
			current[2] = temp->RWGSign[2] ? sol->GetCurrent(temp->RWGID(2), loc.x(), loc.y()) : 0;
			subEfield += equation->LScatter(temp, observation - Rb, current);
		}

		efield += subEfield;
	}

	return efield;
}



Vector3cd Core::Request::FarField::EFieldBenchMark(const double theta, const double phi, VectorXcd& current) const
{
	const Vector3d ob(Radius*cos(phi)*sin(theta), Radius*sin(phi)*sin(theta), Radius*cos(theta));
	Vector3cd efield{ 0,0,0 };
	auto _green = IGreen::GetInstance();
	for (auto bf = _bf->begin(),ed=_bf->end(); bf!=ed; ++bf)//迭代器部分估计会有问题
	{
		auto zmc = static_cast<RWG*>(*bf);
		const short K = 4;
		RWGTriangle* tplus = zmc->TrianglePlus();
		RWGTriangle* tminus = zmc->TriangleMinus();

		
		Vector3cd temp{ 0,0,0 };
		for (int i = 0; i < K; ++i)
		{//Source Triangle
			Vector3d pt1 = tplus->Quad4()[i], pt2 = tminus->Quad4()[i];
			Vector3cd eplus = -1i*Omega*Mu*_green->Scalar(pt1, ob)*zmc->CurrentPlus(pt1) +
				1i / (Omega*Epsilon)*zmc->ChargePlus(pt1)*_green->Gradient(pt1, ob);
			Vector3cd eminus = -1i*Omega*Mu*_green->Scalar(pt2, ob)*zmc->CurrentMinus(pt2) +
				1i / (Omega*Epsilon)*zmc->ChargeMinus(pt2)*_green->Gradient(pt2, ob);
			temp += W4[i]*eplus* tplus->Area() + W4[i]*eminus* tminus->Area();
		}
		efield += current[zmc->GetID()]*temp;
	}
	return efield;
}

Vector3cd Core::Request::FarField::EFieldBenchMark(const double theta, const double phi,
	VectorXcd& current,Solution::ArrayCurrent* sol) const
{
	const Vector3d ob(Radius*cos(phi)*sin(theta), Radius*sin(phi)*sin(theta), Radius*cos(theta));
	Vector3cd efield{ 0,0,0 };
	auto _green = IGreen::GetInstance();

	for (auto bf = _bf->begin(), ed = _bf->end(); bf != ed; ++bf)//迭代器部分估计会有问题
	{
		auto zmc = static_cast<RWG*>(*bf);
		const short K = 4;
		Vector3cd subEfield{ 0,0,0 };

		RWGTriangle* tplus = zmc->TrianglePlus();
		RWGTriangle* tminus = zmc->TriangleMinus();

		for (Matrix<int, 2, 1, 0> loc : sol->_arrayLocation)
		{
			Vector3d Rb{ loc.x()*sol->_arrayBiasX,loc.y()*sol->_arrayBiasY,0 };
			Vector3d ArrayOb = ob - Rb;
			Vector3cd temp{ 0,0,0 };
			for (int i = 0; i < K; ++i)
			{//Source Triangle
				Vector3d pt1 = tplus->Quad4()[i], pt2 = tminus->Quad4()[i];
				Vector3cd eplus = -1i*Omega*Mu*_green->Scalar(pt1, ArrayOb)*zmc->CurrentPlus(pt1) +
					1i / (Omega*Epsilon)*zmc->ChargePlus(pt1)*_green->Gradient(pt1, ArrayOb);
				Vector3cd eminus = -1i*Omega*Mu*_green->Scalar(pt2, ArrayOb)*zmc->CurrentMinus(pt2) +
					1i / (Omega*Epsilon)*zmc->ChargeMinus(pt2)*_green->Gradient(pt2, ArrayOb);
				temp += W4[i] * eplus* tplus->Area() + W4[i] * eminus* tminus->Area();
			}
			subEfield += current[zmc->GetID()] *temp;
		}

		efield += subEfield;
	}
	return efield;
}