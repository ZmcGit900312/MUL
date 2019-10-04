#include "stdafx.h"
#include "CoreAPI.h"
#include "ImpAIM.h"
#include "ImpMoM.h"
#include "Log.h"
#include "ModalGeometry.h"
#include "ResultReport.h"
#include "ImpArrayAIM.h"
#include "IterationSolver.h"
#include "Current.h"

using namespace Eigen;

int Core::MatrixSolving(int currentIndex)
{
	int status = 0;
	Core::Runtime->info("Call MatrixSolving(currentIndex={0})", currentIndex);
	try
	{
		//PreCalculateSelfTriangleImpedance
		{
			Console->info("Begin to pre-compute the SelfTriangle Impedance");
			Runtime->info("Pre-compute the SelfTriangle Impedance");

			const clock_t start = clock();
			for (auto i = Mesh::GetInstance()->TriangleVector.begin(),
				e = Mesh::GetInstance()->TriangleVector.end(); i != e; ++i)
			{
				RWGImpOperator::SetSelfTriangleOperatorL(dynamic_cast<RWGTriangle*>(*i), W13, k, eta);
			}
			const clock_t end = clock();
			const double time = double(end - start) / CLOCKS_PER_SEC;
			//
			Console->info("Pre-compute SelfTriangle Impedance costs:\t{} s", time);
			ResultLog->info("Pre-compute SelfTriangle Impedance costs:\t{} s", time);
		}



		//Initial  Solver
		{
			Console->info("{0:-^60}","Initial Solver");
			Runtime->info("Initial Solver");
			Solver = FSolver(SystemConfig.SolverConfig, SystemConfig.ImpConfig.ImpType);
		}
		

		// Impedance
		{
			//Initial
			Runtime->info("Generate Impedance");
			Vector3d delta;
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

				Console->warn("SystemConfig.ImpConfig.ArrayLocation is not Initial");

				ComponentList::ImpService = new ImpArrayAIM(&SystemConfig.ImpConfig);
				break;
			default:
				ComponentList::ImpService = new ImpMoM(SystemConfig.ImpConfig.ImpSize);
			}

			ResultReport::WriteMethodInformation(&SystemConfig.ImpConfig, Console);
			ResultReport::WriteMethodInformation(&SystemConfig.ImpConfig, ResultLog);

			//Filling

			ComponentList::ImpService->FillImpedance();

			Console->info("Whole Filling costs\t{:f} s", ComponentList::ImpService->GetTime());
			Console->info("Memory Cost\t{:f} Mb", ComponentList::ImpService->GetMemoryCost());
			ResultLog->info("Whole Filling costs\t{:f} s", ComponentList::ImpService->GetTime());
			ResultLog->info("Memory Cost\t{:f} Mb", ComponentList::ImpService->GetMemoryCost());
		}

		//Set RightHand
		{
			Runtime->info("Set RightHand");
			ResultReport::WriteExcitationInformation(SystemConfig.SourceConfig, Console);

			const clock_t start = clock();
			ComponentList::ImpService->GetExcitation() = SystemConfig.SourceConfig->SetExcitation(ComponentList::BFvector, SystemConfig.ImpConfig);
			const clock_t end = clock();

			ResultReport::WriteExcitationInformation(SystemConfig.SourceConfig, ResultLog);

			const double time = double(end - start) / CLOCKS_PER_SEC;
			const double memory = double(16 * ComponentList::BFvector.size()) / 1048576;


			Console->info("Set RightHand is cost:\t{:f} s", time);
			ResultLog->info("Set RightHand is cost:\t{:f} s", time);

			Console->info("Memory Cost\t{0:f} Mb", memory);
			ResultLog->info("Memory Cost\t{0:f} Mb", memory);
		}

		//Solve
		{
			Runtime->info("Solve Matrix");
			ResultReport::WriteSolutionInformation(&SystemConfig.SolverConfig, Console);
			ResultReport::WriteSolutionInformation(&SystemConfig.SolverConfig, ResultLog);


			VectorXcd current{ ComponentList::ImpService->GetExcitation().size() };
			status = Solver->Solve(current, ComponentList::ImpService->GetExcitation());
			if (status > 1)throw spd::spdlog_ex("The iteration can't converage");
			if (status == 1)throw spd::spdlog_ex("Reach the max iteration");

			const auto sol = static_cast<Solution::IterativeSolver*>(Solver);
			Console->info("Iteration is cost:\t{:f} s", sol->GetSolveTime());

			ResultLog->info("The Final Iteration is:\t{}", sol->GetIteration());
			ResultLog->info("The results tolerance is:\t{:5.4e}", sol->GetTolerance());
			ResultLog->info("Iteration is cost:\t{:f} s", sol->GetSolveTime());

			auto individualCurrent = Solution::CurrentInfo::GetInstance()->Current[currentIndex];
			for (int zmc = 0; zmc < current.size(); ++zmc)
				individualCurrent->_data.push_back(current[zmc]);


		}
		//Release
		{
			
			if (Solver) {
				
				delete Solver;
				Solver = nullptr;
				Console->debug("Release Solver");
			}

			if (ComponentList::ImpService)
			{
				delete ComponentList::ImpService;
				ComponentList::ImpService = nullptr;
				Console->debug("Release Matrix");
			}

			Runtime->info("Release SOlver and ImpService");
		}
		

		Runtime->info("End MatrixSolving(currentIndex={0})", currentIndex);
		Runtime->flush();

		return status;
	}
	catch(spdlog::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}
}