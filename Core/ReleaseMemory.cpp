#include"stdafx.h"
#include "CoreAPI.h"
#include "Data.h"
#include "Log.h"

int Core::ReleaseMemory()
{
	
	cout << "\n";
	Console->info("{:*^45}","Release the memory");
	RuntimeL->info("{:*^45}", "Release the memory");
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
	if(SystemConfig.PostConfig.size()>0)
	{
		SystemConfig.PostConfig.clear();
		RuntimeL->info("Release Request");
	}
	
	Console->info("{:-^45}","END");
	ResultL->info("{:-^45}", "END");
	ResultL->flush();
	RuntimeL->info("{:-^45}", "END");
	RuntimeL->flush();
	return 0;
}
