// Core.cpp : 定义 DLL 应用程序的导出函数。
//
#include "stdafx.h"
#ifndef GTEST
#include "CoreAPI.h"
#include "Data.h"
#include <windows.h>


void main(int argc,char* argv[])
 {
	cout << "***********************************************\n";
	cout << "*      CEM Verison Alpha 0.0.0 in C++         *\n";
	cout << "***********************************************\n";

#ifdef _DEBUG
	char* configurationFileName = "E:/ZMC/Code/C_program/MUL/test/test.hxj";
	//const char* bfFileName = SystemConfiguration::BasicFunctionFileName.c_str();
	argc = 2;
	cout	<< "**********************************\n"
			<< "*    THIS IS Under DEBUG MODE    *\n" 
			<< "**********************************"<<endl;
#else
	char* configurationFileName = argv[1];

#endif
	
	Core::LogInitial();

	while (argc>1)
	{
		try
		{
			const clock_t start = clock();
			if (Core::EMCParameterInitialization(configurationFileName))throw runtime_error("Error in CoreAPI EMCParamenterInitialization");

			if (Core::CreatMesh())throw runtime_error("Error in Creat Mesh");
			if (Core::CreatBasicFunction())throw runtime_error("Error in Creat BasicFunction");
			if (Core::SetGreenFunction())throw runtime_error("Error in set Green Function");
			if (Core::PreCalculateSelfTriangleImpedance())throw runtime_error("Error in Pre-compute the SelfTriangle Impedance");
			if (Core::CreatImpedance())throw runtime_error("Error in Initial the Impedance class");
			if (Core::FillImpedance())throw runtime_error("Error in Fill Impedance");
			if (Core::SetRightHand())throw runtime_error("Error in Set RightHand");
			if (Core::Solve())throw runtime_error("Error in Solve Matrix with BicgStab");
			if (Core::SaveBasicFunction(SystemConfiguration::BasicFunctionFileName.c_str()))throw runtime_error("Error in save BasicFunction");
			if (Core::CalculateRequest())throw runtime_error("Error in Calculate the FarField");
			ReleaseMemory();
			const clock_t end = clock();
			const double time = double(end - start) / CLOCKS_PER_SEC;

			cout << "\n********************************\nThe total time costs\t=" << time << " s" << endl;
			system("pause");
			return;
		}
		catch (runtime_error error)
		{
			cout << "\nMain Exception:\t"<<error.what()<<"\n";
			ReleaseMemory();
			cout << "\nTry Again? Enter y or n:\n";
			char choose;
			cin >> choose;
			if (!cin || choose == 'n' || choose == 'N')return;
		}
	}
	cout << "Please don't run this *.exe directly!\n";
	cout << "Instead, run the *.bat with the command line parameter with *.hxj path!\n\nGoodbye!";
	system("pause");
 }
#endif