#pragma once

#include <Eigen/Core>
#include "Green.h"
#include "IntegrationRWG.h"
//Not use temporary
namespace Core
{
	enum IETYPE { EFIE = 0, MFIE, CFIE, IBCEFIE, IBCMFIE, IBCCFIE };

	//IE interface
	class IE
	{

	public:
		virtual ~IE() {}
		/**
		 * \brief Fill Impedance Matrix in triangle index(self)
		 * \param t Specific triangle
		 * \return The list of self-impedance service for same  RWG
		 */
		virtual vector<element> Set(RWGTriangle*t) = 0;
		/**
		 * \brief K operator with Different Triangle Pair
		 * \param field Field Triangle
		 * \param source Source Triangle
		 * \param val The marked RWG basis functions
		 */
		virtual void Set(RWGTriangle*field, RWGTriangle*source, vector<element>& val) = 0;
		/**
		 * \brief Calculate the specific RWG RightHand
		 * \param source BasisFunction
		 * \param ki The direction of wave propagation
		 * \param efield The direction of E field
		 * \param hfield The direction of H field
		 * \return Righthand 
		 */
		virtual dcomplex SetRight(RWG* source, Vector3d ki, Vector3d efield,Vector3d hfield=Vector3d::Zero()) const {
			return _computerCore.SetIncidentFieldVector(source, ki, efield);
		}
		/**
		 * \brief IE Factory 
		 * \param ty IE  Type
		 * \return IE*
		 */
		static IE* FIE(IETYPE ty);
	protected:
		IE(const double k, double const w4[], double const w7[],
			const double eta) :_computerCore(k, w4, w7, eta) {}
		
		const RWGImpOperator _computerCore;
	};

	class EFIEPEC :public IE
	{
	public:
		virtual ~EFIEPEC() = default;

		EFIEPEC(const double k, double const w4[], double const w7[],
			const double eta = 120 * 3.1415926) :IE(k, w4, w7, eta) {}
		inline vector<element> Set(RWGTriangle*t)override {
			return _computerCore.OperatorL(t);
		}
		inline void Set(RWGTriangle*field, RWGTriangle*source, vector<element>& val)override {
			_computerCore.OperatorL(field, source, val);
		}		
	};

	class MFIEPEC :public IE
	{
	public:
		virtual ~MFIEPEC() = default;

		MFIEPEC(const double k, double const w4[], double const w7[],
			const double eta = 120 * 3.1415926) :IE(k, w4, w7, eta) {}
		inline vector<element> Set(RWGTriangle*t)override {
			auto res = _computerCore.OperatorIdentity(t);

			for(auto val=res.begin();val!=res.end();++val)
			{
				get<2>(*val) *= 0.5;
				//dcomplex& value = get<2>(*val);
				//value *= 0.5;
			}			
			return res;
		}
		inline void Set(RWGTriangle*field, RWGTriangle*source, vector<element>& res)override {
			_computerCore.OperatorK(field, source, res);
			for (auto val = res.begin();val != res.end();++val)
			{
				get<2>(*val) *= -1;
				//dcomplex& value = get<2>(*val);
				//value *= 0.5;
			}
		}
	};


	class CFIEPEC :public IE
	{
	public:

		CFIEPEC(const double alpha,const double k, double const w4[], double const w7[], const double eta = 120 * 3.1415926);
		virtual ~CFIEPEC();
		inline vector<element> Set(RWGTriangle*t)override;
		inline void Set(RWGTriangle*field, RWGTriangle*source, vector<element>& val)override;

		dcomplex SetRight(RWG* source, Vector3d ki, Vector3d efield, Vector3d hfield) const override;
		double Eta = 0, Alpha = 0;
	private:
	};
}