#include "stdafx.h"
#include "CoreAPI.h"
#include "Data.h"
#include "Log.h"
#include "IterationSolver.h"

int Core::Solve()
{
	RuntimeL->info("Run Solve()");
	cout << '\n';

	Console->info("Solve Matrix with BiCGSTAB");
	Console->info("The Maxiteration is:\t{}", SystemConfiguration::SolverConfig.Maxiteration);
	Console->info("The Tolerance is:\t{:e}", SystemConfiguration::SolverConfig.Tolerance);
	ResultL->info("Solve Matrix with BiCGSTAB");
	ResultL->info("The Maxiteration is:\t{}", SystemConfiguration::SolverConfig.Maxiteration);
	ResultL->info("The Tolerance is:\t{:e}", SystemConfiguration::SolverConfig.Tolerance);

	const auto unknown = SystemConfiguration::ImpConfig.ImpSize;
	VectorXcd current{ unknown };
	int info = 0;
	try
	{
		info = ComponentList::Solver->Solve(current, ComponentList::ImpService->GetExcitation());
		if (info > 1)throw spd::spdlog_ex("The iteration can't converage");
		if (info == 1)throw spd::spdlog_ex("Reach the max iteration");

		auto& bf = ComponentList::BFvector;
		for (int i = 0; i<unknown; ++i)
		{
			bf[i]->Current() = current(i);
		}

		const auto sol = static_cast<Solution::IterativeSolver*>(ComponentList::Solver);
		Console->info("Iteration is cost:\t{:f} s", sol->GetSolveTime());
		
		ResultL->info("The Final Iteration is:\t{}", sol->GetIteration());
		ResultL->info("The results tolerance is:\t{:5.4e}", sol->GetTolerance());
		ResultL->info("Iteration is cost:\t{:f} s", sol->GetSolveTime());
		
		
		RuntimeL->info("Finish");
		RuntimeL->flush();
		return info;
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return info;
	}
	
}
