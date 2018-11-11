#pragma once
#include <Eigen/Core>
#include "Mesh.h"
using namespace Eigen;

namespace Core
{
	__interface IBasicFunction
	{
		/**
		 * \brief 查询基函数序号
		 * \return 返回基函数的序号
		 */
		// ReSharper disable CppInconsistentNaming
		virtual size_t GetID()const =0;
		// ReSharper restore CppInconsistentNaming
		/**
		 * \brief 正基函数电流函数
		 * \param pt 定义域中的任意位置矢量
		 * \return 正基函数的电流值
		 */
		virtual Vector3d CurrentPlus(const Vector3d pt)const =0;
		/**
		* \brief 负基函数电流函数
		* \param pt 定义域中的任意位置矢量
		* \return 负基函数的电流值
		*/
		virtual Vector3d CurrentMinus(const Vector3d pt)const =0;
		/**
		* \brief 正基函数电荷函数
		* \param pt 定义域中的任意位置矢量
		* \return 负基函数的电流值
		*/
		virtual double ChargePlus(const Vector3d pt)const =0;
		/**
		* \brief 负基函数电荷函数
		* \param pt 定义域中的任意位置矢量
		* \return 负基函数的电流值
		*/
		virtual double ChargeMinus(const Vector3d pt)const =0;
		/**
		 * \brief 正基函数
		 * \return 正部分的定义域的序号
		 */
		virtual int LimitPlus()const =0;
		/**
		* \brief 负基函数
		* \return 负部分的定义域的序号
		*/
		virtual int LimitMinus()const =0;
		/**
		 * \brief 电流项系数的访问器
		 * \return 基函数电流系数的引用
		 */
		virtual dcomplex& Current() = 0;
		/**
		 * \brief 以二进制形式储存当下的基函数信息
		 * \param ofs 写入的文件流
		 */
		virtual void SaveBinary(ofstream &ofs) = 0;
	};

}


