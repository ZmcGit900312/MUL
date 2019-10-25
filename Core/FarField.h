///////////////////////////////////////////////////////////
//  FarField.h
//  Implementation of the Class FarField
//  Created on:      24-4月-2018 20:40:02
//  Original author: ZhengMX
///////////////////////////////////////////////////////////

#pragma once
#include "RequestConfiguration.h"
#include "IBasisFunction.h"
#include "Mesh.h"
#include "Current.h"
namespace Core
{
	
	namespace Request
	{

		/**
		* \brief 远场Sphere版本 计算效率太低
		* 优化更好，提供GetEField和GetRCS来给出RCS和电场
		*/
		class FarField
		{
		public:
			typedef MatrixXd RCSData;

			FarField(vector<IBasisFunction*>*, Mesh*, 
				Solution::CurrentInfo* current=Solution::CurrentInfo::GetInstance());
			~FarField();
			/**
			* \brief 直接调用的主函数
			*/
			void CalculateRCS(FarFieldConfiguration& config,ofstream& ofs) const;
			void CalculateRCS(FarFieldConfiguration& config, int row,int col)const;
			/**
			* \brief Calculate the E field of the specific direction(theta,phi)
			* \param theta rad
			* \param phi rad
			* \return Vector3cd Efield
			*/
			Vector3cd EField(const double theta, const double phi,Solution::ElementCurrent* ec) const;
			//Array
			Vector3cd EField(const double theta, const double phi, Solution::ArrayCurrent* ac) const;
			/**
			 * \brief 用于测试的标准数据生成函数
			 * \param theta 
			 * \param phi 
			 * \return 
			 */
			Vector3cd EFieldBenchMark(const double theta, const double phi,VectorXcd& current)const;
			//Array
			Vector3cd EFieldBenchMark(const double theta, const double phi, Solution::ArrayCurrent* ac)const;
			
			Vector3cd EFieldArrayFactor(const double theta, const double phi, Solution::ArrayCurrent* ac)const;
			static void SaveRCS(ofstream&ofs, FarFieldConfiguration& config,int zmc);

			/**
			* \brief Radius of farfield observation
			* \n\b PS: It should be ajust with the size of object
			*/
			double Radius = 10000;//
			double Coef;
			Solution::CurrentInfo* _current = nullptr;
					
			static Matrix<RCSData,Dynamic, Dynamic> RCS;
		private:

			Mesh* _mesh;
			vector<IBasisFunction*>*_bf;
			//Integration Kernel and Integration
			
			Vector3cd _theta0, _theta180;
			double _rcs0 = 0, _rcs180 = 0;
		};




	}
	
	
}

