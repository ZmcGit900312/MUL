#include "stdafx.h"
#include "CoreAPI.h"
#include "Data.h"
#include "Log.h"
#include "ResultReport.h"
#include "Current.h"

int Core::SetGreenFunction()
{
	cout << "\n";
	Console->info("Set Green Function");
	Runtime->info("Call SetGreenFunction()");
	try
	{
		IGreen::FactoryGreen(&SystemConfig.GreenConfig);
		ResultReport::WriteGreenFunctionInformation(&SystemConfig.GreenConfig,ResultLog);
		ResultReport::WriteGreenFunctionInformation(&SystemConfig.GreenConfig,Console);		
		Runtime->flush();

		return 0;
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}
}
