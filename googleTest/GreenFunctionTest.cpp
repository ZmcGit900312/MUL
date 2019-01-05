#include "stdafx.h"
#ifdef GTEST

#include "Data.h"
#include "CoreAPI.h"

TEST(CoreAPITEST,GreenFunctionTest)
{
	EXPECT_EQ(0,Core::SetGreenFunction());

	auto func = Core::IGreen::GetInstance();

	Vector3d pt0{ 0,0,0 }, pt1{ 0,0,1 }, pt2{ 1.3,0,0 }, pt3{ 0,3.3,0 };
	dcomplex ref1{ 0.07957747, 0.0 }, 
	ref2{ -0.01891599313622, - 0.058217440664 }, 
	ref3{ -0.00745175487184, - 0.022934143292};

	dcomplex val1 = func->Scalar(pt0, pt1);
	dcomplex val2 = func->Scalar(pt0, pt2);
	dcomplex val3 = func->Scalar(pt0, pt3);

	EXPECT_NEAR(norm(ref1),norm(val1),1.0e-4);
	EXPECT_NEAR(norm(ref2), norm(val2), 1.0e-4);
	EXPECT_NEAR(norm(ref3), norm(val3), 1.0e-4);

}


#endif