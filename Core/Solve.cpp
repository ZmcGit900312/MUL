#include "stdafx.h"
#include "CoreAPI.h"
#include "Data.h"
#include "Log.h"
#include "IterationSolver.h"
#include "ResultReport.h"
#include "Current.h"

int Core::Solve(int currentIndex)
{	
	Runtime->info("Solve Matrix");
	ResultReport::WriteSolutionInformation(&SystemConfig.SolverConfig, Console);
	ResultReport::WriteSolutionInformation(&SystemConfig.SolverConfig, ResultLog);

	auto& bf = ComponentList::BFvector;

	VectorXcd current{ ComponentList::ImpService->GetExcitation().size() };
	auto curInfo = Solution::CurrentInfo::GetInstance();
	int info = 0;
	try
	{
		
		if(curInfo->Current.size()==0)throw spd::spdlog_ex("Current is not Initialization!");

		info = Solver->Solve(current, ComponentList::ImpService->GetExcitation());
		if (info > 1)throw spd::spdlog_ex("The iteration can't converage");
		if (info == 1)throw spd::spdlog_ex("Reach the max iteration");
		

		const auto sol = static_cast<Solution::IterativeSolver*>(Solver);
		Console->info("Iteration is cost:\t{:f} s", sol->GetSolveTime());
		
		ResultLog->info("The Final Iteration is:\t{}", sol->GetIteration());
		ResultLog->info("The results tolerance is:\t{:5.4e}", sol->GetTolerance());
		ResultLog->info("Iteration is cost:\t{:f} s", sol->GetSolveTime());		
		
		auto individualCurrent = Solution::CurrentInfo::GetInstance()->Current[currentIndex];
		for (int zmc = 0; zmc < current.size(); ++zmc)
			individualCurrent->_data.push_back(current[zmc]);

		Runtime->flush();		

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
