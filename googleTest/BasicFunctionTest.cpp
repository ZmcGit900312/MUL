#include "stdafx.h"
#ifdef GTEST
#include "gtest/gtest.h"
#include "CoreAPI.h"
#include "Mesh.h"
#include "Data.h"
#include "RWG.h"


using namespace Eigen;
using namespace std;


class ComponentListTest :public testing::Test
{
protected:

	//TestSuite
	static void SetUpTestCase()
	{
		if(Mesh::GetInstance()->IsLock())ASSERT_EQ(0, Core::CreatMesh()) << "Error in Creat Mesh";
		if (ComponentList::BFvector.size() > 0)
		{
			for (auto element : ComponentList::BFvector) { delete element; }
			ComponentList::BFvector.clear();
			ComponentList::BFvector.shrink_to_fit();
			Console->debug("Release BasicFunction");
		}
		
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


TEST_F(ComponentListTest, RWGTriangleTest)
{
	const int id = 2, nodeId1 = 4568, nodeId2 = 3, nodeId3 = 9, material = 4, entity = 12;
	Vector3d t1(1.0, 1.0, 1.0), t2(4.5, 2.3, -6.8), t3(5.2, -4.8, 9.9);
	RWGTriangle rt(t1, t2, t3, 2, 4568, 3, 9,4,12);
	Console->debug("Construct RWGTriangle with (t1,t2,t3,id,nodeId1,nodeId2,nodeId3,material,entity):\n"
		"t1=\t({0},{1},{2})\nt2=\t({3},{4},{5})\nt3=\t({6},{7},{8})",
		t1.x(), t1.y(), t1.z(), t2.x(), t2.y(), t2.z(), t3.x(), t3.y(), t3.z());
	Console->debug("Three node ID test");

	EXPECT_EQ(nodeId1, rt.NodeID(0)) << "Error in rt.ID(0)";
	EXPECT_EQ(nodeId2, rt.NodeID(1)) << "Error in rt.ID(1)";
	EXPECT_EQ(nodeId3, rt.NodeID(2)) << "Error in rt.ID(2)";
	EXPECT_EQ(material, rt.Material()) << "Error in rt.Material()";
	EXPECT_EQ(entity, rt.Entity()) << "Error in rt.Entity()";
	//Vertex
	Console->debug("Three vertexs test");
	EXPECT_NEAR(0, (t1 - rt.Vertex().col(0)).norm(), 1.0e-4) << "Error in vertex(0)";
	EXPECT_NEAR(0, (t2 - rt.Vertex().col(1)).norm(), 1.0e-4) << "Error in vertex(1)";
	EXPECT_NEAR(0, (t3 - rt.Vertex().col(2)).norm(), 1.0e-4) << "Error in vertex(2)";

	//Area
	Console->debug("Area test");
	EXPECT_NEAR(38.3462, rt.Area(), 1.0e-4) << "Error in Area";


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