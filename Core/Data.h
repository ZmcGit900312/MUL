#pragma once
#include "IBasisFunction.h"
#include "IImpedance.h"
#include "Green.h"
#include "RequestConfiguration.h"
#include <list>
#include "TeoplitzAssist.h"
#include "ISolver.h"
#include "Excitation.h"
#include "IEKernel.h"
namespace Core
{
	struct ComponentList 
	{
		static vector<IBasisFunction*> BFvector;//手动释放资源
		static IImpService* ImpService;
	};

	struct SystemConfiguration
	{	
		string ProjectName = "SourceData";
		string ProjectDir = "C:/";
		string ReportFilePath = "C:/";
		string MeshFilePath;
		string BasisFunctionFilePath= ProjectDir + ProjectName + ".bf";
		string CurrentFilePath = ProjectDir + ProjectName + ".cu";
		int BasisFunctionType = 0;
		ImpConfiguration ImpConfig;
		GreenConfiguration GreenConfig;
		IEConfiguration IEConfig;
		Source::ISource* SourceConfig=nullptr;
		Solution::SolverConfiguration SolverConfig;		
		vector<Request::FarFieldConfiguration> PostConfig;
		
	};
	
	extern SystemConfiguration SystemConfig;

	struct Tools
	{
		static AIMAssist::TeoplitzAssist* TeoplitzMultiplicator;
	};

	extern Solution::ISolver*Solver;
	extern IE*equation;
}

