#pragma once
#include "Mesh.h"
#include "IBasicFunction.h"
#include "IImpedance.h"
#include "Green.h"
#include "RequestConfiguration.h"
#include <list>
#include "TeoplitzAssist.h"
#include "Planewave.h"
#include "ISolver.h"

namespace Core
{
	struct ComponentList 
	{
		static vector<IBasicFunction*> BFvector;//手动释放资源
		static IImpService* ImpService;
		static IGreen* Green;
		
		static Source::Planewave RightHand;
		static Solution::ISolver* Solver;
	};

	struct SystemConfiguration
	{
		static string ProjectName;
		static string ProjectDir;
		static string MeshFileName;
		static string BasicFunctionFileName;
		static ImpConfiguration ImpConfig;
		static GreenConfiguration GreenConfig;
		static Solution::SolverConfiguration SolverConfig;
		static list<Request::FarFieldConfiguration> PostConfig;
	};
	
	struct Tools
	{
		static AIMAssist::TeoplitzAssist* TeoplitzMultiplicator;
	};
}

