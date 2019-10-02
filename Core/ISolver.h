//Solver.h

#ifndef SOLVER_H
#define SOLVER_H


#include <Eigen\Sparse>
#include "IImpedance.h"

using namespace Eigen;


namespace Core
{
	namespace Solution
	{
		enum ESolutionType{BICGSTAB, LU};
		enum EPreconditionerType{Identity,Jacobi,ILU};
		struct SolverConfiguration
		{
			ESolutionType SolutionType = BICGSTAB;
			EPreconditionerType Precond = ILU;
			unsigned Maxiteration = 0;
			double Residum = -1.0;
			double StopTolerance = 10.0;
		};

		__interface ISolver
		{
			int virtual Precondition(IImpService*) = 0;
			int virtual  Solve(VectorXcd& sol,VectorXcd& right) = 0;
			double GetTolerance()const = 0;
			double GetPreconditionTime()const = 0;
			double GetSolveTime()const = 0;
		};

		ISolver* FSolver(SolverConfiguration&config, EImpedance imptype);

		inline extern string GetSolutionTypeString(ESolutionType st)
		{
			switch(st)
			{
			case BICGSTAB: return "BICGSTAB";
			case LU: return "LU Decomposition";
			default: throw exception("error SolutionType");
			}
		}

		inline extern string GetPrecondionTypeString(EPreconditionerType pt)
		{
			switch(pt)
			{
			case Identity: return "Identity";
			case Jacobi: return "Jacobi";
			case ILU: return "Incomplete LU";
			default: throw exception("error PreconditioningType");
			}
		}
	}
}


#endif
