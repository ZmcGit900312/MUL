#include "stdafx.h"
#include "Mesh.h"
#include <fstream>
#include <string>
#include <ctime>
#include "Log.h"

using namespace std;

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
}

bool Mesh::SetMesh(const char* meshFile)
{
	if (!meshFile)throw spdlog::spdlog_ex("MeshFile is nullptr!");
	//if (!meshFile)return false;
	ifstream fp(meshFile,ios::in);

	if (fp.is_open())
	{//read the mesh
		string buffer;
		double x=0, y=0, z=0;
		int triID[5],nodeID=0;
		const clock_t start = clock();
		while (fp.good()&&!fp.eof())
		{
			getline(fp,buffer);

			switch (buffer[0])
			{
			case 'G'://Grids
				nodeID = stoi(buffer.substr(6,20));
				x=stod(buffer.substr(40, 16));
				y = stod(buffer.substr(56, 16));
				getline(fp, buffer);
				z = stod(buffer.substr(8, 16));
				_grids.push_back(Vector3d(x, y, z));
				break;
			case 'C'://patches
				triID[0] = stoi(buffer.substr(6,10))-1;//ID
				triID[1] = stoi(buffer.substr(16, 8));//Material
				triID[2] = stoi(buffer.substr(24, 8))-1;//node a
				triID[3] = stoi(buffer.substr(32, 8))-1;//node b
				triID[4] = stoi(buffer.substr(40, 8))-1;//node c
				_triangles.push_back(Triangle(triID, GetNode(triID[2]), GetNode(triID[3]), GetNode(triID[4])));
				break;
			default:
				break;
			}
		}
		const clock_t end = clock();
		fp.clear();
		fp.close();
		_numGrids = (int)_grids.size();
		_numTriangles =(int) _triangles.size();
		_time = double(end - start) / CLOCKS_PER_SEC;
		_memoryCost = (16.0*_numGrids + 120.0*_numTriangles) / 1024 / 1024;
		_lock = false;	
		return _lock;
	}
	throw spdlog::spdlog_ex("Failed to open meshFile " + string(meshFile));
	//END
}

/**
 * \brief 计算所有三角形的求积点
 * \param Q4 标准三角形4求积点
 * \param Q7 标准三角形7求积点
 * \param Q13 标准三角形13求积点
 * \return 是否完成求积点计算
 */
bool Mesh::TriangleQuad(double Q4[3][4], double Q7[3][7], double Q13[3][13])
{
	if (_lock)throw spdlog::spdlog_ex("Fail: Please set mesh first");
	MatrixXd q4(3, 4),q7(3,7),q13(3,13);
	for (int row = 0; row < 3; ++row)
	{
		for (int col = 0; col < 4; ++col)q4(row, col) = Q4[row][col];
		for (int col = 0; col < 7; ++col)q7(row, col) = Q7[row][col];
		for (int col = 0; col < 13; ++col)q13(row, col) = Q13[row][col];
	}
	
	for (auto i = _triangles.begin(); i != _triangles.end(); ++i)
	{
		MatrixXd temp4=i->Vertex()*q4;
		MatrixXd temp7 = i->Vertex()*q7;
		MatrixXd temp13 = i->Vertex()*q13;
		for (short col = 0; col < 4; ++col)i->Quad4()(col) = temp4.col(col);
		for (short col = 0; col < 7; ++col)i->Quad7()(col) = temp7.col(col);
		for (short col = 0; col < 13; ++col)i->Quad13()(col) = temp13.col(col);
	}

	return true;
}
