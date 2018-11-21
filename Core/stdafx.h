// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#define EIGEN_USE_MKL_ALL
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <ctime>
#include<functional>
#include <iomanip>
#include <string>
#ifdef NDEBUG
#define EIGEN_NO_DEBUG
#define EIGEN_USE_MKL_ALL
#endif

#ifdef GTEST
#include "gtest/gtest.h"
#endif
// TODO: 在此处引用程序需要的其他头文件
