#include "stdafx.h"
#include "CoreAPI.h"
#include "Data.h"
#include "FarField.h"
#include "Log.h"
#include "ResultReport.h"
/**
	* \brief Calculate the far field RCS and write in RCS.txt
	* \param IsReadFromFile Whether read the basicfunction from file. Default is false
	* \return State of Calculation
	* \n 0 Success
*/
int static CalculateRequest(const bool IsReadFromFile)//Not used
{
	Runtime->info("Run CalculateRequest()");
	//TODO It should be changed for a new version, this function is not well

	cout << "\n";
	Console->info("{:*^45}", "Calcuate the FarField RCS");
	Runtime->info("{:*^45}", "Calcuate the FarField RCS");
	ResultLog->info("{:*^45}", "FarField");
	try
	{
		/*if (IsReadFromFile)
		{
			if (CreatMesh())throw spd::spdlog_ex("Error in Request");
			if (CreatBasisFunction(false))throw spd::spdlog_ex("Error in Request");
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
			ResultReport::WriteRequestInformation(&value, Runtime);
			ResultReport::WriteRequestInformation(&value,ResultLog);
			post.CalculateRCS(value, ofs);

			ofs.flush();
			ofs.close();
		}
		const clock_t end = clock();
		double timecost = double(end - start) / CLOCKS_PER_SEC;
		Console->info("FarField Calculate cost {:f} s", timecost);
		Runtime->info("FarField Calculate cost {:f} s", timecost);
		ResultLog->info("FarField Calculate cost {:f} s", timecost);
		Runtime->flush();
		return 0;
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}

	
}


int Core::CalculateRequest(int row)
{
	Runtime->info("Run CalculateRequest(int ={0})",row);


	Console->info("{:-^60}", "Calcuate the FarField RCS");
	ResultLog->info("{:-^60}", "FarField");
	try
	{
		Request::FarField post(&ComponentList::BFvector, Mesh::GetInstance(),
			Solution::CurrentInfo::GetInstance());


		const clock_t start = clock();
		for (int col = 0;col < SystemConfig.PostConfig.size();++col)
		{
			auto value = SystemConfig.PostConfig[col];
		
			ResultReport::WriteRequestInformation(&value, Console);
			ResultReport::WriteRequestInformation(&value, ResultLog);
			post.CalculateDipoleFF(value, row,col);//结果保存在RCS
		}
		const clock_t end = clock();
		double timecost = double(end - start) / CLOCKS_PER_SEC;
		Console->info("FarField Calculate cost {:f} s", timecost);
		ResultLog->info("FarField Calculate cost {:f} s", timecost);
		Runtime->flush();

		Console->info("Finish Calculating {0}/{1} Mission",
			row + 1, Solution::CurrentInfo::GetInstance()->_numberOfConfig);
		return 0;
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}
}