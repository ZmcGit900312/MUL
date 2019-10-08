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
		status+=Core::PreCalculateSelfTriangleImpedance();

		//Initial Solver And Matrix
		status += Core::InitialSolverAndImpedance();
		//Filling
		status += Core::FillImpedance();		

		//Set RightHand
		status += Core::SetRightHand();

		//Solve
		status += Core::Solve(currentIndex);
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