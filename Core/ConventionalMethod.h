#ifndef CONVENTIONALMETHOD_H
#define CONVENTIONALMETHOD_H
#include "Teoplitzcd.h"
#include "Green.h"
#include "IntegrationRWG.h"
#include "ImpAIM.h"
#include "IMatrixFiller.h"

namespace Core
{

	class ConventionalMethod:public IMatrixFiller
	{
	public:
		ConventionalMethod(const ImpConfiguration& configuration, IImpService*impedance, const IEConfiguration& ieConfig);
		~ConventionalMethod();
		void MultipoleExpansion(vector< IBasicFunction*>&bf)override;
		void GreenMatrixSet(IGreen*green)override;
		
		//Near Matrix Setting
		void NearCorrection(vector<IBasicFunction*>&bf)override;
		void TriangleFillingStrategy(Mesh& mesh, vector<IBasicFunction*>&bf)override;

		
		//ForTest
		MatrixXcd GetLocalMatrix()const { return _localGreen; }
		dcomplex GetImpAIM(const size_t row, const size_t col) { return GetFarFieldImpedacneAIM(row, col); }	
		
		//Some Tools
		AIMAssist::Teoplitzcd VectorTp;

	protected:
		void GenerateGreenBase(IGreen*green)override;

		dcomplex GetFarFieldImpedacneAIM(const size_t row, const size_t col)override;
		
		ImpAIM* _imp = nullptr;
	};


}


#endif