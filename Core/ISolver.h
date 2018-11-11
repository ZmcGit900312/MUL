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
		enum ESolutionType{LU,BiCGStab=10};
		enum EPreconditionerType{Identity,Jacobi=8,ILU=9};
		struct SolverConfiguration
		{
			ESolutionType Sol = BiCGStab;
			EPreconditionerType Precond = ILU;
			unsigned Maxiteration = 0;
			double Tolerance = -1.0;
			double MaxStopTolerance = 10.0;
		};

		__interface ISolver
		{
			int virtual Precondition(IImpService*) = 0;
			int virtual  Solve(VectorXcd& sol,VectorXcd& right) = 0;

		};

		ISolver* FSolver(SolverConfiguration&config, EImpedance imptype); 
	}
}


#endif
