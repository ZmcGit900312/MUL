#pragma once
#include <Eigen\Core>
#include "Segment.h"
#include "Triangle.h"
#include "Cuboid.h"
#include "Tetrahedra.h"
#include <vector>
using namespace std;
using namespace Eigen;
using namespace Core;

namespace Core
{
	class Mesh
	{
		public:
		static Mesh* GetInstance() { return &_instance; }
		virtual ~Mesh();

		/**
		 * \brief Erase all the vector
		 */
		void Erase();

		//Locker
		bool IsLock()const { return _lock; }
		void SetLock(bool val){_lock = val;}
		
		//feature
		size_t GetNode() const { return _grids.size(); }
		size_t GetSegment() const { return _segments.size(); }
		size_t GetTriangle() const { return TriangleMock.size(); }
		size_t GetCuboid()const { return _cuboids.size(); }
		size_t GetTetrahedra()const { return _tetrahedras.size(); }
			
		Vector3d& GetNode(const int val) { return _grids[val]; }
		Segment& GetSegment(const int val) { return _segments[val]; }
		//Triangle& GetTriangle(const int val) { return _triangles[val]; }
		Cuboid& GetCuboid(const int val) { return _cuboids[val]; }
		Tetrahedra& GetTetrahedra(const int val) { return _tetrahedras[val]; }
		Triangle* GetTriangle(const int val) { return TriangleMock[val]; }
		
		vector<Vector3d>* GridVector() { return &_grids; }
		vector<Segment>* SegmentVector() { return &_segments; }
		//vector<Triangle>* TriangleVector() { return &_triangles; }
		vector<Cuboid>* CuboidVector() { return &_cuboids; }
		vector<Tetrahedra>* TetrahedraVector() { return &_tetrahedras; }
		vector<Triangle*> TriangleMock;
	private:
		Mesh() = default;
		Mesh operator=(Mesh&) = delete;
		Mesh(Mesh&) = delete;

		bool _lock = true;
		//Data Vector
		vector<Vector3d> _grids;
		vector<Segment> _segments;
		vector<Triangle> _triangles;
		vector<Cuboid> _cuboids;
		vector<Tetrahedra>_tetrahedras;

		static Mesh _instance;
	};
}


bool TriangleQuad(Mesh *mesh,double q4[3][4], double q7[3][7], double q13[3][13]);