#include "stdafx.h"
#include "CoreAPI.h"
#include "Data.h"
#include "RWG.h"
#include"Log.h"

int Core::CreatBasicFunction(const bool isCreat)
{
	
	try
	{
		cout << "\n";
		
		Mesh* mesh = Mesh::GetInstance();

		const int basicFunctionType = 0;//以后扩展BasicFunction type

		if (Mesh::GetInstance()->IsLock())throw spdlog::spdlog_ex("Mesh is lock!");
		Console->info("Initial BasicFunction");

		switch (basicFunctionType)
		{
		case 1:
			throw spdlog::spdlog_ex("Other BasicFunction is not developing!");
		default:
			Console->info("Choose RWG Function as BasicFunction\n");
			ResultL->info("Choose RWG Function as BasicFunction\n");
			
		}

		if (LoadBasicFunction(SystemConfig.BasicFunctionFileName.c_str()))
		{
			const clock_t start = clock();
			SystemConfig.ImpConfig.ImpSize =
				RWG::CreatRWGBasicFunctionList(mesh, &ComponentList::BFvector);
			const clock_t end = clock();
			const double _time = double(end - start) / CLOCKS_PER_SEC;
			Console->info("The number of BasicFuntions is:\t{0:d}", SystemConfig.ImpConfig.ImpSize);
			Console->info("Initial BasicFuntions costs:\t{0:10.9} s", _time);
			ResultL->info("The number of BasicFuntions is:\t{0:d}", SystemConfig.ImpConfig.ImpSize);
			ResultL->info("Initial BasicFuntions costs:\t{0:10.9} s", _time);
		}		
		return 0;
			
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}
}

int Core::LoadBasicFunction(const char* loadFileName)
{
	Console->info("Load Basic Function");
	RuntimeL->info("Run LoadBasicFunction()");
	//GetCurrentDirectoryA(1000, buff);//获得当前工作目录
	
	ifstream ifs(loadFileName, ios::in | ios::binary);
	
	try
	{
		if (ifs.is_open())
		{
			
			Mesh* mesh = Mesh::GetInstance();
			ComponentList::BFvector.clear();
			pair<int, Vector3d> tempNode[4];
			int info[7] = { 0,0,0,0,0,0,0 }, BFTYPE = 0;//BFTYPE=0:RWG
			dcomplex current = { 0,1 };
			ifs.read(reinterpret_cast<char*>(&BFTYPE), sizeof(int));
			ifs.read(reinterpret_cast<char*>(&SystemConfig.ImpConfig.ImpSize), sizeof(int));
			const clock_t start = clock();
			const auto unknowns = SystemConfig.ImpConfig.ImpSize;
			for (int i = 0; i < unknowns; ++i)
			{
				//Save Format 
				ifs.read(reinterpret_cast<char*>(info), sizeof(int) * 7);
				ifs.read(reinterpret_cast<char*>(&current), sizeof(dcomplex));
				tempNode[0] = { info[3],mesh->GetNode(info[3]) };
				tempNode[1] = { info[4],mesh->GetNode(info[4]) };
				tempNode[2] = { info[5],mesh->GetNode(info[5]) };
				tempNode[3] = { info[6],mesh->GetNode(info[6]) };
				ComponentList::BFvector.push_back(new RWG(info[0], tempNode, mesh->GetTriangle(info[1]), mesh->GetTriangle(info[2])));
				ComponentList::BFvector.back()->Current() = current;
			}
			ifs.clear();
			ifs.close();
			//SystemConfig.ImpConfig.ImpSize = ComponentList::BFList.size();
			const clock_t end = clock();
			const double _time = double(end - start) / CLOCKS_PER_SEC;
			Console->info("The number of BasicFuntions is:\t{0:d}", unknowns);
			Console->info("Load BasicFuntions costs:\t{0:10.9} s", _time);
			ResultL->info("The number of BasicFuntions is:\t{0:d}", unknowns);
			ResultL->info("Load BasicFuntions costs:\t{0:10.9} s", _time);
			RuntimeL->info("Finish");
			RuntimeL->flush();
			/*cout << "The number of BasicFuntions is\t=\t" << unknowns << "\n";
			cout << "Finish!\nLoad BasicFuntions costs\t" << _time << " s\n";*/
			return 0;
		}
		throw spd::spdlog_ex("Load BasicFunction Error in Finding "+string(loadFileName));
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}
	
}

int Core::SaveBasicFunction(const char * saveFileName)
{
	cout << '\n';
	Console->info("Save BasicFunction in binary type");
	RuntimeL->info("Run SaveBasicFunction()");
	
	//GetCurrentDirectoryA(1000, buff);//获得当前工作目录
	ofstream ofs(saveFileName,ios::out|ios::binary| ios::trunc);
	try
	{
		if (ComponentList::BFvector.size() < 1)throw spd::spdlog_ex("BasicFunction Vector is empty");
		if (ofs.is_open())
		{
			Console->info("The Save File Path is\t"+string(saveFileName));
			ResultL->info("The Save File Path is\t" + string(saveFileName));
			//RWG 代号0
			int RWG = 0;
			int unknown = (int)ComponentList::BFvector.size();
			ofs.write(reinterpret_cast<char*>(&RWG), sizeof(int));
			ofs.write(reinterpret_cast<char*>(&unknown), sizeof(int));

			for (auto element : ComponentList::BFvector)element->SaveBinary(ofs);
			ofs.flush();
			ofs.close();
			RuntimeL->info("Finish");
			RuntimeL->flush();
			return 0;
		}
		throw spd::spdlog_ex("Save BasicFunction Error in Finding\t" + string(saveFileName));
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}
	
}