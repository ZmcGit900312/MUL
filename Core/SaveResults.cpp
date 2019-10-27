#include "stdafx.h"
#include "CoreAPI.h"
#include "Log.h"
#include "Current.h"
#include "Data.h"
#include "ResultReport.h"
#include "FarField.h"


namespace Core {namespace Request {
	class FarField;
}
}

int Core::SaveResults(bool requestonly)
{
	
	int status = 0;
	try
	{
		if(!requestonly)
		{
			status += SaveBasisFunction(SystemConfig.BasisFunctionFilePath.c_str());

			if (!Solution::CurrentInfo::GetInstance()->SaveCurrent(SystemConfig.CurrentFilePath.c_str()))
				throw spd::spdlog_ex("Save Current Error in\t" + SystemConfig.CurrentFilePath);
		}		
		

		ofstream ofs;
		ofs.flags(ios::left);
		for (int zmc=0;zmc< SystemConfig.PostConfig.size();++zmc)
		{
			auto value = SystemConfig.PostConfig[zmc];

			const string saveFilePath = SystemConfig.ProjectDir + '\\' + value.FarFileName + "_"+value.GetImpedanceTypeString()+".csv";
			ofs.open(saveFilePath, ios_base::out);
			if (!ofs.is_open())throw spd::spdlog_ex("Save Directory Error in " + saveFilePath);

			//Title
			ofs << "Theta" << ',' << "Phi";
			for (auto val : Solution::CurrentInfo::GetInstance()->Current)
				ofs << ',' << val->Tag + " "+ value.GetImpedanceTypeString();
			ofs << '\n';
			Request::FarField::SaveRCS(ofs, value, zmc);

			ofs.flush();
			ofs.close();
			Console->info("Save File into:\t" + saveFilePath);
					
		}
	}
	catch(spdlog::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}

	return status;
}
