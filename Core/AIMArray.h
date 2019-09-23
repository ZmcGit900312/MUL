#ifndef AIMARRAY_H
#define AIMARRAY_H

#include "Green.h"
#include "IntegrationRWG.h"
#include "IMatrixFiller.h"
#include "Multiplicator.h"
#include "ImpArrayAIM.h"

namespace Core
{

	class AIMArray :public IMatrixFiller
	{
	public:
		AIMArray(const ImpConfiguration& configuration, IImpService*impedance, const IEConfiguration& ieConfig);
		~AIMArray();
		void MultipoleExpansion(vector< IBasicFunction*>&bf)override;
		//Filling AIM Green Matrix in multilevel FFT
		void GreenMatrixSet(IGreen* green)override;
		//Near Matrix Setting
		void TriangleFillingStrategy(Mesh& mesh, vector<IBasicFunction*>&bf)override;
		void NearCorrection(vector<IBasicFunction*>&bf)override;


		//For Test API
		VectorXcd& GetGreenBase() { return _greenBase; }
		dcomplex GetImpAIM(const size_t row, const size_t col) { return GetFarFieldImpedacneAIM(row, col); }

		VectorXcd ConstructIterated(VectorXi& pos, const unsigned level);

		IGreen* _green = nullptr;
	protected:
		//inner api
		void GenerateGreenBase(IGreen* green)override;
		dcomplex GetFarFieldImpedacneAIM(const size_t row, const size_t col)override;
		int _numXArray, _numYArray;
		double _distanceBiasX, _distanceBiasY;
		size_t _totalUnknowns;

		VectorXi _layerGreenSize{ 5,1 };
		/**
		 * \brief _layerGreenSizeAcu[i]=_layerGreenSize.head(i+1) 
		 _layerGreenSizeAcu[2] is UnitSize
		 _layerGreenSizeAcu[3] is ArrayX Size
		 _layerGreenSizeAcu[4] is wholeSize
		 */
		VectorXi _layerGreenSizeAcu{ 5,1 };

		ImpArrayAIM*_imp = nullptr;
		
		
	};


}

#endif