#include "stdafx.h"

#ifdef GTEST
#include "gtest/gtest.h"
#include "NasRead.h"
#include "GmshFile.h"
#include "Data.h"

using namespace Eigen;
using namespace std;


class MeshTest :public testing::Test
{
protected:

	//TestSuite
	static void SetUpTestCase()
	{
		
	}

	static void TearDownTestCase()
	{
	}

public:
	static Mesh* mesh; 
};

Mesh* MeshTest::mesh= Mesh::GetInstance();


TEST_F(MeshTest, NasFileTest)
{
	IMeshRead *reader = new NasRead;//Here should be changed
	//Mesh* mesh = Mesh::GetInstance();

	string fileName = "E://ZMC//Code//C_program//MUL//SourceData//MeshTest.nas";
	const int refTriangle1[5] = { 0,720,90,2,89 }, refTriangle2[5] = { 63,720,54,162,144 },
		refTriangle3[5] = { 857,720,339,340,260 };
	const int nodeID[3] = { 0,9,430 };
	const Vector3d refNode[3] = { Vector3d(3.673940397E-17,.0,-.6) ,Vector3d(4.357205165E-01,2.975630966E-02,-4.114148680E-01) ,
		Vector3d(6.799550077E-02, -3.339034976E-01,4.938472093E-01) };

	reader->Read(fileName.c_str(), mesh);
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

	delete reader;
}


TEST_F(MeshTest, GmshFileTest)
{
	IMeshRead *reader = new GmshFile;//Here should be changed
	string fileName = "E://ZMC//Code//C_program//MUL//SourceData//Almond.msh";
	reader->Read(fileName.c_str(), mesh);


	delete reader;
}
#endif