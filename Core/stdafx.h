// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once
#define EIGEN_USE_MKL_ALL
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
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
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
