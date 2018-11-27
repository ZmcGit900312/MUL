#include "stdafx.h"
#include "Log.h"

const int MB = 1024 * 1024;
const int fileSize = 5;
const int rotateNum = 3;
//Initiallization
shared_ptr<spd::logger> Core::Console = spd::stdout_color_mt("console");
shared_ptr<spd::logger> Core::RuntimeL = spd::rotating_logger_mt("runtime", "Runtime.log", fileSize*MB, rotateNum);
shared_ptr<spd::logger> Core::ResultL = nullptr;

void Core::Assist::LogException(spd::spdlog_ex&ex)
{
	Console->error(ex.what());
	RuntimeL->error(ex.what());
	RuntimeL->flush();
}
//GC
class LogGC
{
	LogGC()
	{
		//char buffer[1000];
	//GetCurrentDirectoryA(1000, buffer);//获得当前工作目录
		Core::Console->set_pattern("[%r][thread %t][%^%l%$] %v");
		Core::RuntimeL->set_pattern("[%c %a][%l] %v");

#ifdef _DEBUG
		Core::Console->set_level(spd::level::debug);
		Core::RuntimeL->set_level(spd::level::debug);
#else
		Core::Console->set_level(spd::level::info);
		Core::RuntimeL->set_level(spd::level::info);
#endif
	}

	~LogGC()
	{
		spdlog::drop_all();
		Core::RuntimeL = nullptr;
		Core::Console = nullptr;
		Core::ResultL = nullptr;
	}
	static LogGC _gcintance;
};

LogGC LogGC::_gcintance;


	
