#include "stdafx.h"
#include "CoreAPI.h"
#include "IImpedance.h"
#include "Data.h"
#include "Log.h"


int Core::FillImpedance()
{
	cout << "\n";
	Console->info("{:*^45}","Fill Impedance Matrix");
	Runtime->info("{:*^45}","Fill Impedance Matrix");
	
	ComponentList::ImpService->FillImpedance();
	
	Console->info("Whole Filling costs\t{:f} s", ComponentList::ImpService->GetTime());
	Console->info("Memory Cost\t{:f} Mb", ComponentList::ImpService->GetMemoryCost());
	ResultLog->info("Whole Filling costs\t{:f} s", ComponentList::ImpService->GetTime());
	ResultLog->info("Memory Cost\t{:f} Mb", ComponentList::ImpService->GetMemoryCost());
	Runtime->info("Whole Filling costs\t{:f} s", ComponentList::ImpService->GetTime());
	Runtime->info("Memory Cost\t{:f} Mb", ComponentList::ImpService->GetMemoryCost());
	Runtime->flush();
	return 0;
}
