#include "stdafx.h"
#include "CoreAPI.h"
#include "Const.h"
#include "Data.h"
#include "IntegrationRWG.h"
#include "Log.h"

int Core::PreCalculateSelfTriangleImpedance()
{
	//特化RWG填充策略使用，预先计算三角形的自阻抗
	cout << '\n';
	Console->info("Begin to pre-compute the SelfTriangle Impedance");
	RuntimeL->info("Begin to pre-compute the SelfTriangle Impedance");
	const clock_t start = clock();
#pragma region RWGSET
	for (auto i = Mesh::GetInstance()->TriangleMock.begin(),
		e= Mesh::GetInstance()->TriangleMock.end(); i != e; ++i)
	{
		EFRImp::SetSelfImpedanceTriangle(dynamic_cast<RWGTriangle*>(*i), W13, k, eta);
	}
	const clock_t end = clock();
	const double time = double(end - start) / CLOCKS_PER_SEC;
	//
	Console->info("Pre-compute SelfTriangle Impedance costs:\t{} s", time);
	ResultL->info("Pre-compute SelfTriangle Impedance costs:\t{} s", time);
	RuntimeL->info("Finish Pre-compute");
	RuntimeL->flush();
#pragma endregion 
	return 0;
}
