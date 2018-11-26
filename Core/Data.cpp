#include "stdafx.h"
#include "Data.h"


#pragma region ComponentList

vector<IBasicFunction*> ComponentList::BFvector;
IImpService* ComponentList::ImpService = nullptr;
IGreen* ComponentList::Green = nullptr;

Source::Planewave ComponentList::RightHand;
Solution::ISolver* ComponentList::Solver=nullptr;
#pragma endregion 

#pragma region SystemConfiguration
//Initial Global static variable in SystemConfiguration
string SystemConfiguration::ProjectName="SourceData";
string SystemConfiguration::ProjectDir="C:/";
string SystemConfiguration::MeshFileName;
string SystemConfiguration::BasicFunctionFileName = ProjectDir + ProjectName + ".bf";
ImpConfiguration SystemConfiguration::ImpConfig;
GreenConfiguration SystemConfiguration::GreenConfig;
Solution::SolverConfiguration SystemConfiguration::SolverConfig;
list<Request::FarFieldConfiguration> SystemConfiguration::PostConfig;

#pragma endregion
//Tools
AIMAssist::TeoplitzAssist* Tools::TeoplitzMultiplicator = nullptr;