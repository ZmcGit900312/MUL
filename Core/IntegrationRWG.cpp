#include "stdafx.h"
#define _USE_MATH_DEFINES
#include "IntegrationRWG.h"
#include "Const.h"
#include "Green.h"
#include <Eigen/Geometry>//包含cross操作

Core::RWGImpOperator::RWGImpOperator(const double k, double const w4[], double const w7[], const double eta) :
	_k(k), _eta(eta), _w4(w4), _w7(w7) {}


//填充三角形的自耦项
void RWGImpOperator::SetSelfTriangleOperatorL(RWGTriangle * t, const double w[13], const double k, const double eta)
{
	const short K = 13;
	//self impedance of unsingular 
	//at most six different combination for one triangle
	MatrixXcd Z00(K, K), Z11(K, K), Z22(K, K), Z01(K, K), Z02(K, K), Z12(K, K);
	VectorXcd Zs00(K), Zs11(K), Zs22(K), Zs01(K), Zs02(K), Zs12(K);
	const double Phi = 4.0 / (k*k);
	//Triangle t and its quad7  t.Node(0) t.Node(1)) t.Node(2);

	//Unit Vector of three lengths
	const Vector3d l1(t->Edge(0).first / t->Edge(0).second);
	const Vector3d l2(t->Edge(1).first / t->Edge(1).second);
	const Vector3d l3(t->Edge(2).first / t->Edge(2).second);

	//Unsingular part of Integration 1/R for the specialfic Triangle t
	for (short i = 0; i < K; ++i)
	{
		Vector3d rho1i(t->Quad13()[i] - t->Node(0)), rho2i(t->Quad13()[i] - t->Node(1)), rho3i(t->Quad13()[i] - t->Node(2));

		for (short j = i + 1; j < K; ++j)
		{
			Vector3d rho1j(t->Quad13()[j] - t->Node(0)), rho2j(t->Quad13()[j] - t->Node(1)), rho3j(t->Quad13()[j] - t->Node(2));
			double R = (t->Quad13()[i] - t->Quad13()[j]).norm();
			dcomplex g = -0.5*k*k*R + 1i*k / 6.0*(k*k*R*R - 6);
			const dcomplex wg = w[i] * w[j] * g;
			//Set at most Six Impedance wg=w[i] * w[j] * g
			Z00(i, j) = wg * (rho1i.dot(rho1j) - Phi);
			Z00(j, i) = Z00(i, j);
			Z11(i, j) = wg * (rho2i.dot(rho2j) - Phi);
			Z11(j, i) = Z11(i, j);
			Z22(i, j) = wg * (rho3i.dot(rho3j) - Phi);
			Z22(j, i) = Z22(i, j);
			Z01(i, j) = wg * (rho1i.dot(rho2j) - Phi);
			Z01(j, i) = Z01(i, j);
			Z02(i, j) = wg * (rho1i.dot(rho3j) - Phi);
			Z02(j, i) = Z02(i, j);
			Z12(i, j) = wg * (rho2i.dot(rho3j) - Phi);
			Z12(j, i) = Z12(i, j);
		}
		//i=j
		{
			const dcomplex wk{ 0, -k * w[i] * w[i] };//weight=-1i*k*w[i] * w[i]
			Z00(i, i) = wk * (rho1i.dot(rho1i) - Phi);
			Z11(i, i) = wk * (rho2i.dot(rho2i) - Phi);
			Z22(i, i) = wk * (rho3i.dot(rho3i) - Phi);
			Z01(i, i) = wk * (rho1i.dot(rho2i) - Phi);
			Z02(i, i) = wk * (rho1i.dot(rho3i) - Phi);
			Z12(i, i) = wk * (rho2i.dot(rho3i) - Phi);
		}
		//Singular part of Integration 1/R for the specialfic Triangle t;
		Vector3d r1(-rho1i), r2(-rho2i), r3(-rho3i);
		//Rp1=Rm2=r3.norm(),Rm1=Rp3=r2.norm(),Rp2=Rm3=r1.norm()
		double Rp1 = r3.norm(), Rm1 = r2.norm(), Rp2 = r1.norm();
		double Lp1 = r3.dot(l1), Lm1 = r2.dot(l1);
		double Lp2 = r1.dot(l2), Lm2 = r3.dot(l2);
		double Lp3 = r2.dot(l3), Lm3 = r1.dot(l3);
		//The lengths from the quadrature to three edges
		double P1 = (r2 - Lm1 * l1).norm(), P2 = (r3 - Lm2 * l2).norm(), P3 = (r1 - Lm3 * l3).norm();
		double gSingular = P1 * log((Rp1 + Lp1) / (Rm1 + Lm1)) + P2 * log((Rp2 + Lp2) / (Rp1 + Lm2)) + P3 * log((Rm1 + Lp3) / (Rp2 + Lm3));

		gSingular *= w[i] / t->Area();
		Zs00[i] = (rho1i.dot(t->Centre() - t->Node(0)) - Phi)*gSingular;
		Zs11[i] = (rho2i.dot(t->Centre() - t->Node(1)) - Phi)*gSingular;
		Zs22[i] = (rho3i.dot(t->Centre() - t->Node(2)) - Phi)*gSingular;
		Zs01[i] = (rho1i.dot(t->Centre() - t->Node(1)) - Phi)*gSingular;
		Zs02[i] = (rho1i.dot(t->Centre() - t->Node(2)) - Phi)*gSingular;
		Zs12[i] = (rho2i.dot(t->Centre() - t->Node(2)) - Phi)*gSingular;

	}
	//1/(4*PI*4*A1*A2)
	const dcomplex coef = 1i / 16.0*k*eta*M_1_PI;
	const double edge0 = t->RWGSign[0] * t->Edge(0).second, edge1 = t->RWGSign[1] * t->Edge(1).second,
		edge2 = t->RWGSign[2] * t->Edge(2).second;
	t->SetZ(coef *(Z00.sum() + Zs00.sum())*edge0*edge0, 0);//edge0^2
	t->SetZ(coef *(Z11.sum() + Zs11.sum())*edge1 *edge1, 1);//edge1 ^ 2
	t->SetZ(coef *(Z22.sum() + Zs22.sum())*edge2*edge2, 2);//edge2^2
	t->SetZ(coef *(Z01.sum() + Zs01.sum())*edge0*edge1, 0, 1);//edge0*edge1
	t->SetZ(coef *(Z02.sum() + Zs02.sum())*edge0*edge2, 0, 2);//edge0*edge2
	t->SetZ(coef *(Z12.sum() + Zs12.sum())*edge1 *edge2, 1, 2);//edge1*edge2

}

dcomplex RWGImpOperator::SetImpedanceL(RWG * field, RWG * source)const
{
	dcomplex zpp, zpm, zmp, zmm;

	if (field->LimitPlus() != source->LimitPlus())
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


	return zpp + zpm + zmp + zmm;
}

dcomplex Core::RWGImpOperator::SetImpedanceK(RWG * field, RWG * source) const
{
	dcomplex pp(0), pm(0), mp(0), mm(0);
	RWGTriangle* TFplus = field->TrianglePlus(), *TFminus = field->TriangleMinus(),
		*TSplus = source->TrianglePlus(), *TSminus = source->TriangleMinus();

	for (int i = 0; i < 4; ++i)
	{//Field Triangle

		Vector3d TFPlusQuad = TFplus->Quad4()[i], TFMinusQuad = TFminus->Quad4()[i];
		for (int j = 0; j < 4; ++j)
		{//Source Triangled
			Vector3d TSPlusQuad = TSplus->Quad4()[j], TSMinusQuad = TSminus->Quad4()[j];
			Vector3cd gpp = W4[i] * W4[j] * IGreen::GetInstance()->Gradient(TFPlusQuad, TSPlusQuad)*TFplus->Area()*TSplus->Area();
			Vector3cd gpm = W4[i] * W4[j] * IGreen::GetInstance()->Gradient(TFPlusQuad, TSMinusQuad)*TFplus->Area()*TSminus->Area();
			Vector3cd gmp = W4[i] * W4[j] * IGreen::GetInstance()->Gradient(TFMinusQuad, TSPlusQuad)*TFminus->Area()*TSplus->Area();
			Vector3cd gmm = W4[i] * W4[j] * IGreen::GetInstance()->Gradient(TFMinusQuad, TSMinusQuad)*TFminus->Area()*TSminus->Area();

			pp += source->CurrentPlus(TSPlusQuad).cross(field->CurrentPlus(TFPlusQuad)).dot(gpp);
			pm += source->CurrentMinus(TSMinusQuad).cross(field->CurrentPlus(TFPlusQuad)).dot(gpm);
			mp += source->CurrentPlus(TSPlusQuad).cross(field->CurrentMinus(TFMinusQuad)).dot(gmp);
			mm += source->CurrentMinus(TSMinusQuad).cross(field->CurrentMinus(TFMinusQuad)).dot(gmm);

		}
	}

	dcomplex res = pp + pm + mp + mm;
	return res;
}

vector<element> RWGImpOperator::OperatorL(RWGTriangle * t) const
{
	vector<element> Z;

	if (t->Rn[0])Z.push_back({ t->RWGID(0),t->RWGID(0),t->Z(t->RWGID(0)) });
	if (t->Rn[1])Z.push_back({ t->RWGID(1),t->RWGID(1),t->Z(t->RWGID(1)) });
	if (t->Rn[2])Z.push_back({ t->RWGID(2),t->RWGID(2),t->Z(t->RWGID(2)) });
	if (t->Rn[0] && t->Rn[1])Z.push_back({ t->RWGID(0),t->RWGID(1),t->Z(t->RWGID(0),t->RWGID(1)) });
	if (t->Rn[0] && t->Rn[2])Z.push_back({ t->RWGID(0),t->RWGID(2),t->Z(t->RWGID(0),t->RWGID(2)) });
	if (t->Rn[1] && t->Rn[2])Z.push_back({ t->RWGID(1),t->RWGID(2),t->Z(t->RWGID(1),t->RWGID(2)) });

	return Z;
}

void RWGImpOperator::OperatorL(RWGTriangle* field, RWGTriangle* source, vector<element>& val) const
{
	const short K = 4;

	dcomplex m1 = 0, m4 = 0;
	Vector3cd m2{ 0,0,0 }, m3{ 0,0,0 };
	//Three vertexes of field and source triangle

	const double Phi = 4.0 / (_k*_k);

	for (int i = 0; i < K; ++i)
	{//Field Triangle
		for (int j = 0; j < K; ++j)
		{//Source Triangle
			double R = (field->Quad4()[i] - source->Quad4()[j]).norm();
			const dcomplex gij = _w4[i] * _w4[j] * exp(-1i*_k*R) / R;

			m4 += gij;
			m1 += field->Quad4()[i].dot(source->Quad4()[j])*gij;
			m2 += gij * field->Quad4()[i];
			m3 += gij * source->Quad4()[j];
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

dcomplex RWGImpOperator::SetIncidentFieldVector(RWG * source, Vector3d ki, Vector3cd incfield) const
{
	const short K = 4;
	complex<double> plus(0), minus(0);
	RWGTriangle* tplus = source->TrianglePlus(), *tminus = source->TriangleMinus();
	for (int i = 0; i < K; ++i)
	{//Source Triangle
		Vector3d pt1 = tplus->Quad4()[i], pt2 = tminus->Quad4()[i];
		plus += _w4[i] * exp(-1i*_k*pt1.dot(ki))*source->CurrentPlus(pt1).dot(incfield);
		minus += _w4[i] * exp(-1i*_k*pt2.dot(ki))*source->CurrentMinus(pt2).dot(incfield);
	}
	return plus * tplus->Area() + minus * tminus->Area();
}

dcomplex Core::RWGImpOperator::SetIncidentFieldVector(RWG * source, Vector3d ki, Vector3cd efield, double alpha, double eta) const
{
	const short K = 4;
	complex<double> plus(0), minus(0);
	Vector3cd hfield(ki.cross(efield));
	Vector3cd field(alpha*efield + (1 - alpha)*hfield);
	RWGTriangle* tplus = source->TrianglePlus(), *tminus = source->TriangleMinus();
	for (int i = 0; i < K; ++i)
	{//Source Triangle
		Vector3d pt1 = tplus->Quad4()[i], pt2 = tminus->Quad4()[i];
		plus += _w4[i] * exp(-1i*_k*pt1.dot(ki))*source->CurrentPlus(pt1).dot(field);
		minus += _w4[i] * exp(-1i*_k*pt2.dot(ki))*source->CurrentMinus(pt2).dot(field);
	}
	return plus * tplus->Area() + minus * tminus->Area();

}

vector<element> Core::RWGImpOperator::OperatorIdentity(RWGTriangle * t) const
{
	const short K = 7;
	vector<element> res;
	dcomplex Z[3] = { dcomplex(0),dcomplex(0),dcomplex(0) };//Z12,Z13,Z23
	for (int i = 0; i < K; ++i)
	{
		Vector3d quad = t->Quad7()[i];
		Vector3d rho1(quad - t->Node(0)),
			rho2(quad - t->Node(1)), rho3(quad - t->Node(2));//场三角形的RHO向量
		Vector3d nrho1 = t->Normal().cross(rho1),
			nrho2 = t->Normal().cross(rho2);
		Z[0] += _w7[i] * nrho1.dot(rho2);
		Z[1] += _w7[i] * nrho1.dot(rho3);
		Z[2] += _w7[i] * nrho2.dot(rho3);
	}

	//only three 
	if (t->Rn[0] && t->Rn[1])
	{
		dcomplex v = 0.5*0.25*t->Edge(0).second*t->Edge(1).second*t->RWGSign[0] * t->RWGSign[1] * Z[0] / t->Area();
		res.push_back({ t->RWGID(0),t->RWGID(1),v });
		res.push_back({ t->RWGID(1),t->RWGID(0),-v });

	}
	if (t->Rn[0] && t->Rn[2])
	{
		dcomplex v = 0.5*0.25*t->Edge(0).second*t->Edge(2).second*t->RWGSign[0] * t->RWGSign[2] * Z[1] / t->Area();
		res.push_back({ t->RWGID(0),t->RWGID(2),v });
		res.push_back({ t->RWGID(2),t->RWGID(0),-v });
	}
	if (t->Rn[1] && t->Rn[2]) {
		dcomplex v = 0.5*0.25*t->Edge(1).second*t->Edge(2).second*t->RWGSign[1] * t->RWGSign[2] * Z[2] / t->Area();
		res.push_back({ t->RWGID(1),t->RWGID(2),v });
		res.push_back({ t->RWGID(2),t->RWGID(1),-v });
	}

	return res;
}

void Core::RWGImpOperator::OperatorK(RWGTriangle* field, RWGTriangle* source, vector<element>& val) const
{
	const short K = 4;

	dcomplex k1 = 0;
	Vector3cd k2{ 0,0,0 }, k3{ 0,0,0 }, k4{ 0,0,0 };
	//Three vertexes of field and source triangle

	for (int i = 0; i < K; ++i)
	{//Field Triangle
		for (int j = 0; j < K; ++j)
		{//Source Triangle
			Vector3cd gij = _w4[i] * _w4[j] * IGreen::GetInstance()->Gradient(field->Quad4()[i], source->Quad4()[j]);

			k4 += gij;
			k1 += source->Quad4()[j].cross(field->Quad4()[i]).dot(gij);
			k2 += source->Quad4()[j].cross(gij);
			k3 += gij.cross(field->Quad4()[i]);

		}

	}

	for (auto i = val.begin(); i != val.end(); ++i)
	{
		int& local1 = std::get<0>(*i), &local2 = std::get<1>(*i);
		const dcomplex value = 0.25*(
			k1 + k2.dot(field->Node(local1)) + k3.dot(source->Node(local2))
			+ source->Node(local2).cross(field->Node(local1)).dot(k4)
			)*field->Edge(local1).second* source->Edge(local2).second;
		std::get<2>(*i) *= value;
		local1 = field->RWGID(local1);
		local2 = source->RWGID(local2);
	}
}

vector<element> Core::RWGImpOperator::OperatorCPEC(RWGTriangle * t, double alpha, double eta) const
{
	vector<element> Z;
	Z.push_back({ t->RWGID(0),t->RWGID(0),alpha*t->Z(t->RWGID(0)) });
	Z.push_back({ t->RWGID(1),t->RWGID(1),alpha*t->Z(t->RWGID(1)) });
	Z.push_back({ t->RWGID(2),t->RWGID(2),alpha*t->Z(t->RWGID(2)) });

	const short K = 7;
	dcomplex ZI[3] = { dcomplex(0),dcomplex(0),dcomplex(0) };//Z12,Z13,Z23
	for (int i = 0; i < K; ++i)
	{
		Vector3d quad = t->Quad7()[i];
		Vector3d rho1(quad - t->Node(0)),
			rho2(quad - t->Node(1)), rho3(quad - t->Node(2));//场三角形的RHO向量
		Vector3d nrho1 = t->Normal().cross(rho1),
			nrho2 = t->Normal().cross(rho2);
		ZI[0] += _w7[i] * nrho1.dot(rho2);
		ZI[1] += _w7[i] * nrho1.dot(rho3);
		ZI[2] += _w7[i] * nrho2.dot(rho3);
	}

	dcomplex z01 = (1 - alpha)*eta*0.5*0.25*t->Edge(0).second*t->Edge(1).second*t->RWGSign[0] * t->RWGSign[1] * ZI[0] / t->Area();
	Z.push_back({ t->RWGID(0),t->RWGID(1),alpha*t->Z(t->RWGID(0),t->RWGID(1)) - z01 });
	Z.push_back({ t->RWGID(1),t->RWGID(0),alpha*t->Z(t->RWGID(0),t->RWGID(1)) + z01 });

	dcomplex z02 = (1 - alpha)*eta*0.5*0.25*t->Edge(0).second*t->Edge(2).second*t->RWGSign[0] * t->RWGSign[2] * ZI[1] / t->Area();
	Z.push_back({ t->RWGID(0),t->RWGID(2),alpha*t->Z(t->RWGID(0),t->RWGID(2)) - z02 });
	Z.push_back({ t->RWGID(2),t->RWGID(0),alpha*t->Z(t->RWGID(0),t->RWGID(2)) + z02 });

	dcomplex z12 = (1 - alpha)*eta*0.5*0.25*t->Edge(1).second*t->Edge(2).second*t->RWGSign[1] * t->RWGSign[2] * ZI[2] / t->Area();
	Z.push_back({ t->RWGID(1),t->RWGID(2),alpha*t->Z(t->RWGID(1),t->RWGID(2)) - z12 });
	Z.push_back({ t->RWGID(2),t->RWGID(1),alpha*t->Z(t->RWGID(2),t->RWGID(1)) + z12 });
	return Z;
}

void Core::RWGImpOperator::OperatorCPEC(RWGTriangle * field, RWGTriangle * source, vector<element>& val, double alpha, double eta) const
{
	const short K = 4;

	dcomplex m1 = 0, m4 = 0, k1 = 0;
	Vector3cd m2{ 0,0,0 }, m3{ 0,0,0 }, k2{ 0,0,0 }, k3{ 0,0,0 }, k4{ 0,0,0 };
	//Three vertexes of field and source triangle

	const double Phi = 4.0 / (_k*_k);

	for (int i = 0; i < K; ++i)
	{//Field Triangle
		for (int j = 0; j < K; ++j)
		{//Source Triangle
			dcomplex gscalar = _w4[i] * _w4[j] * IGreen::GetInstance()->Scalar(field->Quad4()[i], source->Quad4()[j]);

			Vector3cd gij = _w4[i] * _w4[j] * IGreen::GetInstance()->Gradient(field->Quad4()[i], source->Quad4()[j]);

			m4 += gscalar;
			m1 += field->Quad4()[i].dot(source->Quad4()[j])*gscalar;
			m2 += gscalar * field->Quad4()[i];
			m3 += gscalar * source->Quad4()[j];

			k4 += gij;
			k1 += source->Quad4()[j].cross(field->Quad4()[i]).dot(gij);
			k2 += source->Quad4()[j].cross(gij);
			k3 += gij.cross(field->Quad4()[i]);
		}

	}

	for (auto i = val.begin(); i != val.end(); ++i)
	{
		int& local1 = std::get<0>(*i), &local2 = std::get<1>(*i);

		double effE = alpha * 0.25*_k*_eta*field->Edge(local1).second* source->Edge(local2).second,
			effH = (1 - alpha)*eta*0.25*field->Edge(local1).second* source->Edge(local2).second;
		const dcomplex vL = (m1 -
			field->Node(local1).dot(m3) - source->Node(local2).dot(m2) +
			(field->Node(local1).dot(source->Node(local2)) - Phi)*m4)*effE * 1i;
		const dcomplex vK = effH * (
			k1 + k2.dot(field->Node(local1)) + k3.dot(source->Node(local2))
			+ source->Node(local2).cross(field->Node(local1)).dot(k4));
		std::get<2>(*i) *= vL + vK;
		local1 = field->RWGID(local1);
		local2 = source->RWGID(local2);
	}
}

Vector3cd Core::RWGImpOperator::OperatorKScatter(RWGTriangle * source, Vector3d ob, dcomplex current[3]) const
{
	const short K = 4;
	Vector3cd field{ 0,0,0 };
	//Free Space

	for (int i = 0; i < K; ++i)
	{
		Vector3d pt = source->Quad4()[i];
		//Green
		Vector3cd gGradient(_w4[i] * IGreen::GetInstance()->Gradient(ob, pt));

		for (short j = 0;j < 3;j++)
		{
			if (source->RWGSign[j] == 0)continue;
			Vector3cd kernel(-(pt - source->Node(j)).cross(gGradient));
			field += current[j] * source->Edge(j).second*(source->RWGSign[j] * kernel);
		}
	}
	return 0.5 * field;
}

Vector3cd Core::RWGImpOperator::OperatorLScatter(RWGTriangle* source, Vector3d ob, dcomplex current[3])const
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
		double R = rv.norm(), trans = R * _k;
		//Green
		dcomplex gScalar = _w4[i] * exp(-1i*trans) / R;
		Vector3cd gGradient(rv*(dcomplex(1, trans)*gScalar) / (R*R));

		for (short j = 0;j < 3;j++)
		{
			if (source->RWGSign[j] == 0)continue;
			Vector3cd kernel((pt - source->Node(j))*gScalar - Phi * gGradient);
			efield += current[j] * source->Edge(j).second*(source->RWGSign[j] * kernel);
		}
	}
	return coef * efield;
}

dcomplex RWGImpOperator::UnsingularRWGIntegration(RWGTriangle* field, RWGTriangle* source, const Vector3d fieldFreePt, const Vector3d sourceFreePt, double const* w, double k)
{
	const short K = 4;
	complex<double> Z(0);
	const double Phi = 4.0 / (k*k);

	for (int i = 0; i < K; ++i)
	{//Field Triangle
		Vector3d rho1(field->Quad4()[i] - fieldFreePt);//场三角形的RHO向量
		complex<double> temp(0);
		for (int j = 0; j < K; ++j)
		{//Source Triangle
			Vector3d rho2(source->Quad4()[j] - sourceFreePt);//源三角形的RHO向量
			double R = (field->Quad4()[i] - source->Quad4()[j]).norm();
			temp += w[j] * (rho1.dot(rho2) - Phi)*exp(-1i*k*R) / R;
		}
		Z += w[i] * temp;
	}
	return 0.0625*Z;
}

vector<element> RWGImpOperator::SetImpedanceL(RWGTriangle* field, RWGTriangle* source) const
{
	const short K = 4;

	//Temporary Value and Impedance
	vector<element> Z;

	dcomplex m1 = 0, m4 = 0;
	Vector3cd m2{ 0,0,0 }, m3{ 0,0,0 };
	//Three vertexes of field and source triangle

	const double Phi = 4.0 / (_k*_k);

	for (int i = 0; i < K; ++i)
	{//Field Triangle
		for (int j = 0; j < K; ++j)
		{//Source Triangle
			double R = (field->Quad4()[i] - source->Quad4()[j]).norm();
			const dcomplex gij = _w4[i] * _w4[j] * exp(-1i*_k*R) / R;

			m4 += gij;
			m1 += field->Quad4()[i].dot(source->Quad4()[j])*gij;
			m2 += gij * field->Quad4()[i];
			m3 += gij * source->Quad4()[j];
		}

	}

	for (short i = 0; i < 3; i++)
	{
		for (short j = 0; j < 3; j++)
		{
			const int coupleState = field->RWGSign[i] * source->RWGSign[j];
			if (coupleState == 0)continue;
			Z.push_back({ field->RWGID(i),source->RWGID(j),
				1i*0.0625*_k*_eta*M_1_PI*(m1 - field->Node(i).dot(m3) - source->Node(j).dot(m2) +
				(field->Node(i).dot(source->Node(j)) - Phi)*m4)*field->Edge(i).second* source->Edge(j).second });
			if (coupleState < 0)std::get<2>(Z.back()) = -std::get<2>(Z.back());
		}
	}
	return Z;
}
