#include "stdafx.h"
#include "CoreAPI.h"
#include "IImpedance.h"
#include "Data.h"
#include "Log.h"
#include "Excitation.h"
#include "ResultReport.h"

int Core::SetRightHand()
{
	cout << '\n';

	ResultReport::WriteExcitationInformation(SystemConfig.SourceConfig, Console);
	ResultReport::WriteExcitationInformation(SystemConfig.SourceConfig, RuntimeLog);

	const clock_t start = clock();
	ComponentList::ImpService->GetExcitation() = SystemConfig.SourceConfig->SetExcitation(ComponentList::BFvector,SystemConfig.ImpConfig);
	const clock_t end = clock();

	ResultReport::WriteExcitationInformation(SystemConfig.SourceConfig,ResultLog);

	const double time = double(end - start) / CLOCKS_PER_SEC;
	const double memory = double(16*ComponentList::BFvector.size()) / 1048576;

	
	Console->info("Set RightHand is cost:\t{:f} s", time);
	RuntimeLog->info("Set RightHand is cost:\t{:f} s",time);
	ResultLog->info("Set RightHand is cost:\t{:f} s", time);

	Console->info("Memory Cost\t{0:f} Mb", memory);
	RuntimeLog->info("Memory Cost\t{0:f} Mb", memory);
	ResultLog->info("Memory Cost\t{0:f} Mb", memory);

	RuntimeLog->flush();
	return 0;
}
