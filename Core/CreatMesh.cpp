#include "stdafx.h"
#include "CoreAPI.h"
#include "IMeshRead.h"
#include "NasRead.h"
#include "Const.h"
#include "Log.h"
#include "ModalGeometry.h"
#include "Data.h"

int Core::CreatMesh()
{
	cout << "\n";
	Console->info("Creat Mesh");
	RuntimeL->info("Creat Mesh");
	try
	{
		IMeshRead *reader = new NasRead;//Here should be changed
		Mesh* mesh = Mesh::GetInstance();

		const clock_t start = clock();
		if (!reader->Read(SystemConfiguration::MeshFileName.c_str(), mesh))throw spdlog::spdlog_ex("Initial Mesh Failure!");
		const clock_t end = clock();
		double time = double(end - start) / CLOCKS_PER_SEC;
		//Initial Geometry
		Assist::ModalGeometry::GetInstance()->SetLimitationBoundary(mesh);
		
		Console->info("Calculate the quadratrue for each triangles...");

		if (!TriangleQuad(mesh,Quad4, Quad7, Quad13))
			throw spdlog::spdlog_ex("Calcuate the Quadrature Error,Please check the TriagnleQuad");

		Console->info("Creat Mesh costs\t{:f} s",time);
		ResultL->info("Creat Mesh costs\t{:f} s", time);
		/*Console->info("Memory Cost\t{:f} Mb", GetMemoryCost());
		
		ResultL->info("Memory Cost\t{:f} Mb", GetMemoryCost());*/
		RuntimeL->info("Finish Creat Mesh");
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
