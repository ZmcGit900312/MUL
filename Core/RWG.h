///////////////////////////////////////////////////////////
//  RWG.h
//  Implementation of the Interface IIBasicFunction
//  Created on:      04-05月-2018 15:56:35
//  Original author: ZhengMX
///////////////////////////////////////////////////////////
#ifndef RWG_H
#define RWG_H
#include "IBasisFunction.h"
#include "RWGTriangle.h"
#include "Mesh.h"
using namespace std;

namespace Core {

	class RWG :public IBasisFunction
	{
	public:
		RWG(const size_t id,const pair<int, Vector3d> node[4], Triangle* tplus, Triangle* tminus);
		~RWG() { _tplus = nullptr, _tminus = nullptr; }

		//Get Private Variable API
		size_t GetID()const override { return _id; }
		double Length()const { return _edgeLength; }
		Vector3d Node(const int val)const { return _node[val].second; }
		Vector3d Centre()const { return _centre; }
		RWGTriangle* TrianglePlus()const{ return _tplus; }
		RWGTriangle* TriangleMinus() const { return _tminus; }
		dcomplex& Current()override { return _current; }

		//Current and Charge Function 
		Vector3d CurrentPlus(const Vector3d val)const override;
		Vector3d CurrentMinus(const Vector3d val)const override;
		double ChargePlus(const Vector3d)const override { return _edgeLength / _tplus->Area(); }
		double ChargeMinus(const Vector3d)const override { return -_edgeLength / _tminus->Area(); }
		int LimitPlus()const override { return _tplus->ID(); }
		int LimitMinus()const override { return _tminus->ID(); }
		
		/**
		* \brief 以二进制形式储存当下的RWG基函数信息
		* \n Format is [ID trianglePlusID triangleMinusID nodePlusID nodeMinusID nodeLeftID nodeRightID current]
		* \param ofs 写入的文件流
		*/
		void SaveBinary(ofstream &ofs)override;
		/**
		* \brief 静态方法：建立RWG基函数
		* \param mesh 剖分数据
		* \param RWGList 基函数序列
		* \return 基函数个数
		*/
		static size_t CreatRWGBasisFunctionList(Mesh* mesh,vector<IBasisFunction*>*RWGList);

	private:	
		size_t _id = 0;
		/**
		 * \brief Four nodes in one RWG
		 * plus		0
		 * minus	1
		 * left		2
		 * right	3
		 */
		pair<int, Vector3d> _node[4];
		Vector3d _centre;
		complex<double> _current{ 0,0 };
		double _edgeLength;
		RWGTriangle *_tplus, *_tminus;	
	};



}




#endif