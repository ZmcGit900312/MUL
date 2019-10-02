#pragma once

#include <Eigen/Core>
#include "Green.h"
#include "IntegrationRWG.h"
//Not use temporary
namespace Core
{
	enum IETYPE { EFIE = 0,MFIE, CFIE, IBCEFIE, IBCMFIE, IBCCFIE };

	struct IEConfiguration
	{
		IETYPE type = CFIE;
		double Alpha = 0.2;
		double Eta = 120 * 3.1415926;
	};
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
		 * \return Righthand 
		 */
		virtual dcomplex SetRight(RWG* source, Vector3d ki, Vector3cd efield, Vector3d bias) const {
			return _computerCore.SetIncidentFieldVector(source, ki, efield,bias);
		}
		/**
		 * \brief Radiation by Operator K
		 * \param source Source Triangle
		 * \param ob observation
		 * \param current surface current or mag
		 * \return scattering field
		 */
		virtual Vector3cd KScatter(RWGTriangle* source, Vector3d ob, dcomplex current[3]) const
		{
			return _computerCore.OperatorKScatter(source, ob, current);
		}

		/**
		 * \brief Radiation by Operator L
		 * \param source Source Triangle
		 * \param ob observation
		 * \param current surface current or mag
		 * \return scattering field
		 */
		virtual Vector3cd LScatter(RWGTriangle* source, Vector3d ob, dcomplex current[3])
		{
			return _computerCore.OperatorLScatter(source, ob, current);
		}
		/**
		 * \brief IE Factory 
		 * \param ty IE  Type
		 * \return IE*
		 */
		static IE* FIE(IETYPE ty);
		string static GetTypeString(IETYPE ty);
		virtual IETYPE GetType()const = 0;
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
		vector<element> Set(RWGTriangle*t)override {
			return _computerCore.OperatorL(t);
		}

		void Set(RWGTriangle*field, RWGTriangle*source, vector<element>& val)override {
			_computerCore.OperatorL(field, source, val);
		}
		
		IETYPE GetType()const override { return EFIE; }
	};

	class MFIEPEC :public IE
	{
	public:
		virtual ~MFIEPEC() = default;

		MFIEPEC(const double k, double const w4[], double const w7[],
			const double eta = 120 * 3.1415926) :IE(k, w4, w7, eta) {}

		vector<element> Set(RWGTriangle*t)override {
			return _computerCore.OperatorIdentity(t);
		}

		void Set(RWGTriangle*field, RWGTriangle*source, vector<element>& res)override {
			_computerCore.OperatorK(field, source, res);
			for (auto val = res.begin();val != res.end();++val)
			{
				get<2>(*val) *= -1;
				//dcomplex& value = get<2>(*val);
				//value *= 0.5;
			}
		}

		IETYPE GetType()const override { return MFIE; }
	};


	class CFIEPEC :public IE
	{
	public:

		CFIEPEC(const double alpha,const double k, double const w4[], double const w7[], const double eta = 120 * 3.1415926);
		virtual ~CFIEPEC();

		vector<element> Set(RWGTriangle*t)override{
			return _computerCore.OperatorCPEC(t, Alpha, Eta);
		}

		void Set(RWGTriangle*field, RWGTriangle*source, vector<element>& val)override{
			return _computerCore.OperatorCPEC(field, source, val, Alpha, Eta);
		}

		dcomplex SetRight(RWG* source, Vector3d ki, Vector3cd efield, Vector3d bias) const override
		{
			return _computerCore.SetIncidentFieldVector(source, ki, efield, Alpha, Eta, bias);
		}

		IETYPE GetType()const override { return CFIE; }
		double Eta = 0, Alpha = 0;
	private:
	};
}