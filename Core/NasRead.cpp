///////////////////////////////////////////////////////////
//  NasRead.cpp
//  Implementation of the Class NasRead
//  Created on:      26-11ÔÂ-2018 15:53:59
//  Original author: ZhengMX
///////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NasRead.h"
#include "Log.h"

bool NasRead::Read(const char* fileName, Mesh* mesh){
	if (!fileName)throw spdlog::spdlog_ex("MeshFile is nullptr!");
	//if (!meshFile)return false;
	ifstream fp(fileName, ios::in);

	if (fp.is_open())
	{//read the mesh
		mesh->Erase();

		mesh->GridVector()->reserve(10000);
		mesh->TriangleVector()->reserve(10000);

		string buffer;
		double x = 0, y = 0, z = 0;
		int triID[5], nodeID = 0;
		const clock_t start = clock();
		while (fp.good() && !fp.eof())
		{
			getline(fp, buffer);

			switch (buffer[0])
			{
			case 'G'://Grids
				nodeID = stoi(buffer.substr(6, 20));
				x = stod(buffer.substr(40, 16));
				y = stod(buffer.substr(56, 16));
				getline(fp, buffer);
				z = stod(buffer.substr(8, 16));
				mesh->GridVector()->push_back(Vector3d(x, y, z));
				break;
			case 'C'://patches
				triID[0] = stoi(buffer.substr(6, 10)) - 1;//ID
				triID[1] = stoi(buffer.substr(16, 8));//Material
				triID[2] = stoi(buffer.substr(24, 8)) - 1;//node a
				triID[3] = stoi(buffer.substr(32, 8)) - 1;//node b
				triID[4] = stoi(buffer.substr(40, 8)) - 1;//node c
				mesh->TriangleVector()->push_back(Triangle(triID, mesh->GetNode(triID[2]), mesh->GetNode(triID[3]), mesh->GetNode(triID[4])));
				break;
			default:
				break;
			}
		}
		const clock_t end = clock();
		fp.clear();
		fp.close();
		double time = double(end - start) / CLOCKS_PER_SEC;
		//_memoryCost = (16.0*_numGrids + 120.0*_numTriangles) / 1024 / 1024;
		mesh->SetLock(false);
		return true;
	}
	throw spdlog::spdlog_ex("Failed to open meshFile " + string(fileName));
	//END
}


bool NasRead::Write(const char* fileName, Mesh* mesh){

	return false;
}