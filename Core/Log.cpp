#include "stdafx.h"
#include "Log.h"

const int MB = 1024 * 1024;
const int fileSize = 5;
const int rotateNum = 3;
//Initiallization
shared_ptr<spd::logger> Core::Console = spd::stdout_color_mt("console");
shared_ptr<spd::logger> Core::RuntimeLog = spd::rotating_logger_mt("runtime", "Runtime.log", fileSize*MB, rotateNum);
shared_ptr<spd::logger> Core::ResultLog = nullptr;

void Core::Assist::LogException(spd::spdlog_ex&ex)
{
	Console->error(ex.what());
	RuntimeLog->error(ex.what());
	RuntimeLog->flush();
}
//GC
class LogGC
{
	LogGC()
	{
		//char buffer[1000];
	//GetCurrentDirectoryA(1000, buffer);//��õ�ǰ����Ŀ¼
		Core::Console->set_pattern("[%r][thread %t][%^%l%$] %v");
		Core::RuntimeLog->set_pattern("[%c %a][%l] %v");

#ifdef _DEBUG
		Core::Console->set_level(spd::level::debug);
		Core::RuntimeLog->set_level(spd::level::debug);
#else
		Core::Console->set_level(spd::level::info);
		Core::RuntimeLog->set_level(spd::level::info);
#endif
	}

	~LogGC()
	{
		spdlog::drop_all();
		Core::RuntimeLog = nullptr;
		Core::Console = nullptr;
		Core::ResultLog = nullptr;
	}
	static LogGC _gcintance;
};

LogGC LogGC::_gcintance;


	
