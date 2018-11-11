#pragma once
#include <Eigen\Core>
#include "Segment.h"
#include "Triangle.h"
#include "Cuboid.h"
#include "Tetrahedra.h"
#include <map>
#include <vector>
using namespace std;
using namespace Eigen;
using namespace Core;

namespace Core
{
	class Mesh
	{
		public:
		Mesh(const char * pf=nullptr){ _lock = SetMesh(pf); }
		virtual ~Mesh();
		
		/**
		 * \brief 填充Mesh类
		 * \param meshFile 读取文件路径
		 * \return 是否填充成功
		 */
		bool SetMesh(const char * meshFile);

		//API
		/**
		 * \brief 查询是否可以使用
		 * \return true(可以使用)false(锁定状态)
		 */
		bool IsLock() const { return _lock; }
		double GetTime() const { return _time; }
		double GetMemoryCost() const { return _memoryCost; }
		bool TriangleQuad(double q4[3][4], double q7[3][7], double q13[3][13]);
		//feature
		int GetNode() const { return _numGrids; }
		int GetSegment() const { return _numSegments; }
		int GetTriangle() const { return _numTriangles; }
		int GetCuboid()const { return _numCuboids; }
		int GetTetrahedra()const { return _numTetrhedras; }
			
		Vector3d& GetNode(const int val) { return _grids[val]; }
		Segment& GetSegment(const int val) { return _segments[val]; }
		Triangle& GetTriangle(const int val) { return _triangles[val]; }
		Cuboid& GetCuboid(const int val) { return _cuboids[val]; }
		Tetrahedra& GetTetrahedra(const int val) { return _tetrahedras[val]; }

		
		vector<Vector3d>* GridVector() { return &_grids; }
		vector<Segment>* GetSegmentVector() { return &_segments; }
		vector<Triangle>* TriangleVector() { return &_triangles; }
		vector<Cuboid>* GetCuboidVector() { return &_cuboids; }
		vector<Tetrahedra>* GetTetrahedraVector() { return &_tetrahedras; }
	private:
		int _numGrids = 0, _numSegments=0,_numTriangles = 0,
		_numCuboids=0,_numTetrhedras=0;
		vector<Vector3d> _grids;
		vector<Segment> _segments;
		vector<Triangle> _triangles;
		vector<Cuboid> _cuboids;
		vector<Tetrahedra>_tetrahedras;
		double _time = 0.0, _memoryCost = 0.0;
		bool _lock = true;
	};
}


