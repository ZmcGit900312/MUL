#include "stdafx.h"
#include "CoreAPI.h"
#include "IMeshRead.h"
#include "NasRead.h"
#include "Const.h"
#include "ResultReport.h"
#include "Log.h"
#include "ModalGeometry.h"
#include "Data.h"

int Core::CreatMesh()
{
	cout << "\n";
	Console->info("{:*^45}", "Mesh");
	Runtime->info("{:*^45}", "Mesh");
	try
	{
#pragma region Mesh
		IMeshRead *reader = new NasRead;//Here should be changed
		Mesh* mesh = Mesh::GetInstance();

		const clock_t start = clock();
		if (!reader->Read(SystemConfig.MeshFilePath.c_str(), mesh))
			throw spdlog::spdlog_ex("Initial Mesh Failure!");
		const clock_t end = clock();
		double time = double(end - start) / CLOCKS_PER_SEC;

		delete reader;

		ResultReport::WriteMeshInformation(mesh, SystemConfig.MeshFilePath, Console);
		ResultReport::WriteMeshInformation(mesh, SystemConfig.MeshFilePath, Runtime);		
		ResultReport::WriteMeshInformation(mesh, SystemConfig.MeshFilePath,ResultLog);
		Console->info("Creat Mesh costs\t{:f} s", time);
		Runtime->info("Creat Mesh costs\t{:f} s", time);
		ResultLog->info("Creat Mesh costs\t{:f} s\n", time);
#pragma endregion 
		cout << '\n';
#pragma region Geometry
		//Initial Geometry
		Assist::ModalGeometry::GetInstance()->SetLimitationBoundary(mesh);
				
		ResultReport::WriteGeometeryInformation(Assist::ModalGeometry::GetInstance(), Console);
		ResultReport::WriteGeometeryInformation(Assist::ModalGeometry::GetInstance(), Runtime);
		ResultReport::WriteGeometeryInformation(Assist::ModalGeometry::GetInstance(),ResultLog);
#pragma endregion 
		Console->info("Calculate the quadratrue for each triangles...");
		
		if (!TriangleQuad(mesh,Quad4, Quad7, Quad13))
			throw spdlog::spdlog_ex("Calcuate the Quadrature Error,Please check the TriagnleQuad");	
		/*Console->info("Memory Cost\t{:f} Mb", GetMemoryCost());	
		ResultLog->info("Memory Cost\t{:f} Mb", GetMemoryCost());*/
		Runtime->flush();
		
		return 0;
	}
	catch(runtime_error error)
	{
		cout <<"\nException in CreatMesh.cpp\t"<< error.what() << endl;
		return 1;
	}
	catch(spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}
}
