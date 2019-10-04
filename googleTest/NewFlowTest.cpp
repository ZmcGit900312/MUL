#include "stdafx.h"
#include "Current.h"


#ifdef GTEST

#include "CoreAPI.h"
#include <iostream>
#include "Const.h"
#include "Data.h"
#include "ResultReport.h"



using namespace std;


class NewFlowTest:public testing::Test
{
protected:
	static void SetUpTestCase()
	{
		ASSERT_EQ(0, Core::DataInitialization()) << "Error in Initialization";
		
	}

	static void TearDownTestCase()
	{
		
	}

};


TEST_F(NewFlowTest,MatrixSolving)
{
	bool requestonly = false;

	ASSERT_EQ(0, Core::SetCurrent(requestonly));
	EXPECT_EQ(0, Core::CalculatedMissionCentre(requestonly));

}


TEST_F(NewFlowTest,Request)
{
	bool requestonly = true;

	ASSERT_EQ(0, Core::SetCurrent(requestonly));
	EXPECT_EQ(0, Core::CalculatedMissionCentre(requestonly));
}

TEST_F(NewFlowTest,SaveResultRead)
{
	bool requestonly = false;

	ASSERT_EQ(0, Core::SetCurrent(requestonly));
	EXPECT_EQ(0, Core::CalculatedMissionCentre(requestonly));
	EXPECT_EQ(0, Core::SaveResults(requestonly));
}

TEST_F(NewFlowTest, SaveResultRequestOnly)
{
	bool requestonly = true;

	ASSERT_EQ(0, Core::SetCurrent(requestonly));
	EXPECT_EQ(0, Core::CalculatedMissionCentre(requestonly));
	EXPECT_EQ(0, Core::SaveResults(requestonly));
}
#endif