//�ػ���RWG�迹����
//�����Ľӿں�������ʽ��Ҫ�޸�

#pragma once
#include <Eigen/Core>
#include "RWG.h"
#include <list>
using namespace Eigen;
using namespace Core;

namespace Core
{
	typedef tuple<int, int, dcomplex> element;

	/**
	 * \brief �ػ���RWG�����࣬��Ҫ����RWG���ֵĿ��ټ���
	 * \n ����L��RWGΪ�������ͼ��麯����Ҳ�ʺ�n.cross.RWG��У��n.cross.E��EFIE��J��
	 * \n ����K��RWGΪ��������n.cross.RWGΪ���麯����У��n.cross.H��MFIE�е�J��
	 * \n �Ҷ���ʹ��SetIncidentFieldVector��������E��n.cross.Hֱ�ӽ����ڻ�����
	 */
	class RWGImpOperator
	{
	public:

		RWGImpOperator(const double k,double const w4[],double const w7[],const double eta=120*3.1415926);
		~RWGImpOperator(){};
		
		/**
		 * \brief Pre set self-impedance on the specifice triangle
		 * \param t The specific triangle
		 * \param w Weight of Guass-quadrature
		 * \param k wavenumber
		 * \param eta Freespace Impedance
		 */
		static void SetSelfTriangleOperatorL(RWGTriangle *t,const double w13[13],double k, double eta=120*3.1415926);
#pragma region ImpedanceOperator
		/**
		 * \brief Based on Face-to-Face technology, set all the couple-impedance defined on two different triangles, at most 9 elements
		 * \param field Test RWG basic function
		 * \param source Expanded RWG basic function
		 * \return Impedance element
		 */
		dcomplex SetImpedanceL(RWG*field, RWG*source)const;
		/**
		 * \brief Integration of Triangle
		 * \param field Field Triangle
		 * \param source Source Triangle
		 * \return The list of couple-impedance service for differnet RWG
		 */
		vector<element> SetImpedanceL(RWGTriangle*field, RWGTriangle*source) const;
		/**
		 * \brief Based on Face-to-Face technology, set all the couple-impedance defined on two different triangles, at most 9 elements
		 * \param field Test RWG basic function
		 * \param source Expanded RWG basic function
		 * \return Impedance element
		 */
		dcomplex SetImpedanceK(RWG*field, RWG*source)const;
		/**
		 * \brief Fill Impedance Matrix in triangle index(self)
		 * \param t Specific triangle 
		 * \return The list of self-impedance service for same  RWG
		 */
		vector<element> OperatorL(RWGTriangle* t) const;
		/**
		 * \brief It is used for AIM in TFS
		 * \param field Field Triangle
		 * \param source Source Triangle
		 * \param val The marked RWG basis functions
		 * \return 
		 */
		void OperatorL(RWGTriangle*field, RWGTriangle*source, vector<element>& val) const;
		/**
		 * \brief Identity Operator in a single triangle
		 * \param t Triangle
		 * \return Six 0.5*inner products between n*f and f 
		 */
		vector<element> OperatorIdentity(RWGTriangle * t) const;
		/**
		 * \brief K operator with Different Triangle Pair
		 * \param field Field Triangle
		 * \param source Source Triangle
		 * \param val The marked RWG basis functions
		 */
		void OperatorK(RWGTriangle* field, RWGTriangle* source, vector<element>& val) const;
		/**
		 * \brief Fill Impedance Matrix in triangle index(self)
		 * \param t Specific triangle
		 * \return The list of self-impedance service for 
		 */
		vector<element> OperatorCPEC(RWGTriangle* t,double alpha,double eta) const;
		/**
		 * \brief K operator with Different Triangle Pair
		 * \param field Field Triangle
		 * \param source Source Triangle
		 * \param val The marked RWG basis functions
		 */
		void OperatorCPEC(RWGTriangle* field, RWGTriangle* source, vector<element>& val, double alpha, double eta) const;
		/**
		 * \brief Radiation by Operator K
		 * \param source Source Triangle
		 * \param ob observation
		 * \param current surface current or mag
		 * \return scattering field
		 */
		Vector3cd OperatorKScatter(RWGTriangle* source, Vector3d ob, dcomplex current[3]) const;

		/**
		 * \brief Radiation by Operator L
		 * \param source Source Triangle
		 * \param ob observation
		 * \param current surface current or mag
		 * \return scattering field
		 */
		Vector3cd OperatorLScatter(RWGTriangle* source, Vector3d ob, dcomplex current[3]) const;
#pragma endregion 

		/**
		 * \brief Calculate the specific RWG RightHand
		 * \param source BasisFunction
		 * \param ki The direction of wave propagation
		 * \param incfield The direction of E or H field
		 * \return
		 */
		dcomplex SetIncidentFieldVector(RWG* source, Vector3d ki, Vector3cd incfield) const;

		/**
		 * \brief Calculate the specific RWG RightHand
		 * \param source BasisFunction
		 * \param ki The direction of wave propagation
		 * \param incfield The direction of E+H field
		 * \return
		 */
		dcomplex SetIncidentFieldVector(RWG* source, Vector3d ki, Vector3cd efield,double alpha,double eta) const;
	private:
		/**
		* \brief Impedance between two different triangles as the RWG index
		* \param field Field Triangle
		* \param source Source Triangle
		* \param fieldFreePt Free piont on the field triangle
		* \param sourceFreePt Free point on the source triangle
		* \param w Weight of Guass-quadrature
		* \param k wavenumber
		* \return The couple-impedance between two different triangle
		*/
		static dcomplex UnsingularRWGIntegration(RWGTriangle*field, RWGTriangle*source, const Vector3d fieldFreePt, const Vector3d sourceFreePt,double const* w, double k);

		double const _k;
		double const _eta;
		double const* _w4, *_w7;
	};
}


