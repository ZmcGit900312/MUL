#include "stdafx.h"
#include "CoreAPI.h"
#include "Data.h"
#include "Log.h"

int Core::SetGreenFunction()
{
	cout << "\n";
	Console->info("Set Green Function");
	RuntimeL->info("Run SetGreenFunction()");
	try
	{
		switch (SystemConfiguration::GreenConfig.GreenType)
		{
		case 1:
			throw spdlog::spdlog_ex("Other GreenFunction is not developing!");
		default:
			ComponentList::Green = new FreeSpaceKernel();
			Console->info("Free Space Green Function is choosen");
			ResultL->info("Free Space Green Function is choosen");
			RuntimeL->info("Finish");
			RuntimeL->flush();
			return 0;
		}
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}
}