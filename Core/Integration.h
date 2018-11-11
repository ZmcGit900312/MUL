///////////////////////////////////////////////////////////
//  Integration.h
//  Implementation of the Interface IIntegration
//  Created on:      24-4月-2018 17:23:46
//  Original author: ZhengMX
///////////////////////////////////////////////////////////

#pragma once
#include "Triangle.h"
#include <Eigen/Core>
#include "Const.h"
using namespace Eigen;
using namespace Core;
namespace Core
{

	static double const Tri4[4][4] = { 0.333333333333333,0.333333333333333,0.333333333333333,-0.5625,
		0.6,0.2,0.2,0.520833333333333,
		0.2,0.6,0.2,0.520833333333333,
		0.2,0.2,0.6,0.520833333333333 };
	static double const Tri7[7][4] = { 0.333333333333333, 0.333333333333333, 0.333333333333333, 0.225,
		0.059715871789770, 0.470142064105115, 0.470142064105115, 0.132394152788506,
		0.470142064105115, 0.059715871789770, 0.470142064105115, 0.132394152788506,
		0.470142064105115, 0.470142064105115, 0.059715871789770, 0.132394152788506,
		0.797426985353087, 0.101286507323456, 0.101286507323456, 0.125939180544827,
		0.101286507323456, 0.797426985353087, 0.101286507323456, 0.125939180544827,
		0.101286507323456, 0.101286507323456, 0.797426985353087, 0.125939180544827 };
	static double const Tri13[13][4] = { 0.333333333333333,0.333333333333333,0.333333333333333,-0.149570044467670,
		0.479308067841923,0.260345966079038,0.260345966079038,0.175615257433204,
		0.260345966079038,0.479308067841923,0.260345966079038,0.175615257433204,
		0.260345966079038,0.260345966079038,0.479308067841923,0.175615257433204,
		0.869739794195568,0.065130102902216,0.065130102902216,0.053347235608839,
		0.065130102902216,0.869739794195568,0.065130102902216,0.053347235608839,
		0.065130102902216,0.065130102902216,0.869739794195568,0.053347235608839,
		0.638444188569809,0.312865496004875,0.048690315425315,0.077113760890257,
		0.048690315425315,0.638444188569809,0.312865496004875,0.077113760890257,
		0.312865496004875,0.048690315425315,0.638444188569809,0.077113760890257,
		0.638444188569809,0.048690315425315,0.312865496004875,0.077113760890257,
		0.048690315425315,0.312865496004875,0.638444188569809,0.077113760890257,
		0.312865496004875,0.638444188569809,0.048690315425315,0.077113760890257 };

	

	template<typename r1, typename r2>
	struct IntegrationTriangleKernel
	{
		typedef r1 return_type1;
		typedef r2 return_type2;
	};

	/**
	 * \brief 积分模版的类型萃取器
	 * 所有的积分核构造，必须拥有return_type1和return_type2;
	 * \tparam K 
	 */
	template<class K>
	struct KernelTraits
	{
		typedef typename K::return_type1 return_type1;
		typedef typename K::return_type1 return_type2;
	};


	template<class Kernel>
	class IntegrationTriangle
	{
		typedef typename KernelTraits<Kernel>::return_type1 return_type1;
		typedef typename KernelTraits<Kernel>::return_type2 return_type2;
	public:
		IntegrationTriangle(){}
		~IntegrationTriangle() {}
		

		/**
		* \SingleIntegration
		* Just like complex y=f(r),r is Vector3d
		* \param func
		* \param region
		* \return
		*/
		static return_type1 SingleIntegration(Kernel* func, Triangle region) {
			return_type1 re = return_type1();
			//Matrix3d vertex
			MatrixXd qPoints = region.Vertex()*Quad.block(0, 0, 3, NodeNum);
			for (auto i = 0; i<NodeNum; ++i)re = re+ Quad(3,i)* (*func)(qPoints.col(i));
			return region.Area()*re;
		}

		/**
		* \DoubleIntegrtion
		* like complex y=G(r1,r2),where r1 and r2 are Vector3d
		* \param func
		* \param outer
		* \param inner
		* \return
		*/
		static return_type2 DoubleIntegration(Kernel* func, Triangle inner, Triangle outer) {
			return_type2 re = return_type2(0);
			//Matrix3d vertex
			MatrixXd qPointsOuter = outer.Vertex()*Quad.block(0, 0, 3, NodeNum);
			MatrixXd qPointsInner = inner.Vertex()*Quad.block(0, 0, 3, NodeNum);

			/*MatrixXd funcResult(_quad.cols(), _quad.cols());
			for (int i = 0; i < int(NodeNum); ++i)
			for (int j = 0; j < int(NodeNum); ++j)
			funcResult(i,j)=func(qPointsOuter.col(i), qPointsInner.col(j));
			VectorXd temp=_quad.row(4)*funcResult;
			re = temp.dot(_quad.row(4))*/

			for (auto i = 0; i<NodeNum; ++i)
			{
				return_type2 temp = return_type2(0);
				for (auto j = 0; j<NodeNum; ++j)
				{
					temp += Quad(3, j)*(*func)(qPointsOuter.col(i), qPointsInner.col(j));
				}
				re += Quad(3, i)*temp;
			}
			return outer.Area()*inner.Area()*re;
		}

		static void SetQuad(const int node)
		{
			NodeNum = node;
			switch (node)
			{
			case 7:
				for (int col = 0; col < 7; ++col)
					for (int row = 0; row < 4; ++row)
						Quad(row, col) = Tri7[col][row];
				return;
			case 13:
				for (int col = 0; col < 13; ++col)
					for (int row = 0; row < 4; ++row)
						Quad(row, col) = Tri13[col][row];
				return;
			default:
				for (int col = 0; col < 4; ++col)
					for (int row = 0; row < 4; ++row)
						Quad(row, col) = Tri4[col][row];
			}
		}
	private:
		static int NodeNum;
		static Matrix<double, 4, 13> Quad;
	};
	
	template<typename Kernel>
	int IntegrationTriangle<Kernel>::NodeNum = 4;
	template<typename Kernel>
	Matrix<double, 4, 13> IntegrationTriangle<Kernel>::Quad;
	
}

