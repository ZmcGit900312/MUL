#include "stdafx.h"
#include "CoreAPI.h"
#include "IImpedance.h"
#include "Data.h"
#include "Log.h"


int Core::FillImpedance()
{
	
	ComponentList::ImpService->FillImpedance();
	
	Console->info("Whole Filling costs\t{:f} s", ComponentList::ImpService->GetTime());
	Console->info("Memory Cost\t{:f} Mb", ComponentList::ImpService->GetMemoryCost());
	ResultLog->info("Whole Filling costs\t{:f} s", ComponentList::ImpService->GetTime());
	ResultLog->info("Memory Cost\t{:f} Mb", ComponentList::ImpService->GetMemoryCost());
	return 0;
}
