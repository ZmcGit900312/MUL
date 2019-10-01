#include "stdafx.h"
#include "IterationSolver.h"

	
#pragma region AIMSolver
//AIM Identity
int Core::Solution::BiCGStabAIMIdentity::Precondition(IImpService *imp)
{
	const clock_t start = clock();
	_bicg.compute(*static_cast<ImpAIM*>(imp));
	const clock_t end = clock();
	_preTime = double(end - start) / CLOCKS_PER_SEC;
	return 0;
}

int Core::Solution::BiCGStabAIMIdentity::Solve(VectorXcd & sol, VectorXcd & right)
{
	if (_config->Maxiteration>0)_bicg.setMaxIterations(_config->Maxiteration);
	if (_config->Residum>0 && _config->Residum<1)_bicg.setTolerance(_config->Residum);

	const clock_t start = clock();
	sol = _bicg.solve(right);
	const clock_t end = clock();
	_solveTime = double(end - start) / CLOCKS_PER_SEC;

	_info = _bicg.info();
	_iteration = _bicg.iterations();
	_tolerance = _bicg.error();
	return _info;
}




int Core::Solution::BiCGStabAIMILU::Precondition(IImpService *imp)
{
	const clock_t start = clock();
	_bicg.compute(*static_cast<ImpAIM*>(imp));
	const clock_t end = clock();
	_preTime = double(end - start) / CLOCKS_PER_SEC;
	return 0;
}

int Core::Solution::BiCGStabAIMILU::Solve(VectorXcd & sol, VectorXcd & right)
{

	if (_config->Maxiteration>0)_bicg.setMaxIterations(_config->Maxiteration);
	if (_config->Residum>0 && _config->Residum<1)_bicg.setTolerance(_config->Residum);

	const clock_t start = clock();
	sol = _bicg.solve(right);
	const clock_t end = clock();
	_solveTime = double(end - start) / CLOCKS_PER_SEC;

	_info = _bicg.info();
	_iteration = _bicg.iterations();
	_tolerance = _bicg.error();
	return _info;
}

#pragma endregion

#pragma region MoM
int Core::Solution::BiCGStabMoMJacobi::Precondition(IImpService *imp)
{
	const clock_t start = clock();
	_bicg.compute(static_cast<ImpMoM*>(imp)->LocalMatrix());
	const clock_t end = clock();
	_preTime = double(end - start) / CLOCKS_PER_SEC;
	return 0;
}

int Core::Solution::BiCGStabMoMJacobi::Solve(VectorXcd & sol, VectorXcd & right)
{
	if (_config->Maxiteration>0)_bicg.setMaxIterations(_config->Maxiteration);
	if (_config->Residum>0 && _config->Residum<1)_bicg.setTolerance(_config->Residum);

	const clock_t start = clock();
	sol = _bicg.solve(right);
	const clock_t end = clock();
	_solveTime = double(end - start) / CLOCKS_PER_SEC;

	_info = _bicg.info();
	_iteration = _bicg.iterations();
	_tolerance = _bicg.error();
	return _info;
}

#pragma endregion


