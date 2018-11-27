#include "stdafx.h"
#include "Data.h"


#pragma region ComponentList

vector<IBasicFunction*> ComponentList::BFvector;
IImpService* ComponentList::ImpService = nullptr;
SystemConfiguration Core::SystemConfig;
#pragma endregion 
//Tools
AIMAssist::TeoplitzAssist* Tools::TeoplitzMultiplicator = nullptr;

Solution::ISolver* Core::Solver=nullptr;