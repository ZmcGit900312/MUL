#include "stdafx.h"
#include "CoreAPI.h"
#include "Log.h"
#include "Current.h"
#include "Data.h"
#include "FarField.h"
#include "ResultReport.h"


int Core::CalculatedMissionCentre(bool requestonly)
{
	int status = 0;
	Runtime->info("Call CalculatedMissionCentre(requestonly = {0})", requestonly?"true":"false");
	try
	{

		auto curInfo = Solution::CurrentInfo::GetInstance();
		//Initial RCS
		Console->debug("Initial static Request::FarField::RCS");
		Request::FarField::RCS.resize(curInfo->_numberOfConfig, SystemConfig.PostConfig.size());

		//Configuration Recycle
		for (int zmc = 0; zmc < curInfo->_numberOfConfig; ++zmc)
		{
			Solution::ElementCurrent& valcurrent = *curInfo->Current[zmc];
			cout << "\n";
			Console->info("{0:#^70}", valcurrent.Tag);
			Console->info("Frequency:    \t{0:<15}", valcurrent._frequency);
			Console->info("Unknowns:     \t{0:<15}", valcurrent._unknowns);
			Console->info("ElementNumber:\t{0:<15}", valcurrent._numberOfElement);

			ResultLog->info("\n{0:#^70}", valcurrent.Tag);
			ResultLog->info("Frequency:    \t{0:<15}", valcurrent._frequency);
			ResultLog->info("Unknowns:     \t{0:<15}", valcurrent._unknowns);
			ResultLog->info("ElementNumber:\t{0:<15}", valcurrent._numberOfElement);

			//Initial IE
			valcurrent.EMCParameterUpdate();
			equation = IE::FIE(SystemConfig.IEConfig.type);

			ResultReport::WirteIEInformation(&SystemConfig.IEConfig, ResultLog);
			ResultReport::WirteIEInformation(&SystemConfig.IEConfig, Console);

			//Mission dispatch
			if(!requestonly)
			{
				status += Core::MatrixSolving(zmc);
			}

			status += Core::CalculateRequest(zmc);

			//Release IE
			if (Core::equation)
			{
				delete equation;
				equation = nullptr;
				Console->debug("Release IE");
			}
		}
		

		Runtime->info("End CalculatedMissionCentre(requestonly = {0})", requestonly ? "true" : "false");
		Runtime->flush();
		return status;
	}
	catch (spdlog::spdlog_ex&ex)
	{
		Core::Assist::LogException(ex);
		return 1;
	}
}
