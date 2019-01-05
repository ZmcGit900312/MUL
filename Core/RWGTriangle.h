#ifndef RWGTRIANGLE_H
#define RWGTRIANGLE_H
#include "Triangle.h"

using namespace std;

namespace Core
{
	
	class RWG;
	class RWGTriangle :public Core::Triangle
	{
	public:
		explicit RWGTriangle(const Vector3d& pt1, const Vector3d& pt2, const Vector3d& pt3, size_t id, size_t nodeId1, size_t nodeId2, size_t nodeId3, int material, int entity) :
			Triangle(pt1, pt2, pt3, id, nodeId1, nodeId2, nodeId3, material, entity){}


		~RWGTriangle();

		dcomplex& Z(const short val) { return _selfImp[val].second; }
		dcomplex& Z(const size_t i, const size_t j);
		dcomplex& Z(const size_t val);//还有问题的，关于selfTriangleSet
		size_t RWGID(const int val)const;
		

		RWG*Rn[3]={nullptr,nullptr,nullptr};
		int RWGSign[3] = { 0,0,0 };
		
	private:

		// 0 3 4
		// 3 1 5
		// 4 5 2
		pair<size_t, dcomplex>_selfImp[6];
		//dcomplex  = { 0 };
	};

}

#endif