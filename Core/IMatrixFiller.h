#ifndef IMATRIXFILLER_H
#define IMATRIXFILLER_H

#include "Green.h"
#include "IntegrationRWG.h"
#include "ImpAIM.h"


namespace Core
{
	class IMatrixFiller
	{
	public:
		IMatrixFiller(const ImpConfiguration& configuration, IImpService*impedance);
		virtual ~IMatrixFiller(){}

		virtual void MultipoleExpansion(vector<IBasicFunction*>&bf)=0;
		virtual void GreenMatrixSet(IGreen* green) = 0;
		virtual void TriangleFillingStrategy(Mesh&mesh, vector<IBasicFunction*>&bf)=0;
		virtual void NearCorrection(vector<IBasicFunction*>&bf)=0;
		

		static IMatrixFiller* FMatrixFiller(const ImpConfiguration& configuration, IImpService*impedance);
	protected:

		//inner api
		virtual void GenerateGreenBase(IGreen* green) = 0;
		virtual dcomplex GetFarFieldImpedacneAIM(const size_t row, const size_t col)=0;

		//Triplet
		typedef Triplet<dcomplex> T;
		//ImpConfig
		size_t _unknowns;
		double _interval;
		double _threshold;
		double _eps;
		const Vector3i _layerNum;
		//Local Cubic
		size_t _gridNum=1;

		ImpAIM* _imp=nullptr;
		EFRImp _compute;
	};


}


#endif
