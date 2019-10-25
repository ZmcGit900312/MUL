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
		void MultipoleExpansion(vector< IBasisFunction*>&bf)override;
		//Filling AIM Green Matrix in multilevel FFT
		void GreenMatrixSet(IGreen* green)override;
		//Near Matrix Setting
		void TriangleFillingStrategy(Mesh& mesh, vector<IBasisFunction*>&bf)override;
		void NearCorrection(vector<IBasisFunction*>&bf)override;


		//For Test API
#ifdef _DEBUG
		dcomplex GetFarFieldApproximateImpedacne(const size_t row, const size_t col,Vector3i arrayBias); 

#endif
		VectorXcd ConstructIterated(VectorXi& pos, const unsigned level);

		IGreen* _green = nullptr;
		AIMAssist::MulFFTMultiplicator *_tools = nullptr;
		ImpArrayAIM*_imp = nullptr;
	protected:

		
		int _numXArray, _numYArray;
		double _distanceBiasX, _distanceBiasY;
		size_t _totalUnknowns;
		Matrix<bool, Dynamic, Dynamic> _loc;

		VectorXi _layerGreenSize{ 5,1 };
		/**
		 * \brief _layerGreenSizeAcu[i]=_layerGreenSize.head(i+1) 
		 _layerGreenSizeAcu[2] is UnitSize
		 _layerGreenSizeAcu[3] is ArrayX Size
		 _layerGreenSizeAcu[4] is wholeSize
		 */
		VectorXi _layerGreenSizeAcu{ 5,1 };			
		
	};


}

#endif