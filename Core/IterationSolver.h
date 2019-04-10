//IterationSolver.h for BiCG-STAB

#ifndef ITERATIONSOLVER_H
#define ITERATIONSOLVER_H
#include "ISolver.h"
#include "Log.h"
#include "ImpMoM.h"
#include "ImpAIM.h"
using namespace Eigen;


namespace Core
{
	namespace Solution
	{
		class IterativeSolver : public ISolver
		{
		public:

			IterativeSolver(SolverConfiguration& config): _config(&config){};
			virtual ~IterativeSolver() { _config = nullptr; }

			int SolveInfo()const { return _info; }
			long long GetIteration()const { return _iteration; }
			double GetTolerance()const override { return _tolerance; }
			double GetPreconditionTime()const override { return _preTime; }
			double GetSolveTime()const override { return _solveTime; }
		protected:
			SolverConfiguration* _config = nullptr;
			int _info = 1;
			long long _iteration = 0;
			double _tolerance = 1.0e-3;
			double _preTime = 0,_solveTime=0;
		};

		//AIM Identity

		class BiCGStabAIMIdentity :public IterativeSolver
		{
		public:
			BiCGStabAIMIdentity(SolverConfiguration&config) :IterativeSolver(config) {}
			int Precondition(IImpService*)override;
			int Solve(VectorXcd& sol, VectorXcd& right)override;
		private:
			BiCGSTAB<ImpAIM, IdentityPreconditioner> _bicg;
		};



		//AIM ILU
		class BiCGStabAIMILU :public IterativeSolver
		{
		public:
			BiCGStabAIMILU(SolverConfiguration&config) :IterativeSolver(config) {};
			int Precondition(IImpService*imp)override;
			int Solve(VectorXcd& sol, VectorXcd& right)override;
		private:
			BiCGSTAB<ImpAIM, AIMIncompleteLUT<ImpAIM::Scalar>> _bicg;
		};

		//MoM
		class BiCGStabMoMJacobi :public IterativeSolver
		{
		public:
			BiCGStabMoMJacobi(SolverConfiguration&config) :IterativeSolver(config) {}
			int Precondition(IImpService*)override;
			int Solve(VectorXcd& sol, VectorXcd& right)override;

		private:
			BiCGSTAB<MatrixXcd, DiagonalPreconditioner<dcomplex>> _bicg;
		};

	}
}




#endif

