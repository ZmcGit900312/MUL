#include "stdafx.h"
#include "CoreAPI.h"
#include "Log.h"


shared_ptr<spd::logger> Core::Assist::Log::Console =nullptr;
shared_ptr<spd::logger> Core::Assist::Log::RuntimeLog = nullptr;
shared_ptr<spd::logger> Core::Assist::Log::ResultLog = nullptr;

void Core::LogInitial()
{
	const int MB = 1024 * 1024;
	const int fileSize = 5;
	const int rotateNum = 3;
	//char buffer[1000];
	//GetCurrentDirectoryA(1000, buffer);//获得当前工作目录
	Console = spd::stdout_color_mt("console");
	RuntimeL = spd::rotating_logger_mt("runtime","Runtime.log",fileSize*MB,rotateNum);
	Console->set_pattern("[%r][thread %t][%^%l%$] %v");
	RuntimeL->set_pattern("[%c %a][%l] %v");

#ifdef _DEBUG
	Console->set_level(spd::level::debug);
	RuntimeL->set_level(spd::level::debug);
#else
	Console->set_level(spd::level::info);
	RuntimeL->set_level(spd::level::info);
#endif
	
}

void Core::Assist::LogException(spd::spdlog_ex&ex)
{
	Console->error(ex.what());
	RuntimeL->error(ex.what());
	RuntimeL->flush();
}
