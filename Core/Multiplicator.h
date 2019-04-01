#ifndef MULTIPLICATOR_H
#define MULTIPLICATOR_H

#include <Eigen\Core>
#include "FFTmd.h"
#include "Teoplitzcd.h"

using namespace Eigen;

namespace Core
{
	namespace AIMAssist
	{
		class Multiplicator
		{
		public:
			Multiplicator() {}
			virtual ~Multiplicator(){}

			virtual void MVP(const VectorXcd&FFTMatrix, VectorXcd&res)=0;
			virtual MKL_LONG Length()const { return _computer.Length(); }
			virtual MKL_LONG Dim()const { return _computer.Dim(); }
			virtual void fwd(VectorXcd&x)const { _computer.fwd(x); }
			virtual void inv(VectorXcd&x)const { _computer.inv(x); }
			virtual void SetStatus(const bool threadsDynamic, const int threadsNum) { _computer.SetStatus(threadsDynamic, threadsNum); }
			virtual int Reset(MKL_LONG dim, MKL_LONG layer[]) { return _computer.Reset(dim, layer); }
		protected:
			FFTmd _computer;
		};


		//Multilevel FFT
		class MulFFTMultiplicator :public Multiplicator
		{
		public:
			MulFFTMultiplicator() {}
			~MulFFTMultiplicator() {}

			void MVP(const VectorXcd&FFTMatrix, VectorXcd&res) override;			
		};

		//Singlelevel FFT
		class SingleFFTMultiplicator:public Multiplicator
		{
		public:
			SingleFFTMultiplicator(){}
			~SingleFFTMultiplicator();

			void MVP(const VectorXcd&FFTMatrix, VectorXcd&res) override;

			int Reset(Teoplitzcd & tp);

			VectorXcd TpFFT(Teoplitzcd&tp) const;
			//inner api
			void Padding(const VectorXcd& input, VectorXcd& output)const;
			void Unpadding(const VectorXcd& input, VectorXcd& output) const;
		private:
			std::vector<int>Index;	//Save the position of each element of Teoplitz used for Padding/Unpadding
			VectorXcd _workspace;
			unsigned _bias=0;
		};
	}
}

#endif
