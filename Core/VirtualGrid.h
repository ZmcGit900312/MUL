//#ifndef VIRTUALGRID_H
//#define VIRTUALGRID_H
//
//#include "MultipoleExpansioner.h"
//#include "Green.h"
//#include "IntegrationRWG.h"
//#include "ImpAIM.h"
//#include "IMatrixFiller.h"
//#include "Multiplicator.h"
//
//namespace Core
//{
//
//	class VirtualGrid:public IMatrixFiller
//	{
//	public:
//		VirtualGrid(const ImpConfiguration& configuration, IImpService*impedance);
//		~VirtualGrid();
//		void MultipoleExpansion(vector< IBasicFunction*>&bf)override;
//		//Filling AIM Green Matrix in multilevel FFT
//		void GreenMatrixSet(IGreen* green)override;
//		//Near Matrix Setting
//		void TriangleFillingStrategy(Mesh& mesh, vector<IBasicFunction*>&bf)override;
//		void NearCorrection(vector<IBasicFunction*>&bf)override;
//
//
//		//For Test API
//		VectorXcd& GetGreenBase() { return _greenBase; }
//		dcomplex GetImpAIM(const size_t row, const size_t col) { return GetFarFieldImpedacneAIM(row, col); }
//
//		VectorXcd constructIterated(unsigned& bias, const unsigned level);
//		//From Gama location to Green location
//		size_t gridGreenlocation(const Vector4i& p1, const Vector4i& p2)const;
//
//		AIMAssist::VirtualME MultiExpTool;
//	protected:
//		//inner api
//		void GenerateGreenBase(IGreen* green)override;
//		dcomplex GetFarFieldImpedacneAIM(const size_t row, const size_t col)override;
//		
//		//Some Tools	
//		
//		typedef struct {
//			vector<Vector4i> index;
//			VectorXcd gamax, gamay, gamaz, gamad;
//		}gama;
//
//		//Teoplitz parameters
//		int _dim = 2;
//		Vector3i _layerElementSize{1,1,1};//2*N-2
//		Vector3i _layerElementSizeAcu{1,1,1};//multiplication size for layer
//
//		//Temp memory
//		vector<gama> _gama;
//		VectorXcd _greenBase;
//
//	};
//
//
//}
//
//#endif