///////////////////////////////////////////////////////////
//  TeoplitzAssist.h
//  Implementation of the Class Teoplitzcd
//  Created on:      26-6ÔÂ-2018 21:40:27
//  Original author: ZhengMX
///////////////////////////////////////////////////////////

#pragma once
#include <Eigen\Core>
#include "Teoplitzcd.h"
#include "FFT1d.h"
#include <vector>

using namespace Eigen;

namespace Core
{
	namespace AIMAssist
	{
		class TeoplitzAssist
		{
		public:
			TeoplitzAssist(Teoplitzcd&tp) { Reset(tp); }
			~TeoplitzAssist();

			void MultiplyTeoplitz (const VectorXcd& tpfft, VectorXcd& x);
			void Reset(Teoplitzcd&tp);
			//Cosnt service
			VectorXcd TeoplitzFFT(Teoplitzcd&tp) const;
			void Padding(const VectorXcd& input, VectorXcd& output)const;
			void Unpadding(const VectorXcd& input, VectorXcd& output) const;

			long Length()const { return _length; }
			unsigned Bias()const { return _bias; }
			//Data Service
			std::vector<int>Index;	//Save the position of each element of Teoplitz used for Padding/Unpadding
		private:
			VectorXcd _workspace;
			FFT1d _trans;
			long _length;
			unsigned _bias;
		};

	}
}