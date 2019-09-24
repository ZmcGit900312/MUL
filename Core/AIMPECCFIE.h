#ifndef AIMPECCFIE_H
#define AIMPECCFIE_H

#include "Green.h"
#include "IntegrationRWG.h"
#include "ImpAIM.h"
#include "IMatrixFiller.h"
#include "Multiplicator.h"

namespace Core
{

	class AIMPECCFIE :public IMatrixFiller
	{
	public:
		AIMPECCFIE(const ImpConfiguration& configuration, IImpService*impedance,IEConfiguration ie);
		~AIMPECCFIE();
		void MultipoleExpansion(vector< IBasicFunction*>&bf)override;
		//Filling AIM Green Matrix in multilevel FFT
		void GreenMatrixSet(IGreen* green)override;
		//Near Matrix Setting
		void TriangleFillingStrategy(Mesh& mesh, vector<IBasicFunction*>&bf)override;
		void NearCorrection(vector<IBasicFunction*>&bf)override;


		//For Test API
		VectorXcd& GetGreenBase(int xyz)
		{
			switch (xyz)
			{
				case 1:return _greenGradientBaseX;
				case 2:return _greenGradientBaseY;
				case 3:return _greenGradientBaseZ;
				default: return _greenBase;
			}
		}
		dcomplex GetImpAIM(const size_t row, const size_t col) { return GetFarFieldImpedacneAIM(row, col); }
		
		VectorXcd constructIterated(unsigned& bias, const unsigned level);

		/**
		 * \brief Set GreenGradient with the specific component x=0,y=1,z=2
		 * \param component 
		 * \return 
		 */
		VectorXcd construct(int component);		
		//From Gama location to Green location
		size_t gridGreenlocation(const Vector4i& p1, const Vector4i& p2)const;		

		
	protected:
		//inner api
		void GenerateGreenBase(IGreen* green);
		dcomplex GetFarFieldImpedacneAIM(const size_t row, const size_t col);		
		ImpAIM* _imp = nullptr;
	};


}

#endif