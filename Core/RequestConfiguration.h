///////////////////////////////////////////////////////////
//  Post.h
//  Implementation of the Class PostConfiguration
//  Created on:      24-4ÔÂ-2018 20:35:23
//  Original author: ZhengMX
///////////////////////////////////////////////////////////

#pragma once

namespace Core
{
	namespace Request
	{
		enum EFarField{RCS=0,Pattern=1};

		class FarFieldConfiguration
		{
		public:
			EFarField FieldType = RCS;//RCS 0 Pattern 1
			unsigned ThetaNum = 1, PhiNum = 1;
			double ThetaStart = 0, PhiStart = 0;
			double ThetaIncrement = 1, PhiIncrement = 1;
			std::string FarFileName;

			std::string GetImpedanceTypeString()const
			{
				switch (FieldType)
				{
				case Pattern: return "Pattern";
				default: return "RCS";
				}
			}
		};
	}
}

