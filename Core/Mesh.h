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
		size_t NodeSize() const { return _grids.size(); }
		size_t SegmentSize() const { return SegmentVector.size(); }
		size_t TriangleSize() const { return TriangleVector.size(); }
		size_t CuboidSize()const { return CuboidVector.size(); }
		size_t TetrahedraSize()const { return TriangleVector.size(); }
			
		Vector3d& GetNode(const int index) { return _grids[index]; }
		Segment* GetSegment(const int index) { return SegmentVector[index]; }
		//Triangle& GetTriangle(const int val) { return _triangles[val]; }
		Cuboid* GetCuboid(const int index) { return CuboidVector[index]; }
		Tetrahedra* GetTetrahedra(const int index) { return TetrahedraVector[index]; }
		Triangle* GetTriangle(const int index) { return TriangleVector[index]; }
		
		vector<Vector3d>* GridVector() { return &_grids; }
		vector<Segment*> SegmentVector;
		vector<Cuboid*> CuboidVector;
		vector<Tetrahedra*> TetrahedraVector;
		vector<Triangle*> TriangleVector;
	private:
		Mesh() = default;
		Mesh operator=(Mesh&) = delete;
		Mesh(Mesh&) = delete;

		bool _lock = true;
		//Data Vector
		vector<Vector3d> _grids;

		static Mesh _instance;
	};

	bool TriangleQuad(Mesh *mesh, const double q4[3][4], const double q7[3][7], const double q13[3][13]);
}


