#include "stdafx.h"
#include "CoreAPI.h"
#include "ImpAIM.h"
#include "ImpMoM.h"
#include "Log.h"
#include "ModalGeometry.h"
#include "ResultReport.h"

int Core::CreatImpedance()
{
	Vector3d delta;
	cout << "\n";
	try
	{		
		//Initial Solver
		Console->info("Initial Solver");
		RuntimeL->info("Initial Solver");
		Solver = FSolver(SystemConfig.SolverConfig, SystemConfig.ImpConfig.impType);
		switch (SystemConfig.ImpConfig.impType)
		{
		case AIM:
			Console->info("{:*^45}", "Initial AIM Matrix");
			RuntimeL->info("{:*^45}", "Initial AIM Matrix");
			//Configuration
			SystemConfig.ImpConfig.Box[0] = 
				Assist::ModalGeometry::GetInstance()->GetLimitationBoundary(0)
			- (SystemConfig.ImpConfig.GridOrder - 1)*SystemConfig.ImpConfig.Interval*Vector3d::Ones();
			SystemConfig.ImpConfig.Box[1] = 
				Assist::ModalGeometry::GetInstance()->GetLimitationBoundary(7)
				+ (SystemConfig.ImpConfig.GridOrder - 1)* SystemConfig.ImpConfig.Interval*Vector3d::Ones();

			Console->info("AIMBox is from ({0:+5.3f},{1:+5.3f},{2:+5.3f}) to ({3:+5.3f},{4:+5.3f},{5:+5.3f})",
				SystemConfig.ImpConfig.Box[0].x(),
				SystemConfig.ImpConfig.Box[0].y(),
				SystemConfig.ImpConfig.Box[0].z(),
				SystemConfig.ImpConfig.Box[1].x(),
				SystemConfig.ImpConfig.Box[1].y(),
				SystemConfig.ImpConfig.Box[1].z());
			RuntimeL->info("AIMBox is from ({0:+5.3f},{1:+5.3f},{2:+5.3f}) to ({3:+5.3f},{4:+5.3f},{5:+5.3f})",
				SystemConfig.ImpConfig.Box[0].x(),
				SystemConfig.ImpConfig.Box[0].y(),
				SystemConfig.ImpConfig.Box[0].z(),
				SystemConfig.ImpConfig.Box[1].x(),
				SystemConfig.ImpConfig.Box[1].y(),
				SystemConfig.ImpConfig.Box[1].z());


			delta = (SystemConfig.ImpConfig.Box[1] - SystemConfig.ImpConfig.Box[0]) /
				SystemConfig.ImpConfig.Interval;
			Console->info("AIM Interval:\t{:4.2f} ¦Ë", SystemConfig.ImpConfig.Interval);
			RuntimeL->info("AIM Interval:\t{:4.2f} ¦Ë", SystemConfig.ImpConfig.Interval);

			Console->info("Dimension:\t{}", SystemConfig.ImpConfig.Dimension);
			RuntimeL->info("Dimension:\t{}", SystemConfig.ImpConfig.Dimension);

			SystemConfig.ImpConfig.xNumber = int(round(delta.x())) + 1;
			SystemConfig.ImpConfig.yNumber = int(round(delta.y())) + 1;
			SystemConfig.ImpConfig.zNumber = int(round(delta.z())) + 1;

			Console->info("LayerNumber:\t({0},{1},{2})", SystemConfig.ImpConfig.xNumber,
				SystemConfig.ImpConfig.yNumber, SystemConfig.ImpConfig.zNumber);
			RuntimeL->info("LayerNumber:\t({0},{1},{2})", SystemConfig.ImpConfig.xNumber,
				SystemConfig.ImpConfig.yNumber, SystemConfig.ImpConfig.zNumber);

			Console->info("AIM Order:\t{}", SystemConfig.ImpConfig.GridOrder);
			RuntimeL->info("AIM Order:\t{}", SystemConfig.ImpConfig.GridOrder);

			Console->info("AIM Threshold:\t{:4.2f} ¦Ë", SystemConfig.ImpConfig.Threshold);
			RuntimeL->info("AIM Threshold:\t{:4.2f} ¦Ë", SystemConfig.ImpConfig.Threshold);

			Console->info("AIM NearFieldEps:\t{:e}", SystemConfig.ImpConfig.NearCorrectionEps);
			RuntimeL->info("AIM NearFieldEps:\t{:e}", SystemConfig.ImpConfig.NearCorrectionEps);

			ComponentList::ImpService = new ImpAIM(&SystemConfig.ImpConfig);

			RuntimeL->flush();
			break;
		case MUL:
			throw spd::spdlog_ex("Mul Algorithm is not Developing");
		default:
			Console->info("{:*^45}", "Initial MoM Matrix");
			RuntimeL->info("{:*^45}", "Initial MoM Matrix");

			ComponentList::ImpService = new ImpMoM(SystemConfig.ImpConfig.ImpSize);
			RuntimeL->flush();		
		}

		ResultReport::WriteMethodInformation(&SystemConfig.ImpConfig);
		return 0;
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}
}
