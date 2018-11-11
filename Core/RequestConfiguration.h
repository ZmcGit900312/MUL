///////////////////////////////////////////////////////////
//  Post.h
//  Implementation of the Class PostConfiguration
//  Created on:      24-4ÔÂ-2018 20:35:23
//  Original author: ZhengMX
///////////////////////////////////////////////////////////

#pragma once
#include <iostream>

namespace Core
{
	namespace Request
	{
		class FarFieldConfiguration
		{
		public:
			unsigned FieldType = 0;//RCS
			unsigned ThetaNum = 1, PhiNum = 1;
			double ThetaStart = 0, PhiStart = 0;
			double ThetaIncrement = 1, PhiIncrement = 1;
			string FarFileName;
		};
	}
}

