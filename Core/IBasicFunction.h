#pragma once
#include <Eigen/Core>
#include "Mesh.h"
using namespace Eigen;

namespace Core
{
	__interface IBasicFunction
	{
		/**
		 * \brief ��ѯ���������
		 * \return ���ػ����������
		 */
		// ReSharper disable CppInconsistentNaming
		virtual size_t GetID()const =0;
		// ReSharper restore CppInconsistentNaming
		/**
		 * \brief ����������������
		 * \param pt �������е�����λ��ʸ��
		 * \return ���������ĵ���ֵ
		 */
		virtual Vector3d CurrentPlus(const Vector3d pt)const =0;
		/**
		* \brief ����������������
		* \param pt �������е�����λ��ʸ��
		* \return ���������ĵ���ֵ
		*/
		virtual Vector3d CurrentMinus(const Vector3d pt)const =0;
		/**
		* \brief ����������ɺ���
		* \param pt �������е�����λ��ʸ��
		* \return ���������ĵ���ֵ
		*/
		virtual double ChargePlus(const Vector3d pt)const =0;
		/**
		* \brief ����������ɺ���
		* \param pt �������е�����λ��ʸ��
		* \return ���������ĵ���ֵ
		*/
		virtual double ChargeMinus(const Vector3d pt)const =0;
		/**
		 * \brief ��������
		 * \return �����ֵĶ���������
		 */
		virtual int LimitPlus()const =0;
		/**
		* \brief ��������
		* \return �����ֵĶ���������
		*/
		virtual int LimitMinus()const =0;
		/**
		 * \brief ������ϵ���ķ�����
		 * \return ����������ϵ��������
		 */
		virtual dcomplex& Current() = 0;
		/**
		 * \brief �Զ�������ʽ���浱�µĻ�������Ϣ
		 * \param ofs д����ļ���
		 */
		virtual void SaveBinary(ofstream &ofs) = 0;
	};

}


