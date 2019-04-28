#pragma once
#include "IBasicFunction.h"
#include "IImpedance.h"
#include "Green.h"
#include "RequestConfiguration.h"
#include <list>
#include "TeoplitzAssist.h"
#include "ISolver.h"
#include "Excitation.h"
namespace Core
{
	struct ComponentList 
	{
		static vector<IBasicFunction*> BFvector;//手动释放资源
		static IImpService* ImpService;
	};

	struct SystemConfiguration
	{	
		string ProjectName = "SourceData";
		string ProjectDir = "C:/";
		string ReportFileName = "C:/";
		string MeshFileName;
		string BasicFunctionFileName= ProjectDir + ProjectName + ".bf";
		int BasicFunctionType = 0;
		ImpConfiguration ImpConfig;
		GreenConfiguration GreenConfig;
		Source::ISource* SourceConfig=nullptr;
		Solution::SolverConfiguration SolverConfig;
		list<Request::FarFieldConfiguration> PostConfig;
	};
	
	extern SystemConfiguration SystemConfig;

	struct Tools
	{
		static AIMAssist::TeoplitzAssist* TeoplitzMultiplicator;
	};

	extern Solution::ISolver*Solver;
}

