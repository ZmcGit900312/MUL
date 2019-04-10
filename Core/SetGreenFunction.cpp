#include "stdafx.h"
#include "CoreAPI.h"
#include "Data.h"
#include "Log.h"
#include "ResultReport.h"

int Core::SetGreenFunction()
{
	cout << "\n";
	Console->info("Set Green Function");
	RuntimeL->info("Set Green Function");
	try
	{
		IGreen::FactoryGreen(&SystemConfig.GreenConfig);
		ResultReport::WriteGreenFunctionInformation(&SystemConfig.GreenConfig);

		switch (SystemConfig.GreenConfig.GreenType)
		{
		case 1:
			throw spdlog::spdlog_ex("Other GreenFunction is not developing!");
		default:
			Console->info("Free Space Green Function is choosen\n");
			RuntimeL->info("Free Space Green Function is choosen\n");
		}
		
		RuntimeL->flush();

		return 0;
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}
}
