#pragma once
#include <Eigen/Core>
#include "IBasicFunction.h"
#include <vector>

using namespace std;
using namespace Eigen;

namespace Core
{
	namespace Source
	{
		enum SourceType
		{
			EXCITATION_VOLTAGE = 0,
			EXCITATION_LINEAR = 1,
			EXCITATION_CIRC_RIGHT = 2,
			EXCITATION_CIRC_LEFT = 3,
			EXCITATION_CURRENT = 4,
			EXCITATION_VOLTAGE_DISC = 5
		};

		class ISource
		{
		public:
			virtual ~ISource() = default;
			
			dcomplex E0;
			Vector3d Ki{ 0,0,-1 }, Ei{ 1,0,0 };
			//ExcitationName
			string Name;
			/**
			 * \brief Set Exciation(righthand)
			 * \param bfVector
			 * \return Excitation
			 */
			virtual VectorXcd SetExcitation(const vector<IBasicFunction*>&bfVector)const=0;
			inline virtual SourceType GetSourceType()const = 0;
		};
#pragma region Planewave
		class Planewave:public ISource
		{
		public:
			Planewave(std::string name, unsigned thn, unsigned phn, double ths, double phs, double thi, double phi, double mag, double phase, double eta, double rx, double ry, double rz);
			virtual ~Planewave(){}
			//Direction
			unsigned ThetaNum = 1, PhiNum = 1;
			double ThetaStart = 0, PhiStart = 0;
			double ThetaIncrement = 1, PhiIncrement = 1;
			double Magnitude = 1.0;
			double Phase = 0.0;
			double Eta = 0.0;
			double RotationX = 0.0, RotationY = 0.0, RotationZ = 0.0;
		};

		class PlaneWaveLinear:public Planewave
		{
		public:
			PlaneWaveLinear(string name, unsigned thn, unsigned phn, double ths, double phs, double thi, double phi, double mag, double phase, double eta, double rx, double ry, double rz) :Planewave(name, thn, phn, ths, phs, thi, phi, mag, phase, eta, rx, ry, rz){}
			
			SourceType GetSourceType()const override {return EXCITATION_LINEAR;}						
			//Operation
			VectorXcd SetExcitation(const vector<IBasicFunction*>&bfVector)const override;
		};

		class PlaneWaveRight :public Planewave
		{
		public:
			PlaneWaveRight(string name, unsigned thn, unsigned phn, double ths, double phs, double thi, double phi, double mag, double phase, double eta, double rx, double ry, double rz, double ell) :Planewave(name, thn, phn, ths, phs, thi, phi, mag, phase, eta, rx, ry, rz), Ellipticity(ell) {}
			double Ellipticity = 1.0;
			SourceType GetSourceType()const override { return EXCITATION_CIRC_RIGHT; }
			//Operation
			VectorXcd SetExcitation(const vector<IBasicFunction*>&bfVector)const override{};
		};

		class PlaneWaveLeft :public Planewave
		{
		public:
			PlaneWaveLeft(string name, unsigned thn, unsigned phn, double ths, double phs, double thi, double phi, double mag, double phase, double eta, double rx, double ry, double rz, double ell) :Planewave(name, thn, phn, ths, phs, thi, phi, mag, phase, eta, rx, ry, rz),Ellipticity(ell) {}
			double Ellipticity = 1.0;
			SourceType GetSourceType()const override { return EXCITATION_CIRC_LEFT; }
			//Operation
			VectorXcd SetExcitation(const vector<IBasicFunction*>&bfVector)const override {};
		};
#pragma endregion 
		class VoltageGap :public ISource
		{
		public:
			VoltageGap(string name);
			~VoltageGap();
			SourceType GetSourceType()const override { return EXCITATION_VOLTAGE; }
			struct Vol
			{
				unsigned long NegativeSide = 0;
				unsigned long PositiveSide = 0;
				dcomplex Voltage{ 1.0,0.0 };
			};

			vector<Vol> VoltageVector;
			dcomplex Admitance{50.0,0.0};

			VectorXcd SetExcitation(const vector<IBasicFunction*>&bfVector)const override;
		};

		//struct ExcitationConfiguration
		//{
		//	int sourceType = 0;
		//	//Direction
		//	unsigned ThetaNum = 1, PhiNum = 1;
		//	double ThetaStart = 0, PhiStart = 0;
		//	double ThetaIncrement = 1, PhiIncrement = 1;
		//	//E polarisation
		//	int Polarisation = 0;//-1:left,0:linear,1:right
		//	double Magnitude = 1.0;
		//	double Phase = 0.0;
		//	double Eta = 0.0;
		//	double RotationX = 0.0, RotationY = 0.0, RotationZ = 0.0;
		//	//FileName
		//	std::string ExcitationName;
		//	Vector3d Ki{ 0,0,-1 }, Ei{ 1,0,0 };
		//	
		//	//Operation
		//	void SetDirection();
		//	
		//};

		static Vector3d Rotation(const double theta, const double phi, const Vector3d & v);
	}
}


