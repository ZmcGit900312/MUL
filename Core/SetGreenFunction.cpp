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
	RuntimeLog->info("Set Green Function");
	try
	{
		IGreen::FactoryGreen(&SystemConfig.GreenConfig);
		ResultReport::WriteGreenFunctionInformation(&SystemConfig.GreenConfig,ResultLog);
		ResultReport::WriteGreenFunctionInformation(&SystemConfig.GreenConfig,Console);
		ResultReport::WriteGreenFunctionInformation(&SystemConfig.GreenConfig, RuntimeLog);
		
		//Updata Const
		Solution::CurrentInfo::GetInstance()->Current[0]->EMCParameterUpdate();

		equation = IE::FIE(SystemConfig.IEConfig.type);
		ResultReport::WirteIEInformation(&SystemConfig.IEConfig, ResultLog);
		ResultReport::WirteIEInformation(&SystemConfig.IEConfig, Console);
		ResultReport::WirteIEInformation(&SystemConfig.IEConfig, RuntimeLog);

		RuntimeLog->flush();

		return 0;
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}
}
