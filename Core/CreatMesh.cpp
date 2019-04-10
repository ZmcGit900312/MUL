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
	RuntimeL->info("{:*^45}", "Mesh");
	try
	{
#pragma region Mesh
		IMeshRead *reader = new NasRead;//Here should be changed
		Mesh* mesh = Mesh::GetInstance();

		const clock_t start = clock();
		if (!reader->Read(SystemConfig.MeshFileName.c_str(), mesh))
			throw spdlog::spdlog_ex("Initial Mesh Failure!");
		const clock_t end = clock();
		double time = double(end - start) / CLOCKS_PER_SEC;

		Console->info("Creat Mesh costs\t{:f} s", time);
		RuntimeL->info("Creat Mesh costs\t{:f} s", time);
		ResultReport::WriteMeshInformation(mesh, SystemConfig.MeshFileName);
		ResultL->info("Creat Mesh costs\t{:f} s\n", time);
#pragma endregion 

#pragma region Geometry
		//Initial Geometry
		auto geo = Assist::ModalGeometry::GetInstance();
		geo->SetLimitationBoundary(mesh);
		
		Console->info("{:*^45})", "Geometry Info");
		Console->info("Lower Point of Box is ({0:+5.3f},{1:+5.3f},{2:+5.3f})",
			geo->GetLimitationBoundary(0).x(),
			geo->GetLimitationBoundary(0).y(),
			geo->GetLimitationBoundary(0).z());
		Console->info("Upper Point of Box is ({0:+5.3f},{1:+5.3f},{2:+5.3f})",
			geo->GetLimitationBoundary(7).x(),
			geo->GetLimitationBoundary(7).y(),
			geo->GetLimitationBoundary(7).z());

		RuntimeL->info("{:*^45})", "Geometry Info");
		RuntimeL->info("Lower Point of Box is ({0:+5.3f},{1:+5.3f},{2:+5.3f})",
			geo->GetLimitationBoundary(0).x(),
			geo->GetLimitationBoundary(0).y(),
			geo->GetLimitationBoundary(0).z());
		RuntimeL->info("Upper Point of Box is ({0:+5.3f},{1:+5.3f},{2:+5.3f}\n)",
			geo->GetLimitationBoundary(7).x(),
			geo->GetLimitationBoundary(7).y(),
			geo->GetLimitationBoundary(7).z());

		ResultReport::WriteGeometeryInformation(Assist::ModalGeometry::GetInstance());
#pragma endregion 
		Console->info("Calculate the quadratrue for each triangles...");
		
		if (!TriangleQuad(mesh,Quad4, Quad7, Quad13))
			throw spdlog::spdlog_ex("Calcuate the Quadrature Error,Please check the TriagnleQuad");	
		/*Console->info("Memory Cost\t{:f} Mb", GetMemoryCost());	
		ResultL->info("Memory Cost\t{:f} Mb", GetMemoryCost());*/
		RuntimeL->flush();
		
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
