#include "stdafx.h"
#include "MultipoleExpansioner.h"
#include <unsupported/Eigen/KroneckerProduct>

using AIMAssist::MultipoleExpansioner;

void MultipoleExpansioner::Reset(const ImpConfiguration & configuration, double w7[7])
{
	_boxStart = configuration.Box[0];
	_dimension = configuration.Dimension;
	_interval = configuration.Interval;
	_layer[2] = configuration.zNumber;
	_layer[1] = configuration.yNumber;
	_layer[0] = configuration.xNumber;
	_order = configuration.GridOrder;
	_gridNum = _dimension == 2 ? (_order + 1)*(_order + 1) : (_order + 1)*(_order + 1)*(_order + 1);
	W7=w7;
	_index.resize(_gridNum);
}

MatrixXd MultipoleExpansioner::VanderInverse(const double bfCentreX, const double localBoxStartPointX) const
{
	const size_t o1 = _order + 1;
	MatrixXd vandermon(o1, o1);
	VectorXd epsilonX(o1);
	epsilonX.setLinSpaced(0, _order*_interval);
	epsilonX.array() += localBoxStartPointX - bfCentreX;
	//Set Vandermon
	for (size_t row = 0; row <o1; ++row)
		vandermon.row(row) = epsilonX.array().pow(row);
	return  vandermon.inverse();
}

MatrixXd MultipoleExpansioner::MultiExpansionRight(RWG * bf) const
{
	MatrixXd rightQ(_gridNum, _dimension + 1);
	rightQ.setZero();

	Matrix<Vector3d, 1, 7> epPlus = bf->TrianglePlus().Quad7().array() - bf->Centre();
	Matrix<Vector3d, 1, 7> epMinus = bf->TriangleMinus().Quad7().array() - bf->Centre();
	Matrix<Vector3d, 1, 7> rhoplus = bf->TrianglePlus().Quad7().array() - bf->Node(0),
		rhominus = bf->TriangleMinus().Quad7().array() - bf->Node(1);
	int count = 0;
	if (_dimension==3)
	{
		for (auto mz = 0; mz < _order + 1; ++mz)
		{
			for (auto my = 0; my < _order + 1; ++my)
			{
				for (auto mx = 0; mx < _order + 1; ++mx)
				{
					VectorXd tempPlus(7), tempMinus(7);
					Vector3d temp(0, 0, 0);
					for (int i = 0; i < 7; ++i)
					{
						tempPlus[i] = pow(epPlus[i].x(), mx)*pow(epPlus[i].y(), my)*pow(epPlus[i].z(), mz)*W7[i];
						tempMinus[i] = pow(epMinus[i].x(), mx)*pow(epMinus[i].y(), my)*pow(epMinus[i].z(), mz)*W7[i];
						temp += tempPlus[i] * rhoplus[i] - tempMinus[i] * rhominus[i];
					}
					//auto temp1 = (tempPlus.array()*rhoplus.array()).sum()-(tempMinus.array()*rhominus.array()).sum();
					//rightQ(count, Dimension) = tempMinus.sum();//for debug coefficients should be one
					rightQ(count, _dimension) = (tempPlus.sum() - tempMinus.sum())*bf->Length();
					rightQ.block(count, 0, 1, _dimension) = 0.5*bf->Length()*temp.transpose();
					++count;
				}
			}
		}
		return rightQ;
	}
	else
	{
		for (auto my = 0; my < _order + 1; ++my)
		{
			for (auto mx = 0; mx < _order + 1; ++mx)
			{
				VectorXd tempPlus(7), tempMinus(7);
				Vector3d temp(0, 0, 0);
				for (int i = 0; i < 7; ++i)
				{
					tempPlus[i] = pow(epPlus[i].x(), mx)*pow(epPlus[i].y(), my)*W7[i];
					tempMinus[i] = pow(epMinus[i].x(), mx)*pow(epMinus[i].y(), my)*W7[i];
					temp += tempPlus[i] * rhoplus[i] - tempMinus[i] * rhominus[i];
				}
				rightQ(count, _dimension) = (tempPlus.sum() - tempMinus.sum())*bf->Length();
				rightQ.block(count, 0, 1, _dimension) = 0.5*bf->Length()*temp.head<2>().transpose();
				++count;
			}
		}
		return rightQ;
	}

}

Vector3d MultipoleExpansioner::SearchGrids(const Vector3d & centre)
{
	// 求解离待求点最近的格点;
	if (_order > 4)throw spd::spdlog_ex("Order {} more than 4 is not Developing",(int)_order);

	Vector3d localCentre((centre - _boxStart) / _interval);
	
	Vector3d localStartIndex = _order%2?
		localStartIndex = floor(localCentre.array() - _order / 2):
		localStartIndex = round(localCentre.array() - _order / 2);

	const size_t xStartIndex=size_t(localStartIndex.x()),
	yStartIndex= size_t(localStartIndex.y()),
	zStartIndex= size_t(localStartIndex.z()),
	xEnd = xStartIndex + _order + 1, 
	yEnd = yStartIndex + _order + 1, 
	zEnd = zStartIndex + _order + 1;

	const size_t weight1 = _layer[0], weigth2 = _layer[1] * _layer[0];
	int* pgrids = _index.data();
	for (auto zzmc = zStartIndex; zzmc<zEnd; ++zzmc)
	{
		for (auto yzmc = yStartIndex; yzmc < yEnd; ++yzmc)
		{
			for (auto xzmc = xStartIndex; xzmc < xEnd; ++xzmc)
			{
				*pgrids++ = zzmc*weigth2 + yzmc*weight1 + xzmc;
			}
		}
	}
	return _boxStart + _interval*localStartIndex;
}

Vector2d MultipoleExpansioner::SearchGrids(const Vector2d & centre)
{
	// 求解离待求点最近的格点;
	if (_order > 4)throw spd::spdlog_ex("Order {} more than 4 is not Developing", (int)_order);

	Vector2d localCentre((centre - _boxStart.head<2>()) / _interval);

	Vector2d localStartIndex = _order % 2 ?
		localStartIndex = floor(localCentre.array() - _order / 2) :
		localStartIndex = round(localCentre.array() - _order / 2);

	const size_t xStartIndex = size_t(localStartIndex.x()),
		yStartIndex = size_t(localStartIndex.y()),
		xEnd = xStartIndex + _order + 1,
		yEnd = yStartIndex + _order + 1;

	const size_t weight1 = _layer[0];
	int* pgrids = _index.data();

	for (auto yzmc = yStartIndex; yzmc < yEnd; ++yzmc)
	{
		for (auto xzmc = xStartIndex; xzmc < xEnd; ++xzmc)
		{
			*pgrids++ = yzmc*weight1 + xzmc;
		}
	}
	return _boxStart.head<2>() + _interval*localStartIndex;
}

MatrixXd MultipoleExpansioner::operator()(RWG * bf)
{
	Vector3d centre(bf->Centre());

	if (_dimension == 3)
	{
		Vector3d localStart = SearchGrids(centre);

		const MatrixXd rightQ = MultiExpansionRight(bf);
		const MatrixXd s1 = VanderInverse(centre.x(), localStart.x());
		const MatrixXd s2 = VanderInverse(centre.y(), localStart.y());
		//作为整块的矩阵放在后面
		const MatrixXd s12 = kroneckerProduct(s2, s1);//这里需要矩阵直积s1Xs2
		const MatrixXd s3 = VanderInverse(centre.z(), localStart.z());
		const MatrixXd s123 = kroneckerProduct(s3, s12);
		return s123*rightQ;//coefficients
	}
	Vector2d localStart = SearchGrids(Vector2d{ centre.head(2) });

	const MatrixXd rightQ = MultiExpansionRight(bf);
	const MatrixXd s1 = VanderInverse(centre.x(), localStart.x());
	const MatrixXd s2 = VanderInverse(centre.y(), localStart.y());
	//作为整块的矩阵放在后面
	const MatrixXd s12 = kroneckerProduct(s2, s1);//这里需要矩阵直积s1Xs2
	return s12*rightQ;
}

