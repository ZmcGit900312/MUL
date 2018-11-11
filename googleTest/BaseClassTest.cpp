#include "stdafx.h"
#ifdef GTEST
#include "gtest/gtest.h"
#include "Triangle.h"
#include "TxtReader.h"
#include "Log.h"
using namespace Core;
using namespace Eigen;


TEST(BaseTest,TriangleClass) {
	int id[5] = { 2,4568,3,9,4 };
	const Vector3d t1(1.0, 1.0, 1.0), t2(4.5, 2.3, -6.8), t3(5.2, -4.8, 9.9);
	Triangle t(id, t1, t2, t3);
	Console->debug("Construct Triangle with (id,t1,t2,t3):\n"
		"t1=\t({0},{1},{2})\nt2=\t({3},{4},{5})\nt3=\t({6},{7},{8})",
		t1.x(),t1.y(),t1.z(),t2.x(),t2.y(),t2.z(),t3.x(),t3.y(),t3.z());
	Console->debug("Three node ID test");
	EXPECT_EQ(id[2], t.ID(0).first)<<"Error in t.ID(0)";
	EXPECT_EQ(id[3], t.ID(1).first) << "Error in t.ID(1)";
	EXPECT_EQ(id[4], t.ID(2).first) << "Error in t.ID(2)";

	//Vertex
	Console->debug("Three vertexs test");
	EXPECT_NEAR(0, (t1 - t.Vertex().col(0)).norm(), 1.0e-4) << "Error in vertex(0)";
	EXPECT_NEAR(0, (t2 - t.Vertex().col(1)).norm(), 1.0e-4) << "Error in vertex(1)";
	EXPECT_NEAR(0, (t3 - t.Vertex().col(2)).norm(), 1.0e-4) << "Error in vertex(2)";

	//Area
	Console->debug("Area test");
	EXPECT_NEAR(38.3462, t.Area(), 1.0e-4)<<"Error in Area";
}

TEST(BaseTest,TxtReaderClass)
{
	char* readFileName = "E:/ZMC/Code/C_program/MUL/test/test.hxj";
	char* writeFileName = "TxtReaderClassWriteTest.txt";
	TxtReader TR;
	Console->debug("Read File Test");
	EXPECT_FALSE(TR.Read("MaMa")) << "Error in Read File";
	ASSERT_TRUE(TR.Read(readFileName)) << "Error in Read File";
	Console->debug("Write File Test");
	ASSERT_TRUE(TR.Write(writeFileName)) << "Error in Write File";
}

#endif