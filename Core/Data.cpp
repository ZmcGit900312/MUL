#include "stdafx.h"
#include "Data.h"


#pragma region ComponentList

vector<Core::IBasicFunction*> Core::ComponentList::BFvector;
Core::IImpService* Core::ComponentList::ImpService = nullptr;
Core::SystemConfiguration Core::SystemConfig;
#pragma endregion 
//Tools
Core::AIMAssist::TeoplitzAssist* Core::Tools::TeoplitzMultiplicator = nullptr;

Core::Solution::ISolver* Core::Solver=nullptr;