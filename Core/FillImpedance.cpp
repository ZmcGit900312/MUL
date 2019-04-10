#include "stdafx.h"
#include "CoreAPI.h"
#include "IImpedance.h"
#include "Data.h"
#include "Log.h"


int Core::FillImpedance()
{
	cout << "\n";
	Console->info("{:*^45}","Fill Impedance Matrix");
	RuntimeL->info("{:*^45}","Fill Impedance Matrix");
	
	ComponentList::ImpService->FillImpedance();
	
	Console->info("Whole Filling costs\t{:f} s", ComponentList::ImpService->GetTime());
	Console->info("Memory Cost\t{:f} Mb", ComponentList::ImpService->GetMemoryCost());
	ResultL->info("Whole Filling costs\t{:f} s", ComponentList::ImpService->GetTime());
	ResultL->info("Memory Cost\t{:f} Mb", ComponentList::ImpService->GetMemoryCost());
	RuntimeL->info("Whole Filling costs\t{:f} s", ComponentList::ImpService->GetTime());
	RuntimeL->info("Memory Cost\t{:f} Mb", ComponentList::ImpService->GetMemoryCost());
	RuntimeL->flush();
	return 0;
}
