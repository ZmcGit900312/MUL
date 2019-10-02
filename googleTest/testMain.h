#ifndef TESTMAIN_H
#define TESTMAIN_H
#ifdef GTEST
#include "gtest/gtest.h"
#include "CoreAPI.h"
#include "Log.h"

class MulEnvironment :public testing::Environment
{
public:
	void SetUp() override
	{
#ifdef HXJ
		char* configurationFileName = "E:/ZMC/Code/C_program/MUL/SourceData/test.hxj";
		ASSERT_EQ(0, Core::EMCParameterInitialization(configurationFileName)) << "Error in API InitializationConfiguration";	
#else

		/*char* configurationFileName = "E:/ZMC/Code/C_program/MUL/SourceData/test.xml";
		ASSERT_EQ(0, Core::AnalysisConfigurationFile(configurationFileName)) << "Error in API InitializationConfiguration";*/

		char* configurationFileName = "E:/ZMC/Code/C_program/MUL/SourceData/test.json";
		ASSERT_EQ(0, Core::ParseConfiguratoinFile(configurationFileName)) 
		<< "Error in API InitializationConfiguration";
#endif
	}

	void TearDown() override
	{
		ASSERT_EQ(0, Core::ReleaseMemory()) << "Error in API ReleaseMemory";
	}
};

#endif

#endif
