#include "stdafx.h"
#include "ISolver.h"
#include "Log.h"
#include "IImpedance.h"
#include "ImpAIM.h"
#include "AIMArray.h"
#include "ImpMoM.h"
#include "IterationSolver.h"


Core::Solution::ISolver * Core::Solution::FSolver(SolverConfiguration & config, const EImpedance imptype)
{
	if(imptype==MoM)
	{
		switch (config.SolutionType)
		{
		case LU:
			Console->warn("MoM with LU decomposition is not developed");
		default:
			switch(config.Precond) 
			{ 
				case Identity:Console->warn("MoM with Identity is not recommand");
				default:
					Console->info("Preconditioning chooses Jacobi");
					config.Precond = Jacobi;
					return new BiCGSTABEMC<ImpMoM, DiagonalPreconditioner<dcomplex>>(config);
			}
		}
	}

	if (imptype == AIM)
	{
		switch (config.Precond)
		{
		case ILU:
			Console->info("Preconditioning chooses IncompletLU Decomposition");
			return new BiCGSTABEMC<ImpAIM, AIMIncompleteLUT<ImpAIM::Scalar>>(config);
		case Jacobi:
			Console->warn("AIM with Jacobi is not Developed and replaced by Identity");
		default:
			config.Precond = Identity;
			Console->info("Preconditioning chooses Identity");
			return new BiCGSTABEMC<ImpAIM, IdentityPreconditioner>(config);
		}
	}
	
	if(imptype==Core::Array)
	{
		switch (config.Precond)
		{
		case ILU:
			Console->warn("ArrayAIM with ILU decomposition is not developed");
		case Jacobi:
			Console->warn("ArrayAIMwith Jacobi is not Developed and replaced by Identity");
		default:
			config.Precond = Identity;
			Console->info("Preconditioning chooses Identity");
			return new BiCGSTABEMC<ImpArrayAIM, IdentityPreconditioner>(config);
		}
	}

	throw spd::spdlog_ex("Error in Solver");
}
