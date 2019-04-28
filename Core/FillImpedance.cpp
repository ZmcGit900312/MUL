#include "stdafx.h"
#include "CoreAPI.h"
#include "IImpedance.h"
#include "Data.h"
#include "Log.h"


int Core::FillImpedance()
{
	cout << "\n";
	Console->info("{:*^45}","Fill Impedance Matrix");
	RuntimeLog->info("{:*^45}","Fill Impedance Matrix");
	
	ComponentList::ImpService->FillImpedance();
	
	Console->info("Whole Filling costs\t{:f} s", ComponentList::ImpService->GetTime());
	Console->info("Memory Cost\t{:f} Mb", ComponentList::ImpService->GetMemoryCost());
	ResultLog->info("Whole Filling costs\t{:f} s", ComponentList::ImpService->GetTime());
	ResultLog->info("Memory Cost\t{:f} Mb", ComponentList::ImpService->GetMemoryCost());
	RuntimeLog->info("Whole Filling costs\t{:f} s", ComponentList::ImpService->GetTime());
	RuntimeLog->info("Memory Cost\t{:f} Mb", ComponentList::ImpService->GetMemoryCost());
	RuntimeLog->flush();
	return 0;
}
