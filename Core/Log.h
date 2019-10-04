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
	

	extern shared_ptr<spd::logger> Console;
	extern shared_ptr<spd::logger> Runtime;
	extern shared_ptr<spd::logger> ResultLog;

	namespace Assist
	{

		void LogException(spd::spdlog_ex&ex);


	}
	
}


#endif
