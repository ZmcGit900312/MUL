//特化的RWG阻抗积分
//求积点的接口和命名方式需要修改

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
	 * \brief 特化的RWG积分类，主要用于RWG积分的快速计算
	 * \n 命名方式Tag：
	 * \n EFIE FreeSpace RWG SetImpedance=EFRImp 
	 */
	class EFRImp
	{
	public:

		EFRImp(const double k,double const w4[],double const w7[],const double eta=120*3.1415926);
		~EFRImp(){};
		
		/**
		 * \brief Pre set self-impedance on the specifice triangle
		 * \param t The specific triangle
		 * \param w Weight of Guass-quadrature
		 * \param k wavenumber
		 * \param eta Freespace Impedance
 		 */
		static void SetSelfImpedanceTriangle(RWGTriangle *t,const double w13[13],double k, double eta=120*3.1415926);

		/**
		 * \brief Fill Impedance Matrix in RWG index
		 * \param field Test RWG basic function
		 * \param source Expanded RWG basic function
		 * \return Impedance element
		 */
		dcomplex SetImpedance(RWG*field, RWG*source)const;
		/**
		 * \brief Fill Impedance Matrix in triangle index(self)
		 * \param t Specific triangle 
		 * \return The list of self-impedance service for same  RWG
		 */
		list<element> SetImpedance(RWGTriangle* t) const;
		/**
		 * \brief Integration of Triangle
		 * \param field Field Triangle
		 * \param source Source Triangle
		 * \return The list of couple-impedance service for differnet RWG
		 */
		list<element> SetImpedance(RWGTriangle*field, RWGTriangle*source) const;

		/**
		 * \brief It is used for AIM in TFS
		 * \param field Field Triangle
		 * \param source Source Triangle
		 * \param val The marked RWG BasicFunctions
		 * \return 
		 */
		void SetImpedance(RWGTriangle*field, RWGTriangle*source, vector<element>& val) const;

		/**
		 * \brief Calculate the specific RWG RightHand
		 * \param source BasicFunction
		 * \param ki The direction of wave propagation
		 * \param e The direction of E field
		 * \return 
		 */
		dcomplex SetRightHand(RWG* source, Vector3d ki, Vector3d e);

		Vector3cd Radiation(RWGTriangle* source, Vector3d ob, dcomplex current[3]);
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

		/**
		* \brief Based on Face-to-Face technology, set all the couple-impedance defined on two different triangles, at most 9 elements
		* \param field Field Triangle
		* \param source Source Triangle
		* \param w Weight of Guass-quadrature
		* \param k wavenumber
		* \param eta Freespace Impedance
		* \return The list of all the couple-impedance including the position, at most 9 elements
		*/
		static list<element> UnsingularTriangleIntegration(RWGTriangle*field, RWGTriangle*source, double const*w, double k, double eta);

		double const _k;
		double const _eta;
		double const* _w4, *_w7;
	};
}


