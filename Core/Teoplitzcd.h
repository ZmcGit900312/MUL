///////////////////////////////////////////////////////////
//  Teoplitzcd.h
//  Implementation of the Class Teoplitzcd
//  Created on:      21-6月-2018 21:40:27
//  Original author: ZhengMX
///////////////////////////////////////////////////////////

#pragma once
#include <Eigen\Core>
#include <vector>

using namespace Eigen;
namespace Core
{
	namespace AIMAssist
	{
		/**
		 * 额外需要一个生成基函数来生成原始矩阵
		 */
		class Teoplitzcd
		{

		public:
			Teoplitzcd(VectorXi layerNum=VectorXi::Zero(1));
			~Teoplitzcd() { Index.clear(); }
			void reset(const VectorXi layerNum);
			void construct(const VectorXcd& row) { constructIterated(row, _layer - 1);}
			VectorXcd& data() { return _data; }
			dcomplex at(const VectorXi&row, const VectorXi&col);
			dcomplex at(const unsigned row, const unsigned col) { return _data(_length - _offside + Index.at(col) - Index.at(row)); }

			void padding(const VectorXcd& input, VectorXcd& output)const;
			void unpadding(const VectorXcd& input, VectorXcd& output) const;
			unsigned layer() const{return _layer;}
			unsigned rows() const{return _row;}
			unsigned cols() const{return _col;}
			unsigned Length()const { return _length; }
			unsigned Offside()const { return _offside; }
			VectorXi id_pos(const unsigned val)const;

			std::vector<int>Index;
		private:
			void constructIterated(const VectorXcd& row, const unsigned level, const unsigned bias = 0);
			
			VectorXi _layerNum;//Layer[3]={xNum,yNum,zNum}
			VectorXi _vecN;//2*N-1
			VectorXi _vecES;//vector element size for layer
			VectorXi _ES;//element size for layer
			unsigned _layer;
			unsigned _length;
			unsigned _row;
			unsigned _col;
			unsigned _offside;
			VectorXcd _data;
			
		};

	}

}
