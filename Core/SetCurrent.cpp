#include "stdafx.h"
#include "Current.h"
#include "Data.h"
#include "CoreAPI.h"
#include "Log.h"

int Core::SetCurrent(bool requestonly)
{

	auto curInfo = Solution::CurrentInfo::GetInstance();
	if (requestonly)
	{
		Console->info("{0:x^80}", "Request only");
		ResultLog->info("{0:x^80}", "Request only");
		curInfo->Reformat();
		if (!curInfo->ReadCurrent(SystemConfig.CurrentFilePath.c_str()))return 1;
	}
	else
	{
		curInfo->category = SystemConfig.ImpConfig.ImpType;
		
		for (auto val : curInfo->Current)
		{	
			if (curInfo->category == Core::Array)
				static_cast<Solution::ArrayCurrent*>(val)->_elementUnknowns
				= SystemConfig.ImpConfig.ImpSize;
			val->_unknowns = SystemConfig.ImpConfig.ImpSize*val->_numberOfElement;
			val->_data.reserve(val->_unknowns);
		}

	}
	return 0;
}