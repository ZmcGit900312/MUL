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
		if (ComponentList::MeshService) {
			delete ComponentList::MeshService; ComponentList::MeshService = nullptr;
			Console->debug("Release Mesh");
		}
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



TEST_F(ComponentListTest, MeshTest)
{
	const int refTriangle1[5] = { 0,720,90,2,89 }, refTriangle2[5] = { 63,720,54,162,144 },
		refTriangle3[5] = { 857,720,339,340,260 };
	const int nodeID[3] = { 0,9,430 };
	const Vector3d refNode[3] = { Vector3d(3.673940397E-17,.0,-.6) ,Vector3d(4.357205165E-01,2.975630966E-02,-4.114148680E-01) ,
		Vector3d(6.799550077E-02, -3.339034976E-01,4.938472093E-01) };

	Mesh* mesh = ComponentList::MeshService;
	//ASSERT_FALSE(mesh->IsLock()) << "Mesh is Locked";
	EXPECT_EQ(431, mesh->GetNode()) << "Error in Mesh.GetNode()";
	EXPECT_EQ(858, mesh->GetTriangle()) << "Error in Mesh.GetTriangle()";
	Console->debug("Get Node Test");
	for (int i = 0; i < 2; ++i)
	{
		EXPECT_NEAR(0, (refNode[i] - mesh->GetNode(nodeID[i])).norm(), 1.0e-4) << "Error in GetNode " << nodeID[i] << " from Mesh";
	}

	Console->debug("Get Triangle Test");
	//T1 Test
	Triangle* ptrT = &mesh->GetTriangle(refTriangle1[0]);
	EXPECT_EQ(refTriangle1[1], ptrT->Material()) << "Error in T1 Matrials";
	EXPECT_EQ(refTriangle1[2], ptrT->ID(0).first) << "Error in T1 Node1 ID";
	EXPECT_EQ(refTriangle1[3], ptrT->ID(1).first) << "Error in T1 Node2 ID";
	EXPECT_EQ(refTriangle1[4], ptrT->ID(2).first) << "Error in T1 Node3 ID";

	//T2 Test
	ptrT = &mesh->GetTriangle(refTriangle2[0]);
	EXPECT_EQ(refTriangle2[1], ptrT->Material()) << "Error in T2 Matrials";
	EXPECT_EQ(refTriangle2[2], ptrT->ID(0).first) << "Error in T2 Node1 ID";
	EXPECT_EQ(refTriangle2[3], ptrT->ID(1).first) << "Error in T2 Node2 ID";
	EXPECT_EQ(refTriangle2[4], ptrT->ID(2).first) << "Error in T2 Node3 ID";

	//T3 Test
	ptrT = &mesh->GetTriangle(refTriangle3[0]);
	EXPECT_EQ(refTriangle3[1], ptrT->Material()) << "Error in T3 Matrials";
	EXPECT_EQ(refTriangle3[2], ptrT->ID(0).first) << "Error in T3 Node1 ID";
	EXPECT_EQ(refTriangle3[3], ptrT->ID(1).first) << "Error in T3 Node2 ID";
	EXPECT_EQ(refTriangle3[4], ptrT->ID(2).first) << "Error in T3 Node3 ID";
}

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
	EXPECT_EQ(0, Core::SaveBasicFunction(SystemConfiguration::BasicFunctionFileName.c_str())) << "Error in save BasicFunction";
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
	EXPECT_EQ(0, Core::SaveBasicFunction(SystemConfiguration::BasicFunctionFileName.c_str())) << "Error in save BasicFunction";
}
#endif