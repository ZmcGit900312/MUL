#include "stdafx.h"
#include "CoreAPI.h"
#include "IImpedance.h"
#include "Data.h"
#include "Log.h"


int Core::FillImpedance()
{
	cout << "\n";
	Console->info("Fill Impedance Matrix");
	ResultL->info("\nFill Impedance");
	RuntimeL->info("Run FillImpedance()");
	
	ComponentList::ImpService->FillImpedance();
	
	Console->info("Whole Filling costs\t{:f} s", ComponentList::ImpService->GetTime());
	Console->info("Memory Cost\t{:f} Mb", ComponentList::ImpService->GetMemoryCost());
	ResultL->info("Whole Filling costs\t{:f} s", ComponentList::ImpService->GetTime());
	ResultL->info("Memory Cost\t{:f} Mb", ComponentList::ImpService->GetMemoryCost());
	RuntimeL->info("Finish");
	RuntimeL->flush();
	return 0;
}
