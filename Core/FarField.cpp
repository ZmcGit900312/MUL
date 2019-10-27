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

//Static RCS Keep
/*
 * RCS =		\		Request1	Requst2		Request3
 *			Current1	RCS11		RCS12		RCS13
 *			Curernt2	RCS21		RCS22		RCS23
 *			Current3	RCS31		RCS32		RCS33
 */
Matrix<Request::FarField::RCSData, Dynamic, Dynamic> Request::FarField::RCS;

Request::FarField::FarField(vector<IBasisFunction*>*bf, Mesh *mesh, Solution::CurrentInfo* current) :
	_mesh(mesh), _bf(bf), _current(current)
{
	Vector3d D{ Assist::ModalGeometry::GetInstance()->GetLimitationBoundary(7) - Assist::ModalGeometry::GetInstance()->GetLimitationBoundary(0) };
	if(current->category==Core::Array)
	{
		Solution::ArrayCurrent* ac = static_cast<Solution::ArrayCurrent*>(current->Current[0]);
		int xNum = 0, yNum = 0;
		for(Vector2i loc:ac->_arrayLocation)
		{
			if (loc.x() > xNum)xNum = loc.x();
			if (loc.y() > yNum)yNum = loc.y();
		}

		Vector3d arrayScale{ ac->_arrayBiasX*xNum,ac->_arrayBiasY*yNum,0 };
		D += arrayScale;
	}
	Radius = 100000*D.squaredNorm() /Lambda;//2D^2/lam
	Coef = 4 * M_PI*Radius*Radius;
}

Core::Request::FarField::~FarField()
{
	_mesh = nullptr;
	_current = nullptr;
	_bf = nullptr;
}

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

	vector<MatrixXd> _RCS;
	_RCS.reserve(_current->_numberOfConfig);
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
		_RCS.push_back(rcs);
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
			for(auto mx:_RCS)
			{
				ofs << ',' << mx(th, ph);
			}			
			ofs << '\n';
		}
	}

}

void Core::Request::FarField::CalculateRCS(FarFieldConfiguration & config, int row, int col) const
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


	MatrixXd rcs{ thetaNum, phiNum };
	//Update Const
	if (_current->category == Core::Array)
	{
		for (int th = 0; th < thetaNum; ++th)
		{
			for (int ph = 0; ph < phiNum; ++ph)
			{
					Vector3cd temp{ EField(Theta(th)*M_PI_180, Phi(ph)*M_PI_180,
						static_cast<Solution::ArrayCurrent*>(_current->Current[row])) };
					/*Vector3cd temp{ EFieldDipole(Theta(th)*M_PI_180, Phi(ph)*M_PI_180,
							static_cast<Solution::ArrayCurrent*>(_current->Current[row])) };*/
					rcs(th, ph) = Coef * temp.squaredNorm();
					cout << "Progress:" << setw(10) << (th*phiNum + ph + 1) / Sum << "%\r";
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
						_current->Current[row]) };
					rcs(th, ph) = Coef * temp.squaredNorm();
					cout << "Progress:" << setw(10) << (th*phiNum + ph + 1) / Sum << "%\r";
			}
		}
	}

	RCS(row, col) = rcs;
}

void Core::Request::FarField::CalculateDipoleFF(FarFieldConfiguration & config, int row, int col) const
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

	MatrixXd rcs{ thetaNum, phiNum };
	//Update Const
	for (int th = 0; th < thetaNum; ++th)
	{
		for (int ph = 0; ph < phiNum; ++ph)
		{
			Vector3cd temp = _current->category == Core::Array ?
				EFieldDipole(Theta(th)*M_PI_180, Phi(ph)*M_PI_180,
						static_cast<Solution::ArrayCurrent*>(_current->Current[row])) :
				EFieldDipole(Theta(th)*M_PI_180, Phi(ph)*M_PI_180,_current->Current[row]);
			rcs(th, ph) = Coef * temp.squaredNorm();
			cout << "Progress:" << setw(10) << (th*phiNum + ph + 1) / Sum << "%\r";
		}
	}

	RCS(row, col) = config.FieldType == Request::Pattern?
		10 * Eigen::log10(rcs.array() / rcs.maxCoeff()):rcs;
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

		for(int eleId=0;eleId<sol->_numberOfElement;++eleId)
		{
			auto loc = sol->_arrayLocation[eleId];
			Vector3d Rb{ loc.x()*sol->_arrayBiasX,loc.y()*sol->_arrayBiasY,0 };
			current[0] = temp->RWGSign[0] ? sol->GetCurrent(temp->RWGID(0), eleId) : 0;
			current[1] = temp->RWGSign[1] ? sol->GetCurrent(temp->RWGID(1), eleId) : 0;
			current[2] = temp->RWGSign[2] ? sol->GetCurrent(temp->RWGID(2), eleId) : 0;
			subEfield += equation->LScatter(temp, observation - Rb, current);
		}

		//for (Matrix<int, 2, 1, 0> loc : sol->_arrayLocation)
		//{
		//	Vector3d Rb{ loc.x()*sol->_arrayBiasX,loc.y()*sol->_arrayBiasY,0 };
		//	current[0] = temp->RWGSign[0] ? sol->GetCurrent(temp->RWGID(0), loc.x(), loc.y()) : 0;
		//	current[1] = temp->RWGSign[1] ? sol->GetCurrent(temp->RWGID(1), loc.x(), loc.y()) : 0;
		//	current[2] = temp->RWGSign[2] ? sol->GetCurrent(temp->RWGID(2), loc.x(), loc.y()) : 0;
		//	subEfield += equation->LScatter(temp, observation - Rb, current);
		//}

		efield += subEfield;
	}

	return efield;
}

Vector3cd Core::Request::FarField::EFieldDipole(const double theta, const double phi, Solution::ElementCurrent * sol) const
{
	const Vector3d ob(Radius*cos(phi)*sin(theta), Radius*sin(phi)*sin(theta), Radius*cos(theta));

	Vector3cd efield{ 0,0,0 };

	for (auto bf = _bf->begin(), ed = _bf->end(); bf != ed; ++bf)
	{
		//计算单元因子
		auto zmc = static_cast<RWG*>(*bf);
		RWGTriangle* tplus = zmc->TrianglePlus();
		RWGTriangle* tminus = zmc->TriangleMinus();

		const Vector3d dipoleCenter = 0.5*(tminus->Centre() + tplus->Centre());
		const Vector3d dipoleMoment = zmc->Length()*(tminus->Centre() - tplus->Centre());

		Vector3cd E0 = DipoleEfiled(ob, dipoleMoment, dipoleCenter, eta0, k);

		//叠加电场
		efield += E0 * sol->GetCurrent(zmc->GetID());
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
	Solution::ArrayCurrent* sol) const
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

		for (int eleId = 0;eleId < sol->_numberOfElement;++eleId)
		{
			auto loc = sol->_arrayLocation[eleId];
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
			subEfield += sol->GetCurrent(zmc->GetID(),eleId) * temp;

		}
		efield += subEfield;
	}
	return efield;
}

Vector3cd Core::Request::FarField::EFieldDipole(const double theta, const double phi, Solution::ArrayCurrent * sol) const
{
	const Vector3d ob(Radius*cos(phi)*sin(theta), Radius*sin(phi)*sin(theta), Radius*cos(theta));
	
	Vector3cd efield{ 0,0,0 };

	for (auto bf = _bf->begin(), ed = _bf->end(); bf != ed; ++bf)
	{
		//计算单元因子
		auto zmc = static_cast<RWG*>(*bf);
		RWGTriangle* tplus = zmc->TrianglePlus();
		RWGTriangle* tminus = zmc->TriangleMinus();

		const Vector3d dipoleCenter =0.5*(tminus->Centre() + tplus->Centre());
		const Vector3d dipoleMoment = zmc->Length()*(tminus->Centre()-tplus->Centre());

		Vector3cd E0= DipoleEfiled(ob, dipoleMoment, dipoleCenter, eta0, k);

		//叠加电场
		efield += E0 * ArrayFactor(zmc->GetID(),theta,phi, sol);
	}
	return efield;
}

Vector3cd Core::Request::FarField::DipoleEfiled(const Vector3d ob, const Vector3d dipoleMoment, const Vector3d dipoleCenter, const double eta, const double k)const
{
	const Vector3d dis = ob - dipoleCenter;
	const double R = dis.stableNorm();
	const Vector3d direction = dis / R;

	const dcomplex C = (1.0 + 1.0 / (1i*k*R)) / (R*R);
	//efield
	Vector3d M = direction.dot(dipoleMoment)*direction;	
	Vector3cd efield0 = (M - dipoleMoment)*(1i*k / R + C) + 2.0*C*M;
	const dcomplex ConstantE = 0.25 * eta*M_1_PI*exp(-1i*k*R); 
	efield0 *= ConstantE;
	//hfield
	/*Vector3cd hfield0 = dipoleMoment.cross(dis)*C;
	const dcomplex ConstantH = 0.25*1i *k *M_1_PI*exp(-1i*k*R);
	hfield0 *= ConstantH;*/
	return efield0;
}

dcomplex Core::Request::FarField::ArrayFactor(const size_t id, const double theta, const double phi, Solution::ArrayCurrent * sol) const
{
	dcomplex af(0);
	for (int eleId = 0;eleId < sol->_numberOfElement;++eleId)
	{
		auto loc = sol->_arrayLocation[eleId];
		Vector3d Rb{ loc.x()*sol->_arrayBiasX,loc.y()*sol->_arrayBiasY,0 };

		double d = -(loc.x()*sol->_arrayBiasX*cos(phi) + loc.y()*sol->_arrayBiasY*sin(phi))*sin(theta);
		af += sol->GetCurrent(id, eleId)*exp(-1i*k*d);
	}
	return af;
}

void Core::Request::FarField::SaveRCS(ofstream & ofs, FarFieldConfiguration & config, int zmc)
{
	const int thetaNum = config.ThetaNum;
	const int phiNum = config.PhiNum;
	const double thetaS = config.ThetaStart;
	const double phiS = config.PhiStart;
	const double thetaI = config.ThetaIncrement;
	const double phiI = config.PhiIncrement;


	VectorXd Theta{ VectorXd::LinSpaced(thetaNum,thetaS,thetaS + thetaI * (thetaNum - 1)) };
	VectorXd Phi{ VectorXd::LinSpaced(phiNum,phiS,phiS + phiI * (phiNum - 1)) };


	for (int th = 0; th < thetaNum; ++th)
	{
		for (int ph = 0; ph < phiNum; ++ph)
		{
			ofs << Theta(th) << ',' << Phi(ph);

			for(int row=0;row<RCS.rows();++row)
			{
				ofs << ',' << RCS(row, zmc)(th, ph);
			}
			ofs << '\n';
		}
	}
}