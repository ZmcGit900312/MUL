#include "stdafx.h"
#include "Data.h"


#pragma region ComponentList

vector<Core::IBasisFunction*> Core::ComponentList::BFvector;
Core::IImpService* Core::ComponentList::ImpService = nullptr;
Core::SystemConfiguration Core::SystemConfig;
#pragma endregion 
//Tools
Core::AIMAssist::TeoplitzAssist* Core::Tools::TeoplitzMultiplicator = nullptr;

Core::Solution::ISolver* Core::Solver=nullptr;

Core::IE* Core::equation = nullptr;