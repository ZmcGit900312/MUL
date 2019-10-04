#include "stdafx.h"
#include "CoreAPI.h"
#include "ImpAIM.h"
#include "ImpMoM.h"
#include "Log.h"
#include "ModalGeometry.h"
#include "ResultReport.h"
#include "ImpArrayAIM.h"

int Core::CreatImpedance()
{
	Vector3d delta;
	cout << "\n";
	try
	{		
		//Initial Solver
		Console->info("Initial Solver");
		Runtime->info("Initial Solver");
		Solver = FSolver(SystemConfig.SolverConfig, SystemConfig.ImpConfig.ImpType);
		switch (SystemConfig.ImpConfig.ImpType)
		{
		case AIM:
			//Configuration
			SystemConfig.ImpConfig.Box[0] = 
				Assist::ModalGeometry::GetInstance()->GetLimitationBoundary(0)
			- (SystemConfig.ImpConfig.GridOrder - 1)*SystemConfig.ImpConfig.Interval*Vector3d::Ones();
			SystemConfig.ImpConfig.Box[1] = 
				Assist::ModalGeometry::GetInstance()->GetLimitationBoundary(7)
				+ (SystemConfig.ImpConfig.GridOrder - 1)* SystemConfig.ImpConfig.Interval*Vector3d::Ones();

			delta = (SystemConfig.ImpConfig.Box[1] - SystemConfig.ImpConfig.Box[0]) /
				SystemConfig.ImpConfig.Interval;		

			SystemConfig.ImpConfig.xNumber = int(round(delta.x())) + 1;
			SystemConfig.ImpConfig.yNumber = int(round(delta.y())) + 1;
			SystemConfig.ImpConfig.zNumber = int(round(delta.z())) + 1;

			ComponentList::ImpService = new ImpAIM(&SystemConfig.ImpConfig);
		
			break;
		case Array:
			//Configuration
			SystemConfig.ImpConfig.Box[0] =
				Assist::ModalGeometry::GetInstance()->GetLimitationBoundary(0)
				- (SystemConfig.ImpConfig.GridOrder - 1)*SystemConfig.ImpConfig.Interval*Vector3d::Ones();
			SystemConfig.ImpConfig.Box[1] =
				Assist::ModalGeometry::GetInstance()->GetLimitationBoundary(7)
				+ (SystemConfig.ImpConfig.GridOrder - 1)* SystemConfig.ImpConfig.Interval*Vector3d::Ones();

			delta = (SystemConfig.ImpConfig.Box[1] - SystemConfig.ImpConfig.Box[0]) /
				SystemConfig.ImpConfig.Interval;

			SystemConfig.ImpConfig.xNumber = int(round(delta.x())) + 1;
			SystemConfig.ImpConfig.yNumber = int(round(delta.y())) + 1;
			SystemConfig.ImpConfig.zNumber = int(round(delta.z())) + 1;

			ComponentList::ImpService = new ImpArrayAIM(&SystemConfig.ImpConfig);
			break;
		default:			
			ComponentList::ImpService = new ImpMoM(SystemConfig.ImpConfig.ImpSize);
			Runtime->flush();		
		}

		ResultReport::WriteMethodInformation(&SystemConfig.ImpConfig, Console);
		ResultReport::WriteMethodInformation(&SystemConfig.ImpConfig, Runtime);
		Runtime->flush();
		ResultReport::WriteMethodInformation(&SystemConfig.ImpConfig,ResultLog);
		return 0;
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}
}
