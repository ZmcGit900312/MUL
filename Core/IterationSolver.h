//IterationSolver.h for BiCG-STAB

#ifndef ITERATIONSOLVER_H
#define ITERATIONSOLVER_H
#include "ISolver.h"
#include "ImpMoM.h"

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
		
		//Solver Template
		template<typename _MatrixType,typename _Preconditioner>
		class BiCGSTABEMC:public IterativeSolver
		{
		public:
			BiCGSTABEMC(SolverConfiguration&config) :IterativeSolver(config) {}
			int Precondition(IImpService*imp)override
			{
				const clock_t start = clock();
				_bicg.compute(*static_cast<_MatrixType*>(imp));
				const clock_t end = clock();
				_preTime = double(end - start) / CLOCKS_PER_SEC;
				return 0;
			}

			int Solve(VectorXcd& sol, VectorXcd& right)override
			{
				if (_config->Maxiteration > 0)_bicg.setMaxIterations(_config->Maxiteration);
				if (_config->Residum > 0 && _config->Residum < 1)_bicg.setTolerance(_config->Residum);

				const clock_t start = clock();
				sol = _bicg.solve(right);
				const clock_t end = clock();
				_solveTime = double(end - start) / CLOCKS_PER_SEC;

				_info = _bicg.info();
				_iteration = _bicg.iterations();
				_tolerance = _bicg.error();
				return _info;
			}
		private:
			BiCGSTAB<_MatrixType, _Preconditioner> _bicg;
		};
		
		//Specific For MoM
		template<>
		class BiCGSTABEMC<ImpMoM, DiagonalPreconditioner<dcomplex>>:public IterativeSolver
		{
		public:
			BiCGSTABEMC(SolverConfiguration&config) :IterativeSolver(config) {}
			int Precondition(IImpService*imp)override
			{
				const clock_t start = clock();
				_bicg.compute(static_cast<ImpMoM*>(imp)->LocalMatrix());
				const clock_t end = clock();
				_preTime = double(end - start) / CLOCKS_PER_SEC;
				return 0;
			}
			int Solve(VectorXcd& sol, VectorXcd& right)override
			{
				if (_config->Maxiteration > 0)_bicg.setMaxIterations(_config->Maxiteration);
				if (_config->Residum > 0 && _config->Residum < 1)_bicg.setTolerance(_config->Residum);

				const clock_t start = clock();
				sol = _bicg.solve(right);
				const clock_t end = clock();
				_solveTime = double(end - start) / CLOCKS_PER_SEC;

				_info = _bicg.info();
				_iteration = _bicg.iterations();
				_tolerance = _bicg.error();
				return _info;
			}
		private:
			BiCGSTAB<MatrixXcd, DiagonalPreconditioner<dcomplex>> _bicg;
		};
	}
}




#endif

