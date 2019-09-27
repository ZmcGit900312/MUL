#include "stdafx.h"
#include "CoreAPI.h"
#include "Data.h"
#include "Log.h"
#include "IterationSolver.h"
#include "ResultReport.h"
#include "Current.h"

int Core::Solve()
{	
	cout << '\n';

	ResultReport::WriteSolutionInformation(&SystemConfig.SolverConfig, Console);
	ResultReport::WriteSolutionInformation(&SystemConfig.SolverConfig, RuntimeLog);
	ResultReport::WriteSolutionInformation(&SystemConfig.SolverConfig,ResultLog);

	auto& bf = ComponentList::BFvector;
	const auto unknowns = bf.size();
	VectorXcd current{ unknowns };
	auto curInfo = Solution::CurrentInfo::GetInstance();
	int info = 0;
	try
	{
		
		if(curInfo->Current.size()==0)throw spd::spdlog_ex("Current is not Initialization!");


		info = Solver->Solve(current, ComponentList::ImpService->GetExcitation());
		if (info > 1)throw spd::spdlog_ex("The iteration can't converage");
		if (info == 1)throw spd::spdlog_ex("Reach the max iteration");
		
		//这个地方用来调取配置
		auto individualCurrent = curInfo->Current[0];
		for (int zmc = 0; zmc<unknowns; ++zmc)
			individualCurrent->_data.push_back(current[zmc]);

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
