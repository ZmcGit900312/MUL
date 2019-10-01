#include "stdafx.h"

#ifdef GTEST
#include "gtest/gtest.h"
#include "..\googleTest/testMain.h"
int main(int argc, char**argv)
{
	//argc = 2;
	//char* mockargv[2] = {"gtest","--gtest_filter=AIMTest.AIMCalculate"};
	testing::GTEST_FLAG(catch_exceptions) = true;
	testing::AddGlobalTestEnvironment(new MulEnvironment);
	
	//testing::InitGoogleTest(&argc, mockargv);
	testing::InitGoogleTest(&argc, argv);
	const auto result=RUN_ALL_TESTS();
#ifdef _DEBUG
	
#endif
	return result;
}


#endif