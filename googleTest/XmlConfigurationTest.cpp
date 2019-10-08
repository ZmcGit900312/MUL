#include "stdafx.h"
#ifdef GTEST
#include "gtest/gtest.h"
#include "Log.h"
#include "Data.h"
#include "CoreAPI.h"
#include "tinyxml2/tinyxml2.h"

using namespace tinyxml2;
using namespace std;

TEST(TinyXml2, ConfigurationTest)
{
	char* name = "E:/ZMC/Code/C_program/MUL/SourceData/test.xml";
	ASSERT_EQ(0, Core::AnalysisConfigurationFile(name));
}

#endif