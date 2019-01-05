#include "stdafx.h"
#define _USE_MATH_DEFINES
#include "IntegrationRWG.h"
#include "Const.h"

Core::EFRImp::EFRImp(const double k, double * w4, double * w7, const double eta):
_k(k),_eta(eta),_w4(w4),_w7(w7){}


//填充三角形的自耦项
void EFRImp::SetSelfImpedanceTriangle(RWGTriangle * t, double w[13], const double k, const double eta)
{
	const short K = 13;
	//self impedance of unsingular 
	//at most six different combination for one triangle
	MatrixXcd Z11(K,K),Z22(K,K),Z33(K,K), Z12(K,K), Z13(K,K), Z23(K,K);
	VectorXcd Zs11(K), Zs22(K), Zs33(K), Zs12(K), Zs13(K), Zs23(K);
	const double Phi = 4.0 / (k*k);
	//Triangle t and its quad7  t.Node(0) t.Node(1)) t.Node(2);

	//Unit Vector of three lengths
	const Vector3d l1(t->Edge(0).first / t->Edge(0).second);
	const Vector3d l2(t->Edge(1).first / t->Edge(1).second);
	const Vector3d l3(t->Edge(2).first / t->Edge(2).second);

	//Unsingular part of Integration 1/R for the specialfic Triangle t
	for (short i = 0; i<K; ++i)
	{
		Vector3d rho1i(t->Quad13()[i] - t->Node(0)), rho2i(t->Quad13()[i] - t->Node(1)), rho3i(t->Quad13()[i] - t->Node(2));

		for (short j = i+1; j<K; ++j)
		{
			Vector3d rho1j(t->Quad13()[j] - t->Node(0)), rho2j(t->Quad13()[j] - t->Node(1)), rho3j(t->Quad13()[j] - t->Node(2));
			double R = (t->Quad13()[i] - t->Quad13()[j]).norm();
			dcomplex g = -0.5*k*k*R + 1i*k / 6.0*(k*k*R*R - 6);

			//Set at most Six Impedance
			Z11(i,j) = w[i] * w[j] * (rho1i.dot(rho1j) - Phi)*g;
			Z11(j,i) = Z11(i,j);
			Z22(i,j) = w[i] * w[j] * (rho2i.dot(rho2j) - Phi)*g;
			Z22(j,i) = Z22(i,j);
			Z33(i,j) = w[i] * w[j] * (rho3i.dot(rho3j) - Phi)*g;
			Z33(j,i) = Z33(i,j);
			Z12(i,j) = w[i] * w[j] * (rho1i.dot(rho2j) - Phi)*g;
			Z12(j,i) = Z12(i,j);
			Z13(i,j) = w[i] * w[j] * (rho1i.dot(rho3j) - Phi)*g;
			Z13(j,i) = Z13(i,j);
			Z23(i,j) = w[i] * w[j] * (rho2i.dot(rho3j) - Phi)*g;
			Z23(j,i) = Z23(i,j);
		}
		//i=j
		{
			Z11(i, i) = -1i*k*w[i] * w[i] * (rho1i.dot(rho1i) - Phi);
			Z22(i, i) = -1i*k*w[i] * w[i] * (rho2i.dot(rho2i) - Phi);
			Z33(i, i) = -1i*k*w[i] * w[i] * (rho3i.dot(rho3i) - Phi);
			Z12(i, i) = -1i*k*w[i] * w[i] * (rho1i.dot(rho2i) - Phi);
			Z13(i, i) = -1i*k*w[i] * w[i] * (rho1i.dot(rho3i) - Phi);
			Z23(i, i) = -1i*k*w[i] * w[i] * (rho2i.dot(rho3i) - Phi);
		}
		//Singular part of Integration 1/R for the specialfic Triangle t;
		Vector3d r1(-rho1i), r2(-rho2i), r3(-rho3i);
		//Rp1=Rm2=r3.norm(),Rm1=Rp3=r2.norm(),Rp2=Rm3=r1.norm()
		double Rp1 = r3.norm(), Rm1 = r2.norm(), Rp2 = r1.norm();
		double Lp1 = r3.dot(l1), Lm1 = r2.dot(l1);
		double Lp2 = r1.dot(l2), Lm2 = r3.dot(l2);
		double Lp3 = r2.dot(l3), Lm3 = r1.dot(l3);
		//The lengths from the quadrature to three edges
		double P1 = (r2 - Lm1*l1).norm(), P2 = (r3 - Lm2*l2).norm(), P3 = (r1 - Lm3*l3).norm();
		double gSingular = P1*log((Rp1 + Lp1) / (Rm1 + Lm1)) + P2*log((Rp2 + Lp2) / (Rp1 + Lm2)) + P3*log((Rm1 + Lp3) / (Rp2 + Lm3));

		gSingular *= w[i] / t->Area();
		Zs11[i] = (rho1i.dot(t->Centre() - t->Node(0)) - Phi)*gSingular;
		Zs22[i] = (rho2i.dot(t->Centre() - t->Node(1)) - Phi)*gSingular;
		Zs33[i] = (rho3i.dot(t->Centre() - t->Node(2)) - Phi)*gSingular;
		Zs12[i] = (rho1i.dot(t->Centre() - t->Node(1)) - Phi)*gSingular;
		Zs13[i] = (rho1i.dot(t->Centre() - t->Node(2)) - Phi)*gSingular;
		Zs23[i] = (rho2i.dot(t->Centre() - t->Node(2)) - Phi)*gSingular;

	}
	//1/(4*PI*4*A1*A2)
	const dcomplex coef = 1i / 16.0*k*eta*M_1_PI;
	const double edge0 = t->RWGSign[0] * t->Edge(0).second, edge1 = t->RWGSign[1] * t->Edge(1).second,
		edge2 = t->RWGSign[2] * t->Edge(2).second;
	t->Z(0) = coef *(Z11.sum() + Zs11.sum())*edge0*edge0;//*edge0^2
	t->Z(1) = coef *(Z22.sum() + Zs22.sum() )*edge1 *edge1;//*edge1^2
	t->Z(2) = coef *(Z33.sum() + Zs33.sum())*edge2*edge2;//*edge2^2
	t->Z(3) = coef *(Z12.sum() + Zs12.sum())*edge0*edge1;//*edge0*edge1
	t->Z(5) = coef *(Z23.sum() + Zs23.sum())*edge1 *edge2;//*edge1*edge2
	t->Z(4) = coef *(Z13.sum() + Zs13.sum())*edge0*edge2;//*edge0*edge2	

}

dcomplex EFRImp::SetImpedance(RWG * field, RWG * source)const
{
	dcomplex zpp, zpm, zmp, zmm;

	if(field->LimitPlus()!=source->LimitPlus())
	{
		zpp = 1i*_k*_eta*M_1_PI*field->Length()*source->Length()*
			UnsingularRWGIntegration(field->TrianglePlus(), source->TrianglePlus(), field->Node(0), source->Node(0), _w4, _k);
	}
	else
	{
		zpp = field->TrianglePlus()->Z(field->GetID(), source->GetID());
	}

	if (field->LimitPlus() != source->LimitMinus())
	{
		zpm = -1i*_k*_eta*M_1_PI*field->Length()*source->Length()*
			UnsingularRWGIntegration(field->TrianglePlus(), source->TriangleMinus(), field->Node(0), source->Node(1), _w4, _k);
	}
	else
	{
		zpm = field->TrianglePlus()->Z(field->GetID(), source->GetID());
	}

	if (field->LimitMinus() != source->LimitPlus())
	{
		zmp = -1i*_k*_eta*M_1_PI*field->Length()*source->Length()*
			UnsingularRWGIntegration(field->TriangleMinus(), source->TrianglePlus(), field->Node(1), source->Node(0), _w4, _k);
	}
	else
	{
		zmp = field->TriangleMinus()->Z(field->GetID(), source->GetID());
	}

	if (field->LimitMinus() != source->LimitMinus())
	{
		zmm = 1i*_k*_eta*M_1_PI*field->Length()*source->Length()*
			UnsingularRWGIntegration(field->TriangleMinus(), source->TriangleMinus(), field->Node(1), source->Node(1), _w4, _k);
	}
	else
	{
		zmm = field->TriangleMinus()->Z(field->GetID(), source->GetID());
	}


	return zpp+zpm+zmp+zmm;
}

list<element> EFRImp::SetImpedance(RWGTriangle * t) const
{
	list<element> Z;

	if (t->RWGSign[0])Z.push_back({ t->RWGID(0),t->RWGID(0),t->Z(0) });
	if (t->RWGSign[1])Z.push_back({ t->RWGID(1),t->RWGID(1),t->Z(1) });
	if (t->RWGSign[2])Z.push_back({ t->RWGID(2),t->RWGID(2),t->Z(2) });
	if (t->RWGSign[0] * t->RWGSign[1])Z.push_back({ t->RWGID(0),t->RWGID(1),t->Z(3) });
	if (t->RWGSign[0] * t->RWGSign[2])Z.push_back({ t->RWGID(0),t->RWGID(2),t->Z(4) });
	if (t->RWGSign[1] * t->RWGSign[2])Z.push_back({ t->RWGID(1),t->RWGID(2),t->Z(5) });
	
	return Z;
}

list<element> EFRImp::SetImpedance(RWGTriangle* field, RWGTriangle* source) const
{
	return UnsingularTriangleIntegration(field,source,_w4,_k,_eta);
}

void EFRImp::SetImpedance(RWGTriangle* field, RWGTriangle* source, list<element>& val) const
{
	const short K = 4;

	dcomplex m1 = 0, m4 = 0;
	Vector3cd m2{ 0,0,0 }, m3{ 0,0,0 };
	//Three vertexes of field and source triangle

	const double Phi = 4.0 / (_k*_k);

	for (int i = 0; i<K; ++i)
	{//Field Triangle
		for (int j = 0; j<K; ++j)
		{//Source Triangle
			double R = (field->Quad4()[i] - source->Quad4()[j]).norm();
			const dcomplex gij = _w4[i] * _w4[j] * exp(-1i*_k*R) / R;

			m4 += gij;
			m1 += field->Quad4()[i].dot(source->Quad4()[j])*gij;
			m2 += gij*field->Quad4()[i];
			m3 += gij*source->Quad4()[j];
		}

	}

	for (auto i = val.begin(); i != val.end(); ++i)
	{
		int& local1 = std::get<0>(*i), &local2 = std::get<1>(*i);
		const dcomplex value = 1i*0.0625*_k*_eta*M_1_PI*(m1 -
			field->Node(local1).dot(m3) - source->Node(local2).dot(m2) +
			(field->Node(local1).dot(source->Node(local2)) - Phi)*m4)
		*field->Edge(local1).second* source->Edge(local2).second;
		std::get<2>(*i) *= value;
		local1 = field->RWGID(local1);
		local2 = source->RWGID(local2);
	}
}

dcomplex EFRImp::SetRightHand(RWG * source, Vector3d ki, Vector3d e)
{
	const short K = 4;
	complex<double> plus(0),minus(0);
	RWGTriangle* tplus = source->TrianglePlus(),* tminus = source->TriangleMinus();
	for (int i = 0; i < K; ++i)
	{//Source Triangle
		Vector3d pt1 = tplus->Quad4()[i],pt2=tminus->Quad4()[i];
		plus += _w4[i] * exp(-1i*_k*pt1.dot(ki))*e.dot(source->CurrentPlus(pt1));
		minus+= _w4[i] * exp(-1i*_k*pt2.dot(ki))*e.dot(source->CurrentMinus(pt2));
	}
	return plus*tplus->Area()+minus*tminus->Area();
}

Vector3cd Core::EFRImp::Radiation(RWGTriangle* source, Vector3d ob, dcomplex current[3])
{
	const short K = 4;
	Vector3cd efield{ 0,0,0 };
	//Free Space
	
	const double Phi = 2.0 / (_k*_k);
	const dcomplex coef = -1i*_k*_eta*0.125*M_1_PI;

	for (int i = 0; i < K; ++i)
	{
		Vector3d pt = source->Quad4()[i];
		Vector3d rv(ob - pt);
		double R = rv.norm(),trans=R*_k;
		//Green
		dcomplex gScalar = _w4[i]*exp(-1i*trans)/R;
		Vector3cd gGradient(rv*(dcomplex(1,trans)*gScalar)/(R*R));

		for (short j = 0;j < 3;j++)
		{
			if (source->RWGSign[j] == 0)continue;
			Vector3cd kernel((pt - source->Node(j))*gScalar - Phi * gGradient);
			efield += current[j] * source->Edge(j).second*(source->RWGSign[j] * kernel);
		}
	}
	return coef*efield;
}

dcomplex EFRImp::UnsingularRWGIntegration(RWGTriangle* field, RWGTriangle* source, const Vector3d fieldFreePt, const Vector3d sourceFreePt, double w[4], double k)
{
	const short K = 4;
	complex<double> Z(0);
	const double Phi = 4.0 / (k*k);

	for (int i = 0; i<K; ++i)
	{//Field Triangle
		Vector3d rho1(field->Quad4()[i] - fieldFreePt);//场三角形的RHO向量
		complex<double> temp(0);
		for (int j = 0; j<K; ++j)
		{//Source Triangle
			Vector3d rho2(source->Quad4()[j] - sourceFreePt);//源三角形的RHO向量
			double R = (field->Quad4()[i] - source->Quad4()[j]).norm();
			temp += w[j] * (rho1.dot(rho2) - Phi)*exp(-1i*k*R) / R;
		}
		Z += w[i] * temp;
	}
	return 0.0625*Z;
}

list<element> EFRImp::UnsingularTriangleIntegration(RWGTriangle* field, RWGTriangle* source, double w[4], const double k, const double eta)
{
	const short K = 4;

	//Temporary Value and Impedance
	list<element> Z;

	dcomplex m1 = 0, m4 = 0;
	Vector3cd m2{0,0,0}, m3{0,0,0};
	//Three vertexes of field and source triangle

	const double Phi = 4.0 / (k*k);

	for (int i = 0; i<K; ++i)
	{//Field Triangle
		for (int j = 0; j<K; ++j)
		{//Source Triangle
			double R = (field->Quad4()[i] - source->Quad4()[j]).norm();
			const dcomplex gij = w[i] * w[j] * exp(-1i*k*R) / R;

			m4 += gij;
			m1 += field->Quad4()[i].dot(source->Quad4()[j])*gij;
			m2 += gij*field->Quad4()[i];
			m3 += gij*source->Quad4()[j];
		}

	}

	for (short i = 0; i<3; i++)
	{
		for (short j = 0; j<3; j++)
		{
			const int coupleState =field->RWGSign[i]*source->RWGSign[j];
			if (coupleState == 0)continue;
			Z.push_back({ field->RWGID(i),source->RWGID(j),
				1i*0.0625*k*eta*M_1_PI*(m1 - field->Node(i).dot(m3) - source->Node(j).dot(m2) +
				(field->Node(i).dot(source->Node(j)) - Phi)*m4)*field->Edge(i).second* source->Edge(j).second });
			if (coupleState < 0)std::get<2>(Z.back()) = -std::get<2>(Z.back());
		}
	}
	return Z;
}
