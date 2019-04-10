#include "stdafx.h"
#include "CoreAPI.h"
#include "Data.h"
#include "FarField.h"
#include "Log.h"
#include "ResultReport.h"

int Core::CalculateRequest(const bool IsReadFromFile)
{
	RuntimeL->info("Run CalculateRequest()");
	//TODO It should be changed for a new version, this function is not well

	cout << "\n";
	Console->info("{:*^45}", "Calcuate the FarField RCS");
	RuntimeL->info("{:*^45}", "Calcuate the FarField RCS");
	ResultL->info("{:*^45}", "FarField");
	try
	{
		/*if (IsReadFromFile)
		{
			if (CreatMesh())throw spd::spdlog_ex("Error in Request");
			if (CreatBasicFunction(false))throw spd::spdlog_ex("Error in Request");
			if (SetGreenFunction())throw spd::spdlog_ex("Green Function is null");
		}*/
		Request::FarField post(&ComponentList::BFvector, Mesh::GetInstance());
		
		
		ofstream ofs;
		ofs.flags(ios::left);

		const clock_t start = clock();
		for (auto value : SystemConfig.PostConfig)
		{
			const string savename = SystemConfig.ProjectDir + '\\' + value.FarFileName + "_RCS.csv";
			ofs.open(savename, ios_base::out);
			if (!ofs.is_open())throw spd::spdlog_ex("Save RCS Directory Error in " + savename);
			Console->info("Far Field Request::\t"+value.FarFileName);
			RuntimeL->info("Far Field Request::\t" + value.FarFileName);
			ResultReport::WriteRequestInformation(&value);
			post.SetEField(value, ofs);

			ofs.flush();
			ofs.close();
		}
		const clock_t end = clock();
		double timecost = double(end - start) / CLOCKS_PER_SEC;
		Console->info("FarField Calculate cost {:f} s", timecost);
		RuntimeL->info("FarField Calculate cost {:f} s", timecost);
		ResultL->info("FarField Calculate cost {:f} s", timecost);
		RuntimeL->flush();
		return 0;
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}

	
}
