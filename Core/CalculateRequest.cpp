#include "stdafx.h"
#include "CoreAPI.h"
#include "Data.h"
#include "FarField.h"
#include "Log.h"
#include "ResultReport.h"

int Core::CalculateRequest(const bool IsReadFromFile)
{
	RuntimeLog->info("Run CalculateRequest()");
	//TODO It should be changed for a new version, this function is not well

	cout << "\n";
	Console->info("{:*^45}", "Calcuate the FarField RCS");
	RuntimeLog->info("{:*^45}", "Calcuate the FarField RCS");
	ResultLog->info("{:*^45}", "FarField");
	try
	{
		/*if (IsReadFromFile)
		{
			if (CreatMesh())throw spd::spdlog_ex("Error in Request");
			if (CreatBasicFunction(false))throw spd::spdlog_ex("Error in Request");
			if (SetGreenFunction())throw spd::spdlog_ex("Green Function is null");
		}*/

		auto curInfo = Solution::CurrentInfo::GetInstance();
		if (curInfo->Current.size() == 0)curInfo->ReadCurrent(SystemConfig.CurrentFilePath.c_str());
		Request::FarField post(&ComponentList::BFvector, Mesh::GetInstance(),curInfo);		
		
		ofstream ofs;
		ofs.flags(ios::left);

		const clock_t start = clock();
		for (auto value : SystemConfig.PostConfig)
		{
			const string savename = SystemConfig.ProjectDir + '\\' + value.FarFileName + "_RCS.csv";
			ofs.open(savename, ios_base::out);
			if (!ofs.is_open())throw spd::spdlog_ex("Save RCS Directory Error in " + savename);			
			ResultReport::WriteRequestInformation(&value, Console);
			ResultReport::WriteRequestInformation(&value, RuntimeLog);
			ResultReport::WriteRequestInformation(&value,ResultLog);
			post.CalculateRCS(value, ofs);

			ofs.flush();
			ofs.close();
		}
		const clock_t end = clock();
		double timecost = double(end - start) / CLOCKS_PER_SEC;
		Console->info("FarField Calculate cost {:f} s", timecost);
		RuntimeLog->info("FarField Calculate cost {:f} s", timecost);
		ResultLog->info("FarField Calculate cost {:f} s", timecost);
		RuntimeLog->flush();
		return 0;
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}

	
}
