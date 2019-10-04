#include "stdafx.h"
#include "CoreAPI.h"
#include "Data.h"
#include "RWG.h"
#include "ResultReport.h"
#include "Log.h"
#include "Current.h"
#include "IMatrixFiller.h"

int Core::CreatBasisFunction(const bool isCreat)
{
	
	try
	{
		cout << "\n";
		
		Mesh* mesh = Mesh::GetInstance();

		if (Mesh::GetInstance()->IsLock())throw spdlog::spdlog_ex("Mesh is lock!");
		Console->info("Initial BasisFunction");

		switch (SystemConfig.BasisFunctionType)
		{
		case 1:
			throw spdlog::spdlog_ex("Other BasisFunction is not developing!");
		default:
			Console->info("Choose RWG Function as BasisFunction\n");
			Runtime->info("Choose RWG Function as BasisFunction\n");
			
			
		}

		if (isCreat||LoadBasisFunction(SystemConfig.BasisFunctionFilePath.c_str()))
		{
			const clock_t start = clock();
			SystemConfig.ImpConfig.ImpSize =
				RWG::CreatRWGBasisFunctionList(mesh, &ComponentList::BFvector);
			const clock_t end = clock();
			const double _time = double(end - start) / CLOCKS_PER_SEC;
			Console->info("The number of BasisFuntions is:\t{0:d}", SystemConfig.ImpConfig.ImpSize);
			Console->info("Creat BasisFuntions costs:\t{0:10.9} s", _time);
			Runtime->info("The number of BasisFuntions is:\t{0:d}", SystemConfig.ImpConfig.ImpSize);
			Runtime->info("Creat BasisFuntions costs:\t{0:10.9} s", _time);
			ResultReport::WriteBasisFunctionInformation(SystemConfig.ImpConfig.ImpSize,ResultLog);
			ResultLog->info("Creat BasisFuntions costs:\t{0:10.9} s", _time);
		}
		

		//Initial Current
		/*auto curInfo = Solution::CurrentInfo::GetInstance();
		curInfo->category = SystemConfig.ImpConfig.ImpType;
		if(curInfo->Current.size()==0)
		{
			curInfo->_numberOfConfig = 1;
			if (SystemConfig.ImpConfig.ImpType == Core::Array)
			{
				curInfo->Current.push_back(new Solution::ArrayCurrent(
					SystemConfig.ImpConfig.ImpSize,
					Frequency,
					"Current0",
					SystemConfig.ImpConfig.ArrayNumX*SystemConfig.ImpConfig.ArrayNumY,
					SystemConfig.ImpConfig.ArrayIntervalX,
					SystemConfig.ImpConfig.ArrayIntervalY));
			}
			else
			{
				curInfo->Current.push_back(new Solution::ElementCurrent
				(SystemConfig.ImpConfig.ImpSize, Frequency, "Current0"));
			}
		}
		else
		{
			for(auto val:curInfo->Current)
			{
				val->_unknowns = SystemConfig.ImpConfig.ImpSize*val->_numberOfElement;
			}
		}*/
		return 0;
			
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}
}

int Core::LoadBasisFunction(const char* loadFileName)
{
	//GetCurrentDirectoryA(1000, buff);//
	
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
			Console->info("The number of BasisFuntions is:\t{0:d}", unknowns);
			Console->info("Load BasisFuntions costs:\t{0:10.9} s", _time);
			ResultReport::WriteBasisFunctionInformation(unknowns,ResultLog);
			ResultLog->info("Load BasisFuntions costs:\t{0:10.9} s", _time);
			Runtime->info("The number of BasisFuntions is:\t{0:d}", unknowns);
			Runtime->info("Load BasisFuntions costs:\t{0:10.9} s", _time);
			Runtime->flush();
			/*cout << "The number of BasisFuntions is\t=\t" << unknowns << "\n";
			cout << "Finish!\nLoad BasisFuntions costs\t" << _time << " s\n";*/
			return 0;
		}
		throw spd::spdlog_ex("Load BasisFunction Error in Finding "+string(loadFileName));
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}
	
}

int Core::SaveBasisFunction(const char * saveFileName)
{
	cout << '\n';
	Console->info("Save BasisFunction in binary type");
	Runtime->info("Save BasisFunction in binary type");
	
	//GetCurrentDirectoryA(1000, buff);//
	ofstream ofs(saveFileName,ios::out|ios::binary| ios::trunc);

	try
	{
		/*if (!Solution::CurrentInfo::GetInstance()->SaveCurrent(SystemConfig.CurrentFilePath.c_str()))
			throw spd::spdlog_ex("Save Current Error in\t" + SystemConfig.CurrentFilePath);*/
		
		if (ComponentList::BFvector.size() < 1)throw spd::spdlog_ex("BasisFunction Vector is empty");
		if (ofs.is_open())
		{
			Console->info("The Save File Path is\t"+string(saveFileName));
			Runtime->info("The Save File Path is\t" + string(saveFileName));
			ResultLog->info("The Save File Path is\t" + string(saveFileName));
			//RWG
			int RWG = 0;
			int unknown = (int)ComponentList::BFvector.size();
			ofs.write(reinterpret_cast<char*>(&RWG), sizeof(int));
			ofs.write(reinterpret_cast<char*>(&unknown), sizeof(int));

			for (auto element : ComponentList::BFvector)element->SaveBinary(ofs);
			ofs.flush();
			ofs.close();
			
			Runtime->flush();
			return 0;
		}
		throw spd::spdlog_ex("Save BasisFunction Error in Finding\t" + string(saveFileName));
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}
	
}