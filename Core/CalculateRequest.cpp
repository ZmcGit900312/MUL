#include "stdafx.h"
#include "CoreAPI.h"
#include "Data.h"
#include "FarField.h"
#include "Log.h"

int Core::CalculateRequest(const bool IsReadFromFile)
{
	RuntimeL->info("Run CalculateRequest()");
	//TODO It should be changed for a new version, this function is not well

	cout << "\n";
	Console->info("Calcuate the FarField RCS");
	ResultL->info("Calcuate the FarField RCS");
	try
	{
		if (IsReadFromFile)
		{
			if (CreatMesh())throw spd::spdlog_ex("Error in Request");
			if (CreatBasicFunction(false))throw spd::spdlog_ex("Error in Request");
			if (SetGreenFunction())throw spd::spdlog_ex("Green Function is null");
		}
		Request::FarField post(&ComponentList::BFvector, Mesh::GetInstance());
		
		
		ofstream ofs;
		ofs.flags(ios::left);

		const clock_t start = clock();
		for (auto value : SystemConfiguration::PostConfig)
		{
			const string savename = SystemConfiguration::ProjectDir + '\\' + value.FarFileName + "_RCS.dat";
			ofs.open(savename, ios_base::out);
			if (!ofs.is_open())throw spd::spdlog_ex("Save RCS Directory Error in " + savename);

			post.SetEField(value, ofs);

			ofs.flush();
			ofs.close();
		}
		Console->info("Finish");
		const clock_t end = clock();
		double timecost = double(end - start) / CLOCKS_PER_SEC;
		Console->info("FarField Calculate cost {:f} s", timecost);
		RuntimeL->flush();
		return 0;
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}

	
}
