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
		explicit Triangle(const Vector3d& pt1,const Vector3d& pt2,const Vector3d& pt3, size_t id=-1, size_t nodeId1=-1, size_t nodeId2=-1, size_t nodeId3=-1, int material=-1, int entity=-1):
		_centre((pt1 + pt2 + pt3) / 3.0),_id(id),_material(material),_entity(entity)
		{
			_node[0] = pt1, _node[1] = pt2, _node[2] = pt3;
			_nodeID[0] = nodeId1;
			_nodeID[1] = nodeId2;
			_nodeID[2] = nodeId3;
			_edge[0] = { pt3-pt2,(pt3 - pt2).norm() };
			_edge[1] = { pt1 - pt3,(pt1 - pt3).norm() };
			_edge[2] = { pt2 - pt1,(pt2 - pt1).norm() };
			_normal = _edge[2].first.cross(_edge[0].first);
			_area = _normal.norm()*0.5;
			_normal.normalize();
	
		}

		virtual ~Triangle() { }

		int ID() const { return _id; }
		double Area() const { return _area; }
		int Material() const { return _material; }
		int Entity()const { return _entity; }

		pair<Vector3d,double> Edge(const short val)const{return _edge[val];}
		Vector3d Normal() const { return _normal; }
		Vector3d Centre()const { return _centre; }
		//矩阵形式三顶点
		Matrix3d Vertex() const
		{
			Matrix3d vertex;
			vertex<< _node[0], _node[1], _node[2];
			return vertex;
		}
		//数组形式三顶点
		Vector3d& Node(const int index) { return _node[index]; }
		size_t NodeID(const int index) { return _nodeID[index]; }		
		//Quadrature
		Matrix<Vector3d, 1, 4>& Quad4() { return _quad4; }
		Matrix<Vector3d, 1, 7>& Quad7() { return _quad7; }
		Matrix<Vector3d, 1, 13>& Quad13() { return _quad13; }
	private:
		
		Vector3d _normal{ 0,0,0 },_centre;
		Vector3d _node[3];		
		pair<Vector3d,double> _edge[3];
		Matrix<Vector3d, 1, 4> _quad4;
		Matrix<Vector3d, 1, 7> _quad7;
		Matrix<Vector3d, 1, 13> _quad13;
		int _id = 0, _material = 0,_entity=-1;
		size_t _nodeID[3];
		double _area = 0;
	};

}

