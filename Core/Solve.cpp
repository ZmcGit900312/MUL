#include "stdafx.h"
#include "CoreAPI.h"
#include "Data.h"
#include "Log.h"
#include "IterationSolver.h"
#include "ResultReport.h"

int Core::Solve()
{	
	cout << '\n';

	Console->info("{:*^45}","Solve Matrix with BiCGSTAB");
	Console->info("The Maxiteration is:\t{}", SystemConfig.SolverConfig.Maxiteration);
	Console->info("The Tolerance is:\t{:e}", SystemConfig.SolverConfig.Tolerance);
	RuntimeL->info("{:*^45}", "Solve Matrix with BiCGSTAB");
	RuntimeL->info("The Maxiteration is:\t{}", SystemConfig.SolverConfig.Maxiteration);
	RuntimeL->info("The Tolerance is:\t{:e}", SystemConfig.SolverConfig.Tolerance);

	ResultReport::WriteSolutionInformation(&SystemConfig.SolverConfig);
	const auto unknown = SystemConfig.ImpConfig.ImpSize;
	VectorXcd current{ unknown };
	int info = 0;
	try
	{
		
		info = Solver->Solve(current, ComponentList::ImpService->GetExcitation());
		if (info > 1)throw spd::spdlog_ex("The iteration can't converage");
		if (info == 1)throw spd::spdlog_ex("Reach the max iteration");

		auto& bf = ComponentList::BFvector;
		for (int i = 0; i<unknown; ++i)
		{
			bf[i]->Current() = current(i);
		}

		const auto sol = static_cast<Solution::IterativeSolver*>(Solver);
		Console->info("Iteration is cost:\t{:f} s", sol->GetSolveTime());
		
		ResultL->info("The Final Iteration is:\t{}", sol->GetIteration());
		ResultL->info("The results tolerance is:\t{:5.4e}", sol->GetTolerance());
		ResultL->info("Iteration is cost:\t{:f} s", sol->GetSolveTime());		
		
		RuntimeL->info("The Final Iteration is:\t{}", sol->GetIteration());
		RuntimeL->info("The results tolerance is:\t{:5.4e}", sol->GetTolerance());
		RuntimeL->info("Iteration is cost:\t{:f} s", sol->GetSolveTime());
		RuntimeL->flush();
		
		if (Solver) {
			Console->info("Release Solver");
			delete Solver;
			Solver = nullptr;
		}

		return info;
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		if (Solver) {
			delete Solver;
			Solver = nullptr;
		}
		return info;
	}
	
}
