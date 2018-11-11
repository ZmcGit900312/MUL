//Log.h to provide the log function for Core project
#ifndef LOG_H
#define LOG_H

#ifdef _DEBUG
#define SPDLOG_DEBUG_ON
#endif

#define SPDLOG_TRACE_ON
#include "spdlog/spdlog.h"
#include <memory>
using namespace std;
namespace spd = spdlog;

namespace Core
{
	namespace Assist
	{
		struct Log
		{
			static shared_ptr<spd::logger> Console;
			static shared_ptr<spd::logger> RuntimeLog;
			static shared_ptr<spd::logger> ResultLog;
		};
		
		void LogException(spd::spdlog_ex&ex);
	}

	static shared_ptr<spd::logger>& Console = Assist::Log::Console;
	static shared_ptr<spd::logger>& RuntimeL = Assist::Log::RuntimeLog;
	static shared_ptr<spd::logger>& ResultL = Assist::Log::ResultLog;
}


#endif
