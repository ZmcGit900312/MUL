#include "stdafx.h"
#include "Mesh.h"

using namespace std;

Mesh Mesh::_instance;
Mesh::~Mesh()
{
	_grids.clear();
	_segments.clear();
	_triangles.clear();
	_cuboids.clear();
	_tetrahedras.clear();
	

	_grids.shrink_to_fit();
	_segments.shrink_to_fit();
	_triangles.shrink_to_fit();
	_cuboids.shrink_to_fit();
	_tetrahedras.shrink_to_fit();

	if(!TriangleMock.empty())
	{
		for (auto element : TriangleMock)delete element;
		TriangleMock.clear();
		TriangleMock.shrink_to_fit();
	}
}

void Core::Mesh::Erase()
{
	_grids.clear();
	_segments.clear();
	_triangles.clear();
	_cuboids.clear();
	_tetrahedras.clear();

	if (!TriangleMock.empty())
	{
		for (auto element : TriangleMock)delete element;
		TriangleMock.clear();
		TriangleMock.shrink_to_fit();
	}
}

/**
 * \brief 计算所有三角形的求积点
 * \param Q4 标准三角形4求积点
 * \param Q7 标准三角形7求积点
 * \param Q13 标准三角形13求积点
 * \return 是否完成求积点计算
 */
bool TriangleQuad(Mesh * mesh, double Q4[3][4], double Q7[3][7], double Q13[3][13])
{
	MatrixXd q4(3, 4), q7(3, 7), q13(3, 13);
	for (int row = 0; row < 3; ++row)
	{
		for (int col = 0; col < 4; ++col)q4(row, col) = Q4[row][col];
		for (int col = 0; col < 7; ++col)q7(row, col) = Q7[row][col];
		for (int col = 0; col < 13; ++col)q13(row, col) = Q13[row][col];
	}

	for (auto i = mesh->TriangleMock.begin(),ed= mesh->TriangleMock.end(); i != ed; ++i)
	{
		MatrixXd temp4 = (*i)->Vertex()*q4;
		MatrixXd temp7 = (*i)->Vertex()*q7;
		MatrixXd temp13 = (*i)->Vertex()*q13;
		for (short col = 0; col < 4; ++col)(*i)->Quad4()(col) = temp4.col(col);
		for (short col = 0; col < 7; ++col)(*i)->Quad7()(col) = temp7.col(col);
		for (short col = 0; col < 13; ++col)(*i)->Quad13()(col) = temp13.col(col);
	}

	return true;
}
