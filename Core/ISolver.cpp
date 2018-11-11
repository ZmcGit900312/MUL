#include "stdafx.h"
#include "ISolver.h"
#include "IImpedance.h"
#include "IterationSolver.h"


Core::Solution::ISolver * Core::Solution::FSolver(SolverConfiguration & config, const EImpedance imptype)
{
	if(imptype==MoM)
	{
		switch (config.Sol)
		{
		case LU:
			Console->warn("MoM with LU decomposition is not developed");
		default:
			switch(config.Precond) 
			{ 
				case Identity:Console->warn("MoM with Identity is not recommand");
				default: return new BiCGStabMoMJacobi(config);
			}
		}
	}

	switch(config.Precond)
	{
	case ILU:
		return new BiCGStabAIMILU(config);
	case Jacobi:
		Console->warn("AIM with Jacobi is not Developed and replaced by Identity");
	default: 
		return new BiCGStabAIMIdentity(config);
	}
}
