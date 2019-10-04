#include"stdafx.h"
#include "CoreAPI.h"
#include "Data.h"
#include "Log.h"

int Core::DataInitialization()
{
	int status=0;

	try
	{
		if (Mesh::GetInstance()->IsLock())
		{
			if (Core::CreatMesh())throw runtime_error("Error in Creat Mesh");
			if (Core::CreatBasisFunction())throw runtime_error("Error in Creat BasicFunction");
		}
		else throw runtime_error("Error in Creat Mesh");
		if (Core::SetGreenFunction())throw runtime_error("Error in set Green Function");

		return status;

	}
	catch(spdlog::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}
}