#include "stdafx.h"
#include "CoreAPI.h"
#include "ImpAIM.h"
#include "ImpMoM.h"
#include "MultipoleExpansioner.h"
#include "Log.h"
int Core::CreatImpedance()
{
	Vector3d delta;
	cout << "\n";
	try
	{
		RuntimeL->info("Run CreatImpedance()");
		switch (SystemConfiguration::ImpConfig.impType)
		{
		case AIM:
			Console->info("Initial AIM Matrix");
			ResultL->info("\nMethod AIM");
			//Configuration
			SystemConfiguration::ImpConfig.Box[0] = ComponentList::Geometry.GetLimitationBoundary(0)
				- (SystemConfiguration::ImpConfig.GridOrder - 1)*SystemConfiguration::ImpConfig.Interval*Vector3d::Ones();
			SystemConfiguration::ImpConfig.Box[1] = ComponentList::Geometry.GetLimitationBoundary(7)
				+ (SystemConfiguration::ImpConfig.GridOrder - 1)* SystemConfiguration::ImpConfig.Interval*Vector3d::Ones();

			Console->info("AIMBox is from ({0:+5.3f},{1:+5.3f},{2:+5.3f}) to ({3:+5.3f},{4:+5.3f},{5:+5.3f})",
				SystemConfiguration::ImpConfig.Box[0].x(),
				SystemConfiguration::ImpConfig.Box[0].y(),
				SystemConfiguration::ImpConfig.Box[0].z(),
				SystemConfiguration::ImpConfig.Box[1].x(),
				SystemConfiguration::ImpConfig.Box[1].y(),
				SystemConfiguration::ImpConfig.Box[1].z());
			ResultL->info("Lower Point of AIMBox is ({0:+5.3f},{1:+5.3f},{2:+5.3f})",
				SystemConfiguration::ImpConfig.Box[0].x(),
				SystemConfiguration::ImpConfig.Box[0].y(),
				SystemConfiguration::ImpConfig.Box[0].z());
			ResultL->info("Upper Point of AIMBox is ({0:+5.3f},{1:+5.3f},{2:+5.3f})",
				SystemConfiguration::ImpConfig.Box[1].x(),
				SystemConfiguration::ImpConfig.Box[1].y(),
				SystemConfiguration::ImpConfig.Box[1].z());

			delta = (SystemConfiguration::ImpConfig.Box[1] - SystemConfiguration::ImpConfig.Box[0]) /
				SystemConfiguration::ImpConfig.Interval;
			Console->info("AIM Interval:\t{:4.2f} ¦Ë", SystemConfiguration::ImpConfig.Interval);
			ResultL->info("AIM Interval:\t{:4.2f} ¦Ë", SystemConfiguration::ImpConfig.Interval);

			Console->info("Dimension:\t{}", SystemConfiguration::ImpConfig.Dimension);
			ResultL->info("Dimension:\t{}", SystemConfiguration::ImpConfig.Dimension);

			SystemConfiguration::ImpConfig.xNumber = (int)round(delta.x()) + 1;
			SystemConfiguration::ImpConfig.yNumber = (int)round(delta.y()) + 1;
			SystemConfiguration::ImpConfig.zNumber = (int)round(delta.z()) + 1;

			Console->info("LayerNumber:\t({0},{1},{2})", SystemConfiguration::ImpConfig.xNumber,
				SystemConfiguration::ImpConfig.yNumber, SystemConfiguration::ImpConfig.zNumber);
			ResultL->info("LayerNumber:\t({0},{1},{2})", SystemConfiguration::ImpConfig.xNumber,
				SystemConfiguration::ImpConfig.yNumber, SystemConfiguration::ImpConfig.zNumber);

			Console->info("AIM Order:\t{}", SystemConfiguration::ImpConfig.GridOrder);
			ResultL->info("AIM Order:\t{}", SystemConfiguration::ImpConfig.GridOrder);

			Console->info("AIM Threshold:\t{:4.2f} ¦Ë", SystemConfiguration::ImpConfig.Threshold);
			ResultL->info("AIM Threshold:\t{:4.2f} ¦Ë", SystemConfiguration::ImpConfig.Threshold);

			Console->info("AIM NearFieldEps:\t{:e}", SystemConfiguration::ImpConfig.NearCorrectionEps);
			ResultL->info("AIM NearFieldEps:\t{:e}", SystemConfiguration::ImpConfig.NearCorrectionEps);

			ComponentList::ImpService = new ImpAIM(&SystemConfiguration::ImpConfig);
			RuntimeL->info("Finish");
			RuntimeL->flush();
			return 0;
		case MUL:
			throw spd::spdlog_ex("Mul Algorithm is not Developing");
		default:
			Console->info("Initial MoM Matrix");
			ResultL->info("\nMethod MoM");

			ComponentList::ImpService = new ImpMoM(SystemConfiguration::ImpConfig.ImpSize);

			RuntimeL->info("Finish");
			RuntimeL->flush();
			return 0;
		}
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}
}
