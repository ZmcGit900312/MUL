///////////////////////////////////////////////////////////
//  Integral.h
//  Implementation of the Enumeration QuraNum
//  Created on:      18-12ÔÂ-2017 16:40:56
//  Original author: ZhengMX
///////////////////////////////////////////////////////////


#pragma once
#include<complex>
#include<Eigen\Core>


using namespace std;
using namespace Eigen;
namespace Core
{
	namespace Internal
	{
		enum QuraNum{P4 = 4,P7 = 7,P13 = 13};

#pragma region Base
		class IntegrationBase
		{
		public:
			IntegrationBase(){}
			virtual ~IntegrationBase() = default;
			virtual void SetNodeNum(const QuraNum newVal) = 0;
		};
#pragma endregion 

#pragma region Line
		template<typename Output=double>
		class IntegrationLine : public IntegrationBase
		{
			typedef Output(*Func)(const double);
			
		public:

			IntegrationLine(const QuraNum newVal = P4){ IntegrationLine<Output>::SetNodeNum(newVal);}

			~IntegrationLine() { qptr = nullptr; wptr = nullptr; }
			/**
			 * \SetNodeNum
			 * \param newVal 
			 */
			void SetNodeNum(const QuraNum newVal)override
			{
				NodeNum = newVal;
				switch (newVal)
				{
				case P7: qptr = _quad7; wptr = _w7; return;
				case P13:qptr = _quad13; wptr = _w13; return;
				default: qptr = _quad4; wptr = _w4;
				}
			}

			/**
			 * \Integration 
			 * \param func Just like double y=f(x)
			 * \param start 
			 * Input start value 
			 * \param end
			 * Input end value 
			 * \return double or complex
			 */
			virtual Output operator()(Func func, double start=-1.0, double end=1.0)
			{
				Output re = Output(0);
				const double lengthHalf = 0.5*(end - start),mid = 0.5*(start + end);
				auto qtemp = qptr, wtemp = wptr;
				for (int i = 0; i++ < NodeNum; ++qtemp,++wtemp)
				{
					double node = mid +*qtemp*lengthHalf;
					re += *wtemp*func(node);
				}
				return lengthHalf*re;
			}
		protected:
			static const double _quad4[4],_quad7[7];
			static const double _quad13[13];

			static const double _w4[4], _w7[7];
			static const double _w13[13];
			const double *qptr=nullptr,*wptr=nullptr ;
			QuraNum NodeNum;
		};

#pragma region QuadPoints
		const double IntegrationLine<>::_quad4[4] = { -.8611363116,-.3399810436,.3399810436,.8611363116 };
		const double IntegrationLine<>::_quad7[7] = { -.9491079123,-.7415311856,-.4058451514,0.0,
			.4058451514 ,.7415311856 ,.9491079123 };
		const double IntegrationLine<>::_quad13[13]= { -0.9841830547185880,-0.9175983992229770,-0.8015780907333090
			- 0.6423493394403400,-0.4484927510364460,-0.2304583159551340,0.0,0.2304583159551340,
			0.4484927510364460,0.6423493394403400,0.8015780907333090,0.9175983992229770,0.9841830547185880 };

		const double IntegrationLine<>::_w4[4] = { 0.3478548451,0.6521451459,0.6521451459,0.3478548451 };
		const double IntegrationLine<>::_w7[7] = { .1294849662, .2797053915, .3818300505, .4179591837,
			.3818300505, .2797053915, .1294849662 };
		const double IntegrationLine<>::_w13[13]= { 0.0404840047653158,0.0921214998377284,0.1388735102197870,
			0.1781459807619450,0.2078160475368880,0.2262831802628970,0.2325515532308730,0.2262831802628970,
			0.2078160475368880,0.1781459807619450,0.1388735102197870,0.0921214998377284,0.0404840047653158 };


		typedef IntegrationLine<complex<double>> IntegrationLinecd;

		const double IntegrationLine<complex<double>>::_quad4[4] = { -.8611363116,-.3399810436,.3399810436,.8611363116 };
		const double IntegrationLine<complex<double>>::_quad7[7] = { -.9491079123,-.7415311856,-.4058451514,0.0,
			.4058451514 ,.7415311856 ,.9491079123 };
		const double IntegrationLine<complex<double>>::_quad13[13] ={ -0.9841830547185880,-0.9175983992229770,-0.8015780907333090
			- 0.6423493394403400,-0.4484927510364460,-0.2304583159551340,0.0,0.2304583159551340,
			0.4484927510364460,0.6423493394403400,0.8015780907333090,0.9175983992229770,0.9841830547185880 };

		const double IntegrationLine<complex<double>>::_w4[4] = { 0.3478548451,0.6521451459,0.6521451459,0.3478548451 };
		const double IntegrationLine<complex<double>>::_w7[7] = { .1294849662, .2797053915, .3818300505, .4179591837,
			.3818300505, .2797053915, .1294849662 };
		const double IntegrationLine<complex<double>>::_w13[13] = { 0.0404840047653158,0.0921214998377284,0.1388735102197870,
			0.1781459807619450,0.2078160475368880,0.2262831802628970,0.2325515532308730,0.2262831802628970,
			0.2078160475368880,0.1781459807619450,0.1388735102197870,0.0921214998377284,0.0404840047653158 };
		//Global Data
		static IntegrationLine<> IntLined;
		static IntegrationLinecd IntLinecd;
#pragma endregion 

#pragma endregion 

#pragma region Square
		template<typename Scalar, typename Output=Scalar>
		class IntegrationSquare :public IntegrationLine<Scalar>
		{
			typedef Output(*Func)(const double[2]);
		public:
			IntegrationSquare(const QuraNum newVal = P4):IntegrationLine<Scalar>(newVal){}

			/**
			* \Square integration
			*
			* \param func 
			* For a instance, double z=f(x,y),which (x,y) means a point
			* \param start
			* Input start value of x and y, start[2]
			* \param end
			* Input end value of x and y, end[2]
			* \return Output is Scalar or Vector2
			*/
			Output operator()(Func func, double start[2], double end[2])
			{
				Output re = Output(0);
				double lengthHalf[2] = { 0.5*(end[0] - start[0]),0.5*(end[1] - start[1]) };
				double mid[2] = { 0.5*(end[0] + start[0]),0.5*(end[1] + start[1]) };
				const double* qx = this->qptr, *wx = this->wptr;
				for (int i = 0; i++ < this->NodeNum; ++qx, ++wx)
				{
					auto qy = this->qptr, wy = this->wptr;
					Output temp = Output(0);
					for (int j = 0; j++ < this->NodeNum; ++qy, ++wy)
					{
						double node[2] = { mid[0] + *qx*lengthHalf[0],mid[1] + *qy*lengthHalf[1] };
						temp += *wy*func(node);
					}
					re += *wx*temp;
				}
				return lengthHalf[0] * lengthHalf[1] * re;
			}
		};

		typedef IntegrationSquare<double> IntegrationSquared;
		typedef IntegrationSquare<complex<double>> IntegrationSquarecd;
		typedef IntegrationSquare<complex<double>, Vector2cd> IntegrationSquareVectorcd;

		//Global Data
		static IntegrationSquared IntSqd;
		static IntegrationSquarecd IntSqcd;
		static IntegrationSquareVectorcd IntSqVcd;
#pragma endregion 

#pragma region Sector
		template<typename Scalar, typename Output = Scalar>
		class IntegrationSector :public IntegrationLine< Scalar>
		{
			typedef Output(*Func)(const double[2]);
			typedef double(*Limitation)(const double);
		public:
			IntegrationSector(const QuraNum newVal = P4) :IntegrationLine<Scalar>(newVal) {}

			/**
			* \Square integration
			*
			* \param func
			* For a instance, double z=f(x,y),which (x,y) means a point
			* \param startx
			* Input start value of x
			* \param endx
			* Input end value of x 
			* \param lower
			* lower limitation ylow(node.x)
			* \param upper
			* Upper limitation yup(node.x) 
			* \return Output is Scalar or Vector2
			*/
			Output operator()(Func func, double startx, double endx,Limitation lower,Limitation upper) 
			{
				Output re = Output(0);
				const double lengthHalfx = 0.5*(endx - startx),midx = 0.5*(startx + endx);
				double node[2] = { 0,0 };
				const double* qx = this->qptr, *wx = this->wptr;
				for (int i = 0; i++ < this->NodeNum; ++qx, ++wx)
				{
					node[0] = midx + *qx*lengthHalfx;
					const double starty = lower(node[0]), endy = upper(node[0]);
					double lengthHalfy = 0.5*(endy - starty);
					double midy = 0.5*(starty + endy);
					auto qy = this->qptr, wy = this->wptr;
					Output temp = Output(0);
					for (int j = 0; j++ < this->NodeNum; ++qy, ++wy)
					{
						node[1] = midy + *qy*lengthHalfy;
						temp += *wy*func(node);
					}
					re += *wx*lengthHalfy*temp;
				}
				return lengthHalfx*re;
			}
		};

		typedef IntegrationSector<double> IntegrationSectord;
		typedef IntegrationSector<complex<double>> IntegrationSectorcd;
		typedef IntegrationSector<complex<double>, Vector2cd> IntegrationSectorVectorcd;

		//Global Data
		static IntegrationSectord IntSed;
		static IntegrationSectorcd IntSecd;
		static IntegrationSectorVectorcd IntSeVcd;

#pragma endregion 

#pragma region Triangle
		//every line means three coefficients and weight
		//Noneed coefficients 0.5 before wgt but need the area of triangle
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
			0.312865496004875,0.638444188569809,0.048690315425315,0.077113760890257};
	
		/**
		 * \IntegrationOfTriangle
		 * Single and double integration define on the triangle surface
		 * \tparam triangle 
		 * class triangle must provide two pubilc member functions:
		 * @1 double triangle::Area()
		 * @2 Matrix3d triangle::Vertex() which returns the three vertexs
		 * \tparam Output = double
		 */
		template<class triangle, typename Output=double>
		class IntegrationTriangle:IntegrationBase
		{
		public:
			typedef Output(*Func)(const Vector3d);
			typedef Output(*Func2)(const Vector3d, const Vector3d);
			explicit IntegrationTriangle(const QuraNum newVal = P4){
				IntegrationTriangle<Output, triangle>::SetNodeNum(newVal);
			}

			virtual void SetNodeNum(const QuraNum newVal) override
			{
				NodeNum = newVal;
				SetQuad();
			}
			
			/**
			 * \SingleIntegration
			 * Just like complex y=f(r),r is Vector3d 
			 * \param func 
			 * \param region 
			 * \return 
			 */
			Output operator()(Func func,triangle region)
			{
				Output re = Output(0);
				//Matrix3d vertex
				MatrixXd qPoints = region.Vertex()*_quad.block(0,0,2,_quad.cols());
				VectorXd funcResult(_quad.cols());
				for (auto i = 0; i<int(NodeNum); i++)funcResult[i] = func(qPoints.col(i));
				re = funcResult.dot(_quad.row(4).transpose());
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
			Output operator()(Func2 func, triangle outer,triangle inner)
			{
				Output re = Output(0);
				//Matrix3d vertex
				MatrixXd qPointsOuter = outer.Vertex()*_quad.block(0, 0, 2, _quad.cols());
				MatrixXd qPointsInner = inner.Vertex()*_quad.block(0, 0, 2, _quad.cols());

				/*MatrixXd funcResult(_quad.cols(), _quad.cols());
				for (int i = 0; i < int(NodeNum); ++i)
					for (int j = 0; j < int(NodeNum); ++j)
						funcResult(i,j)=func(qPointsOuter.col(i), qPointsInner.col(j));
				VectorXd temp=_quad.row(4)*funcResult;
				re = temp.dot(_quad.row(4))*/
					
				for (auto i = 0; i<int(NodeNum);i++ )
				{
					Output temp = Output(0);
					for(auto j=0;j<int(NodeNum);j++)
					{
						temp += _quad(4, j)*func(qPointsOuter.col(i), qPointsInner.col(j));
					}
					re += _quad(4, i)*temp;
				}	
				return outer.Area()*inner.Area()*re;
			}

		protected:
			MatrixXd _quad;
			QuraNum NodeNum;

			/**
			* \Bind _quad.resize(4,NodeNum)
			*/
			void SetQuad()
			{
				switch (NodeNum)
				{
				case P7:_quad.resize(4, 7);
					for (int col = 0; col < 7; ++col)
						for (int row = 0; row < 4; ++row)
							_quad(row, col) = Tri7[col][row];
					return;
				case P13:_quad.resize(4, 13);
					for (int col = 0; col < 7; ++col)
						for (int row = 0; row < 4; ++row)
							_quad(row, col) = Tri13[col][row];
					return;
				default:
					_quad.resize(4, 4);
					for (int col = 0; col < 4; ++col)
						for (int row = 0; row < 4; ++row)
							_quad(row, col) = Tri4[col][row];
				}
			}
		};

#pragma endregion 

	}

}

