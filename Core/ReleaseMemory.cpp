#include"stdafx.h"
#include "CoreAPI.h"
#include "Data.h"
#include "Log.h"

int Core::ReleaseMemory()
{
	
	cout << "\n";
	Console->info("{:*^70}","Release the memory");
	Runtime->info("Release the memory");
	if (ComponentList::BFvector.size()>0)
	{
		for (auto element : ComponentList::BFvector) { delete element; }
		ComponentList::BFvector.clear();
		ComponentList::BFvector.shrink_to_fit();
		Runtime->debug("Release BasicFunction");
	}
	if (ComponentList::ImpService)
	{
		delete ComponentList::ImpService;
		ComponentList::ImpService = nullptr;
		Runtime->debug("Release Matrix");
	}
	if (Tools::TeoplitzMultiplicator)
	{
		delete Tools::TeoplitzMultiplicator;
		Tools::TeoplitzMultiplicator = nullptr;
		Runtime->debug("Release TeoplitzMultiplicator");
	}
	if(SystemConfig.PostConfig.size()>0)
	{
		SystemConfig.PostConfig.clear();
		Runtime->debug("Release Request");
	}
	if(SystemConfig.SourceConfig)
	{
		delete SystemConfig.SourceConfig;
		SystemConfig.SourceConfig = nullptr;
		Runtime->debug("Release Source");
	}
	if(Core::equation)
	{
		delete equation;
		equation = nullptr;
		Runtime->debug("Release IE");
	}
	Console->info("{:-^80}","END");
	
	if(ResultLog)
	{
		ResultLog->info("{:-^80}", "END");
		ResultLog->flush();
	}
	
	Runtime->info("{:-^80}\n", "END");
	Runtime->flush();
	return 0;
}
