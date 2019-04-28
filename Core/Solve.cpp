#include "stdafx.h"
#include "CoreAPI.h"
#include "Data.h"
#include "Log.h"
#include "IterationSolver.h"
#include "ResultReport.h"

int Core::Solve()
{	
	cout << '\n';

	ResultReport::WriteSolutionInformation(&SystemConfig.SolverConfig, Console);
	ResultReport::WriteSolutionInformation(&SystemConfig.SolverConfig, RuntimeLog);
	ResultReport::WriteSolutionInformation(&SystemConfig.SolverConfig,ResultLog);
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
		
		ResultLog->info("The Final Iteration is:\t{}", sol->GetIteration());
		ResultLog->info("The results tolerance is:\t{:5.4e}", sol->GetTolerance());
		ResultLog->info("Iteration is cost:\t{:f} s", sol->GetSolveTime());		
		
		RuntimeLog->info("The Final Iteration is:\t{}", sol->GetIteration());
		RuntimeLog->info("The results tolerance is:\t{:5.4e}", sol->GetTolerance());
		RuntimeLog->info("Iteration is cost:\t{:f} s", sol->GetSolveTime());
		RuntimeLog->flush();
		
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
