#include"stdafx.h"
#include "CoreAPI.h"
#include "Data.h"
#include "Log.h"

int Core::ReleaseMemory()
{
	
	cout << "\n";
	Console->info("{:*^45}","Release the memory");
	RuntimeLog->info("{:*^45}", "Release the memory");
	if (ComponentList::BFvector.size()>0)
	{
		for (auto element : ComponentList::BFvector) { delete element; }
		ComponentList::BFvector.clear();
		ComponentList::BFvector.shrink_to_fit();
		RuntimeLog->info("Release BasicFunction");
	}
	if (ComponentList::ImpService)
	{
		delete ComponentList::ImpService;
		ComponentList::ImpService = nullptr;
		RuntimeLog->info("Release Matrix");
	}
	if (Tools::TeoplitzMultiplicator)
	{
		delete Tools::TeoplitzMultiplicator;
		Tools::TeoplitzMultiplicator = nullptr;
		RuntimeLog->info("Release TeoplitzMultiplicator");
	}
	if(SystemConfig.PostConfig.size()>0)
	{
		SystemConfig.PostConfig.clear();
		RuntimeLog->info("Release Request");
	}
	if(SystemConfig.SourceConfig)
	{
		delete SystemConfig.SourceConfig;
		SystemConfig.SourceConfig = nullptr;
	}
	Console->info("{:-^45}","END");
	ResultLog->info("{:-^45}", "END");
	ResultLog->flush();
	RuntimeLog->info("{:-^45}", "END");
	RuntimeLog->flush();
	return 0;
}
