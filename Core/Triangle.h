///////////////////////////////////////////////////////////
//  Triangle.h
//  Implementation of the Class Triangle
//  Created on:      28-12月-2017 17:23:53
//  Original author: ZhengMX
///////////////////////////////////////////////////////////

#pragma once
#include <Eigen/Core>
#include <Eigen/Geometry>//包含cross操作
#include <map>

using namespace Eigen;
using namespace std;

namespace Core
{

	class Triangle
	{
	public:
		Triangle() = default;
		Triangle(int id[], const Vector3d a, const Vector3d b, const Vector3d c)
		{
			_id = id[0];
			_material = id[1];
			_vertex << a, b, c;
			_centre = (a + b + c) / 3.0;
			_nodeToEdge[0] = { id[2],-1 };
			_nodeToEdge[1] = { id[3],-1 };
			_nodeToEdge[2] = { id[4],-1 };
			_node[0] = a ;
			_node[1]=b;
			_node[2] = c ;
				
			_edge[0]={c-b,(c - b).norm()};	
			_edge[1] = {a-c,(a - c).norm()};
			_edge[2] = {b-a,(b - a).norm()};

			
			_normal = (_edge[2].first).cross(_edge[0].first);
			_area = _normal.norm()*0.5;
			_normal.normalize();
		}

		~Triangle() { }

		int ID() const { return _id; }
		double Area() const { return _area; }
		int Material() const { return _material; }

		pair<Vector3d,double> Edge(const short val)const{return _edge[val];}
		Vector3d Normal() const { return _normal; }
		Vector3d Centre()const { return _centre; }
		//矩阵形式三顶点
		Matrix3d Vertex() const { return _vertex; }
		
		dcomplex& Z(const short val){return _selfImp[val];}
		dcomplex& Z(const size_t i,const size_t j)
		{
			int p1 = -1, p2 = -1;
			for (int zmc = 0; zmc < 3; ++zmc)
			{
				if (i == _nodeToEdge[zmc].second)p1 = zmc;
				if (j == _nodeToEdge[zmc].second)p2 = zmc;
			}
			return p1 == p2 ?_selfImp[p1]:_selfImp[p1+p2+2];
		}
		//数组形式三顶点
		Vector3d& Node(const int index) { return _node[index]; }
		pair<int, size_t>& ID(const int index) { return _nodeToEdge[index]; }
		//Quadrature
		Matrix<Vector3d, 1, 4>& Quad4() { return _quad4; }
		Matrix<Vector3d, 1, 7>& Quad7() { return _quad7; }
		Matrix<Vector3d, 1, 13>& Quad13() { return _quad13; }

		int RWGSign[3] = { 0,0,0 };
	private:
		
		Vector3d _normal{ 0,0,0 },_centre;
		Matrix3d _vertex;
		Vector3d _node[3];
		pair<int, size_t> _nodeToEdge[3];
		pair<Vector3d,double> _edge[3];
		Matrix<Vector3d, 1, 4> _quad4;
		Matrix<Vector3d, 1, 7> _quad7;
		Matrix<Vector3d, 1, 13> _quad13;
		// 0 3 4
		// 3 1 5
		// 4 5 2
		dcomplex _selfImp[6] = { 0 };
		int _id = 0, _material = 0;
		double _area = 0;
	};

}

