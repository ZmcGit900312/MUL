#include"stdafx.h"
#include "CoreAPI.h"
#include "Data.h"
#include "../include/spdlog/details/spdlog_impl.h"
#include "Log.h"

int Core::ReleaseMemory()
{
	RuntimeL->info("Run ReleaseMemory()");
	cout << "\n";
	Console->info("Release the memory");
	/*if (Mesh::GetInstance()) {
		delete Mesh::GetInstance(); Mesh::GetInstance() = nullptr;
		RuntimeL->info("Release Mesh");
	}*/
	if (ComponentList::BFvector.size()>0)
	{
		for (auto element : ComponentList::BFvector) { delete element; }
		ComponentList::BFvector.clear();
		ComponentList::BFvector.shrink_to_fit();
		RuntimeL->info("Release BasicFunction");
	}
	if (ComponentList::ImpService)
	{
		delete ComponentList::ImpService;
		ComponentList::ImpService = nullptr;
		RuntimeL->info("Release Matrix");
	}
	if (Tools::TeoplitzMultiplicator)
	{
		delete Tools::TeoplitzMultiplicator;
		Tools::TeoplitzMultiplicator = nullptr;
		RuntimeL->info("Release TeoplitzMultiplicator");
	}
	if(SystemConfiguration::PostConfig.size()>0)
	{
		SystemConfiguration::PostConfig.clear();
		RuntimeL->info("Release Request");
	}
	
	if (ComponentList::Solver)
	{
		delete ComponentList::Solver;
		ComponentList::Solver = nullptr;
		RuntimeL->info("Release Solver");
	}
	Console->info("END");
	ResultL->flush();
	RuntimeL->info("Finish");
	RuntimeL->flush();
	spdlog::drop_all();
	RuntimeL = nullptr;
	Console = nullptr;
	ResultL = nullptr;
	return 0;
}
