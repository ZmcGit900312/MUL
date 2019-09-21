#ifndef IMATRIXFILLER_H
#define IMATRIXFILLER_H

#include "Green.h"
#include "IntegrationRWG.h"
#include "ImpAIM.h"
#include "MultipoleExpansioner.h"


namespace Core
{
	class IMatrixFiller
	{
	public:
		IMatrixFiller(const ImpConfiguration & configuration, IImpService * impedance, const IEConfiguration& ieConfig) :
		_unknowns(configuration.ImpSize),
		_interval(configuration.Interval), 
		_threshold(configuration.Threshold*Lambda), 
		_eps(configuration.NearCorrectionEps),
		_layerNum(Vector3i{ configuration.xNumber,configuration.yNumber,configuration.zNumber }), 
		_compute(k, W4, W7, eta),
		_dim(configuration.Dimension),
		_equationType(ieConfig.type)
		{
			//IE
			if (_equationType == CFIE)
			{
				_alpha = ieConfig.Alpha;
				_eta = ieConfig.Eta;
			}
		}

		virtual ~IMatrixFiller(){}

		virtual void MultipoleExpansion(vector<IBasicFunction*>&bf)=0;
		virtual void GreenMatrixSet(IGreen* green) = 0;
		virtual void TriangleFillingStrategy(Mesh&mesh, vector<IBasicFunction*>&bf)=0;
		virtual void NearCorrection(vector<IBasicFunction*>&bf)=0;
		
		//Factory Function
		static IMatrixFiller* FMatrixFiller(
			const ImpConfiguration& impConfig, 
			IImpService*impedance,
			const IEConfiguration& ieConfig);

		//MultiExpantion Class
		AIMAssist::MultipoleExpansioner MultiExpTool;
	protected:

		//inner api
		virtual void GenerateGreenBase(IGreen* green) = 0;
		virtual dcomplex GetFarFieldImpedacneAIM(const size_t row, const size_t col)=0;

		//Triplet
		typedef Triplet<dcomplex> T;
		typedef struct {
			vector<Vector4i> index;
			VectorXcd gamax, gamay, gamaz, gamad;
		}gama;

		//ImpConfig
		size_t _unknowns;
		double _interval;
		double _threshold;
		double _eps;
		const Vector3i _layerNum;//xyz
		//Local Cubic
		size_t _gridNum=1;
		RWGImpOperator _compute;

		//Teoplitz parameters
		int _dim = 2;
		Vector3i _layerElementSize{ 1,1,1 };
		Vector3i _layerElementSizeAcu{ 1,1,1 };//multiplication size for layer

		//IE Configuration
		double _alpha = 0.2, _eta = 120 * 3.1415926;
		IETYPE _equationType = EFIE;

		//Temp memory
		vector<gama> _gama;
		VectorXcd _greenBase, _greenGradientBaseX, _greenGradientBaseY, _greenGradientBaseZ;
		MatrixXcd _localGreen, _localGreenX, _localGreenY, _localGreenZ;
	};


	
}


#endif
