#include "stdafx.h"
#include "CoreAPI.h"
#include "Data.h"
#include "FarField.h"
#include <iomanip>
#include "Log.h"

int Core::CalculateRequest(const bool IsReadFromFile)
{
	RuntimeL->info("Run CalculateRequest()");
	//TODO It should be changed for a new version, this function is not well

	cout << "\n";
	Console->info("Calcuate the FarField RCS");
	ResultL->info("Calcuate the FarField RCS");
	try
	{
		if (IsReadFromFile)
		{
			if (CreatMesh())throw spd::spdlog_ex("Error in Request");
			if (CreatBasicFunction(false))throw spd::spdlog_ex("Error in Request");
			if (SetGreenFunction())throw spd::spdlog_ex("Green Function is null");
		}
		Request::FF post(ComponentList::Green, &ComponentList::BFvector, ComponentList::MeshService);
		//Calculate 3D pattern
		//post.SetEField();
		const double coef = post.Radius*post.Radius * 4 * M_PI / (1 * 1);
		ofstream ofs;
		ofs.flags(ios::left);

		const clock_t start = clock();
		for (const auto value : SystemConfiguration::PostConfig)
		{
			const string savename = SystemConfiguration::ProjectDir + '\\' + value.FarFileName + "_RCS.dat";
			Console->info("Calculate FarField:\t" + value.FarFileName);
			ResultL->info("Calculate FarField:\t" + value.FarFileName);
			ofs.open(savename, ios_base::out);
			if (!ofs.is_open())throw spd::spdlog_ex("Save RCS Directory Error in "+savename);
			ofs << setw(7) << "Theta" << setw(7) << "Phi" << setw(12) << "RCS(m2)" << endl;

			const double thetaS = value.ThetaStart;
			const double phiS = value.PhiStart;
			const double thetaI = value.ThetaIncrement;
			const double phiI = value.PhiIncrement;
			const unsigned thetaNum = value.ThetaNum;
			const unsigned phiNum = value.PhiNum;
			const double Sum = 0.01*thetaNum * phiNum;
			for (unsigned th = 0; th < thetaNum; ++th)
			{
				for (unsigned ph = 0; ph < phiNum; ++ph)
				{
					const double theta = thetaS + th*thetaI;
					const double phi = phiS + ph*phiI;
					Vector3cd temp = post.EField(theta*M_PI_180, phi*M_PI_180);
					ofs << setw(7) << theta << setw(7) << phi << setw(12) << coef*temp.squaredNorm() << '\n';
					cout << "Progress:" << setw(10) << (th*phiNum+ph+1) / Sum << "%\r";
				}
			}
			ofs.flush();
			ofs.close();
		}
		RuntimeL->info("Finish");
		const clock_t end = clock();
		double timecost = double(end - start) / CLOCKS_PER_SEC;
		Console->info("FarField Calculate cost {}s", timecost);
		RuntimeL->flush();
		return 0;
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}

	
}
