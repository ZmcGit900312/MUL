#include "stdafx.h"
#ifdef GTEST
#include "gtest/gtest.h"
#include "CoreAPI.h"
#include "Mesh.h"
#include "Data.h"

using namespace Eigen;
using namespace std;


class ComponentListTest :public testing::Test
{
protected:

	//TestSuite
	static void SetUpTestCase()
	{
		ASSERT_EQ(0, Core::CreatMesh()) << "Error in Creat Mesh";
	}

	static void TearDownTestCase()
	{
	}

	//Releae BasicFunction every case if exist
	void TearDown()override
	{
		if (ComponentList::BFvector.size() > 0)
		{
			for (auto element : ComponentList::BFvector) { delete element; }
			ComponentList::BFvector.clear();
			ComponentList::BFvector.shrink_to_fit();
			Console->debug("Release BasicFunction");
		}
	}


};


TEST_F(ComponentListTest, BasicFunctionSetTest)
{
	const int refTriangleID[5][2] = { 1,2, 2,118, 19,22, 55,751, 157,159 };
	const int refRWGID[5] = { 0,4,45,127,332 };

	ASSERT_EQ(0, CreatBasicFunction()) << "Error in Creat BasicFunction";

	vector<IBasicFunction*>& bfptr = ComponentList::BFvector;

	EXPECT_EQ(1287, bfptr.size());

	//Edge test
	for (int i = 0; i < 5; ++i)
	{
		//Plus TriangleID
		EXPECT_EQ(refTriangleID[i][0] - 1, bfptr[refRWGID[i]]->LimitPlus()) << "Plus Triangle error in RWG id\t" << refRWGID[i];
		//Minus TriangleID
		EXPECT_EQ(refTriangleID[i][1] - 1, bfptr[refRWGID[i]]->LimitMinus()) << "Minus Triangle error in RWG id\t" << refRWGID[i];
	}
	EXPECT_EQ(0, Core::SaveBasicFunction(SystemConfig.BasicFunctionFileName.c_str())) << "Error in save BasicFunction";
}

TEST_F(ComponentListTest, BasicFunctionLoadTest)
{
	const int refTriangleID[5][2] = { 1,2, 2,118, 19,22, 55,751, 157,159 };
	const int refRWGID[5] = { 0,4,45,127,332 };

	ASSERT_EQ(0, CreatBasicFunction(false)) << "Error in Load BasicFunction";

	vector<IBasicFunction*>& bfptr = ComponentList::BFvector;

	EXPECT_EQ(1287, bfptr.size());

	//Edge test
	for (int i = 0; i < 5; ++i)
	{
		//Plus TriangleID
		EXPECT_EQ(refTriangleID[i][0] - 1, bfptr[refRWGID[i]]->LimitPlus()) << "Plus Triangle error in RWG id\t" << refRWGID[i];
		//Minus TriangleID
		EXPECT_EQ(refTriangleID[i][1] - 1, bfptr[refRWGID[i]]->LimitMinus()) << "Minus Triangle error in RWG id\t" << refRWGID[i];
	}
	EXPECT_EQ(0, Core::SaveBasicFunction(SystemConfig.BasicFunctionFileName.c_str())) << "Error in save BasicFunction";
}
#endif