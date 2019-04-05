#ifndef TESTMAIN_H
#define TESTMAIN_
#ifdef GTEST
#include "gtest/gtest.h"
#include "CoreAPI.h"
#include "Log.h"

class MulEnvironment :public testing::Environment
{
public:
	char* configurationFileName = "E:/ZMC/Code/C_program/MUL/SourceData/test.hxj";
	void SetUp() override
	{
		ASSERT_EQ(0, Core::EMCParameterInitialization(configurationFileName)) << "Error in API InitializationConfiguration";
		//ASSERT_EQ(0, Core::AnalysisConfigurationFile(configurationFileName)) << "Error in API InitializationConfiguration";
	}

	void TearDown() override
	{
		ASSERT_EQ(0, Core::ReleaseMemory()) << "Error in API ReleaseMemory";
	}
};

#endif

#endif
