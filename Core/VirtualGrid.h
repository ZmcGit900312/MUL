#ifndef VIRTUALGRID_H
#define VIRTUALGRID_H

#include "Green.h"
#include "IntegrationRWG.h"
#include "ImpAIM.h"
#include "IMatrixFiller.h"
#include "Multiplicator.h"

namespace Core
{

	class VirtualGrid:public IMatrixFiller
	{
	public:
		VirtualGrid(const ImpConfiguration& configuration, IImpService*impedance, const IEConfiguration& ieConfig);
		~VirtualGrid();
		void MultipoleExpansion(vector< IBasicFunction*>&bf)override;
		//Filling AIM Green Matrix in multilevel FFT
		void GreenMatrixSet(IGreen* green)override;
		//Near Matrix Setting
		void TriangleFillingStrategy(Mesh& mesh, vector<IBasicFunction*>&bf)override;
		void NearCorrection(vector<IBasicFunction*>&bf)override;


		//For Test API
		VectorXcd& GetGreenBase() { return _greenBase; }
		dcomplex GetImpAIM(const size_t row, const size_t col) { return GetFarFieldImpedacneAIM(row, col); }

		VectorXcd constructIterated(unsigned& bias, const unsigned level);
		//From Gama location to Green location
		size_t gridGreenlocation(const Vector4i& p1, const Vector4i& p2)const;

		void GenerateGreenBase2(IGreen* green);
	protected:
		//inner api
		void GenerateGreenBase(IGreen* green)override;
		dcomplex GetFarFieldImpedacneAIM(const size_t row, const size_t col)override;					
		ImpAIM* _imp = nullptr;
	};


}

#endif