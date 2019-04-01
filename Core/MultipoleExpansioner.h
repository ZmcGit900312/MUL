
#ifndef MULTIPOLEEXPANSIONER_H
#define MULTIPOLEEXPANSIONER_H
#include <Eigen/Core>

#include "RWG.h"
#include "IImpedance.h"
#include "Log.h"
using namespace Eigen;
using namespace std;

namespace Core
{
	namespace AIMAssist
	{
		class MultipoleExpansioner
		{
		public:
			MultipoleExpansioner() {};
			MultipoleExpansioner(const ImpConfiguration& configuration, const double w7[7]) { Reset(configuration, w7); }
			virtual ~MultipoleExpansioner() {}
			/**
			* \brief 初始化多级展开配置
			* \param configuration AIM配置
			* \param w7 求积系数
			*/
			virtual void Reset(const ImpConfiguration& configuration, const double w7[7]);

			/**
			* \brief Inverse of Vandermon Matrix
			* \param bfCentreX  The centre points would be expanded (in x,y,z)
			* \param localBoxStartPointX The fisrt point of local box
			* \return Single Direction (x,y or z) Inverse Matrix of Vandermon
			*/
			MatrixXd VanderInverse(const double bfCentreX, const double localBoxStartPointX) const;
			/**
			* \brief Calculate the Righthand of MultipoleExpansion
			* \param bf BasicFunction
			* \return Righthand of MultipoleExpansion with GridNum*(Dimension+1)
			*/
			MatrixXd MultiExpansionRight(RWG* bf) const;
			/**
			* \brief Find the Start Points of Local Box which is nearest for the given Points Centre
			* \n\b Ps 格点坐标的序号从0开始，每个方向的序号的取值范围为[0,LayerXYZ-1]中的整数
			* \param centre The given points
			* \return The Start Point of the local box
			*/
			virtual Vector3d SearchGrids(const Vector3d& centre);

			virtual Vector2d SearchGrids(const Vector2d&centre);
			/**
			* \brief Calculate the MultipoleExpansion of Given BasicFunction
			* \param bf BasicFunction
			* \return Coefficents with size of GridNum*(Dimension+1)
			*/
			virtual MatrixXd operator()(RWG*bf);
			/**
			 * \brief Get the index
			 */
			VectorXi GetIndex() const { return _index; }

			size_t GetGridNum()const { return _gridNum; }
		protected:
			double const* W7;
			Vector3d _boxStart;
			VectorXi _index;
			size_t _order = 2;
			size_t _dimension = 3;
			size_t _gridNum = 27;
			size_t _layer[3];//Layer[3]={xNum,yNum,zNum}
			double _interval = 0.05;
		};

		class VirtualME :public MultipoleExpansioner
		{
		public:
			VirtualME() :MultipoleExpansioner() {};
			~VirtualME();
			void Reset(const ImpConfiguration& configuration, const double w7[7])override;
			//Index of coefficients
			vector<Vector4i> Index;

			MatrixXd operator()(RWG*bf)override;
			/**
			* \brief Find the Start Points of Local Box which is nearest for the given Points Centre
			* \n\b Ps 格点坐标的序号从0开始，每个方向的序号的取值范围为[0,2*LayerXYZ-2]中的整数
			* \param centre The given points
			* \return The Start Point of the local box
			*/
			Vector3d SearchGrids(const Vector3d& centre)override;

			Vector2d SearchGrids(const Vector2d&centre)override;

		private:
			size_t _weight1 = 1, _weight2 = 1;
		};
	}

}

#endif



