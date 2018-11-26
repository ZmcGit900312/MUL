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
		IGreen::FactoryGreen(&SystemConfiguration::GreenConfig);
		return 0;
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}
}