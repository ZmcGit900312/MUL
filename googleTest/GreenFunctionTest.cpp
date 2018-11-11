#include "stdafx.h"
#ifdef GTEST

#include "Data.h"
#include "CoreAPI.h"

TEST(CoreAPITEST,GreenFunctionTest)
{
	auto greentype = SetGreenFunction();

	auto func = ComponentList::Green;


	if (ComponentList::Green)
	{
		delete ComponentList::Green;
		ComponentList::Green = nullptr;
		Console->debug("Release GreenFunction");
	}
}


#endif