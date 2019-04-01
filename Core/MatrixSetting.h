#pragma once
#include "MultipoleExpansioner.h"
#include "Teoplitzcd.h"
#include "Green.h"
#include "IntegrationRWG.h"
#include "ImpAIM.h"


namespace Core
{

	class MatrixSetting
	{
	public:
		MatrixSetting(const ImpConfiguration& configuration, IImpService*impedance);
		~MatrixSetting();
		void MultipoleExpansion(vector< IBasicFunction*>&bf);
		void TeoplitzSet(IGreen*green) { VectorTp.construct(generateGreenVector(green)); }
		//Near Matrix Setting
		void NearCorrection(vector<IBasicFunction*>&bf);
		void TriangleFillingStrategy(Mesh& mesh, vector<IBasicFunction*>&bf);

		void GreenMatrixSet();

		//ForTest
		MatrixXcd GetLocalMatrix()const { return _localGreen; }

		//
		VectorXcd generateGreenVector(IGreen* green) const;
		dcomplex GetFarFieldImpedacneAIM(const size_t row, const size_t col);

		//Some Tools
		AIMAssist::Teoplitzcd VectorTp;

		AIMAssist::MultipoleExpansioner MultiExpTool;
	private:
		typedef struct {
			VectorXi index;
			VectorXcd gamax, gamay, gamaz, gamad;
		}gama;

		//Triplet
		typedef Triplet<dcomplex> T;
		//ImpConfig
		size_t _unknowns;
		double _interval;
		double _threshold;
		double _eps;
		const Vector3i _layerNum;
		//Local Cubic
		size_t _gridNum;

		//Temp memory
		vector<gama> _gama;
		MatrixXcd _localGreen;

		EFRImp _compute;
		//Handle of AIM Impedance
		ImpAIM* _imp;
	};


}
