#include "stdafx.h"
#include "CoreAPI.h"
#include "IImpedance.h"
#include "Data.h"
#include "Log.h"
#include "Planewave.h"
#include "ResultReport.h"

int Core::SetRightHand()
{
	cout << '\n';
	Console->info("{:*^45}","Excitation");
	Console->info("Plane wave Name::\t" + SystemConfig.SourceConfig.ExcitationName);
	RuntimeL->info("{:*^45}", "Excitation");
	RuntimeL->info("Plane wave Name::\t" + SystemConfig.SourceConfig.ExcitationName);	

	Source::Planewave source(&SystemConfig.SourceConfig);

	Console->info("Direction:[{0} {1} {2}]", SystemConfig.SourceConfig.Ki.x(), SystemConfig.SourceConfig.Ki.y(), SystemConfig.SourceConfig.Ki.z());
	Console->info("Polarization:[{0} {1} {2}]", SystemConfig.SourceConfig.Ei.x(), SystemConfig.SourceConfig.Ei.y(), SystemConfig.SourceConfig.Ei.z());
	RuntimeL->info("Direction:[{0} {1} {2}]", SystemConfig.SourceConfig.Ki.x(), SystemConfig.SourceConfig.Ki.y(), SystemConfig.SourceConfig.Ki.z());
	RuntimeL ->info("Polarization:[{0} {1} {2}]", SystemConfig.SourceConfig.Ei.x(), SystemConfig.SourceConfig.Ei.y(), SystemConfig.SourceConfig.Ei.z());

	const clock_t start = clock();
	ComponentList::ImpService->GetExcitation() = source.SetExcitation(ComponentList::BFvector);
	const clock_t end = clock();

	const double time = double(end - start) / CLOCKS_PER_SEC;
	const double memory = double(16*ComponentList::BFvector.size()) / 1048576;

	ResultReport::WriteExcitationInformation(&SystemConfig.SourceConfig);
	Console->info("Set RightHand is cost:\t{:f} s", time);
	RuntimeL->info("Set RightHand is cost:\t{:f} s",time);
	ResultL->info("Set RightHand is cost:\t{:f} s", time);

	Console->info("Memory Cost\t{0:f} Mb", memory);
	RuntimeL->info("Memory Cost\t{0:f} Mb", memory);
	ResultL->info("Memory Cost\t{0:f} Mb", memory);

	RuntimeL->flush();
	return 0;
}
