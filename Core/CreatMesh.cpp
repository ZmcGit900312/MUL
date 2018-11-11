#include "stdafx.h"
#include "CoreAPI.h"
#include "Mesh.h"
#include "Data.h"
#include "Const.h"
#include "Log.h"

int Core::CreatMesh()
{
	cout << "\n";
	Console->info("Creat Mesh");
	RuntimeL->info("Creat Mesh");
	try
	{
		ComponentList::MeshService = new Mesh(SystemConfiguration::MeshFileName.c_str());

		if (ComponentList::MeshService->IsLock())throw spdlog::spdlog_ex("Mesh is lock!");
		//Initial Geometry
		ComponentList::Geometry.SetLimitationBoundary(ComponentList::MeshService);
		
		Console->info("Calculate the quadratrue for each triangles...");

		if (!ComponentList::MeshService->TriangleQuad(Quad4, Quad7, Quad13))
			throw spdlog::spdlog_ex("Calcuate the Quadrature Error,Please check the mesh.TriagnleQuad");

		Console->info("Creat Mesh costs\t{:f} s",ComponentList::MeshService->GetTime());
		Console->info("Memory Cost\t{:f} Mb", ComponentList::MeshService->GetMemoryCost());
		ResultL->info("Creat Mesh costs\t{:f} s", ComponentList::MeshService->GetTime());
		ResultL->info("Memory Cost\t{:f} Mb", ComponentList::MeshService->GetMemoryCost());
		RuntimeL->info("Finish Creat Mesh");
		RuntimeL->flush();
		/*cout << "Finish\nCreat Mesh costs\t" << ComponentList::MeshService->GetTime() << " s\n"
				<< "And cost memory about\t" << ComponentList::MeshService->GetMemoryCost() << " Mb\n";*/
		return 0;
	}
	catch(runtime_error error)
	{
		cout <<"\nException in CreatMesh.cpp:\t"<< error.what() << endl;
		return 1;
	}
	catch(spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}
}
