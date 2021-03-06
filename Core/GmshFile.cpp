#include "stdafx.h"
#include "GmshFile.h"
#include "Log.h"
#include "RWG.h"


bool GmshFile::Read(const char* fileName, Mesh* mesh)
{
	if (!fileName)throw spdlog::spdlog_ex("MeshFile is nullptr!");
	//if (!meshFile)return false;
	ifstream fp(fileName, ios::in);

	if (!fp.is_open())throw spdlog::spdlog_ex("Failed to open meshFile " + string(fileName));
	//read the mesh
	string version,buffer;
	int filetype = 0, datasize = 8;
	mesh->Erase();

	const clock_t start = clock();
	while(fp>>buffer)
	{
		if (buffer.substr(0, 4) == "$End")continue;
		//Format
		if (buffer == "$MeshFormat") {
			fp >> version >> filetype >> datasize;
			continue;
		}
		//Physical
		if (buffer == "$PhysicalNames")continue;
		//Node
		if (buffer == "$Nodes")
		{
			size_t nodeNum = 0;
			fp >> nodeNum;
			mesh->GridVector()->reserve(nodeNum);
			double x = 0, y = 0, z = 0;
			size_t nodeId = 0;
			for (size_t zmc = 0;zmc < nodeNum;zmc++)
			{
				fp >> nodeId >> x >> y >> z;
				mesh->GridVector()->push_back(Vector3d(x, y, z));
			}
			continue;
		}
		//Elements
		if (buffer == "$Elements")
		{
			size_t eleNum = 0;
			fp >> eleNum;
			mesh->TriangleVector.reserve(eleNum);
			size_t nodeNum = 0, lineNum = 0, TriangleNum = 0;
			int eleType, physicType, entityType, geoID;

			for (size_t zmc = 0;zmc < eleNum;)
			{
				fp >> zmc >> eleType >> physicType >> entityType >> geoID;
				if (eleType == 15)
				{
					fp >> nodeNum;//skip
					continue;//Node
				}

				if (eleType == 1)
				{
					fp >> lineNum >> lineNum;//Line
					continue;
				}

				if (eleType == 2)//Triangle
				{
					size_t node1, node2, node3;
					fp >> node1 >> node2 >> node3;
					node1 -= 1, node2 -= 1, node3 -= 1;

					mesh->TriangleVector.push_back(
						new RWGTriangle(mesh->GetNode(node1), mesh->GetNode(node2), mesh->GetNode(node3), TriangleNum++, node1, node2, node3, physicType, geoID));
				}

			}
			continue;
		}

		if (buffer == "$Periodic")continue;

		if (buffer == "$NodeData")continue;

		if (buffer == "$ElementData")continue;

		if (buffer == "$ElementNodeData")continue;

		if (buffer == "$InterpolationScheme")continue;
		 
	}


	const clock_t end = clock();
	
	fp.clear();
	fp.close();
	double time = double(end - start) / CLOCKS_PER_SEC;
	//_memoryCost = (16.0*_numGrids + 120.0*_numTriangles) / 1024 / 1024;
	mesh->SetLock(false);
	return true;
	
}

bool GmshFile::Write(const char* fileName, Mesh* mesh)
{
	throw spdlog::spdlog_ex("This function is not implemented!");
}
