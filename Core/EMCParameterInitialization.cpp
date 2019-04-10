#include "stdafx.h"
#include "CoreAPI.h"
#include "Const.h"
#include "Data.h"
#include "TxtReader.h"
#include "Card.h"
#include "FarField.h"
#include "Log.h"
#include <mkl.h>
#include "ResultReport.h"
using namespace std;




static void SetFF(list<string>::const_iterator& ptr);

static void SetA0(list<string>::const_iterator& ptr);

static void SetCG(list<string>::const_iterator& ptr);

int Core::EMCParameterInitialization(char* configurationFileName)
{
	RuntimeL->info("Run EMCParmeterInitialization()");
	RuntimeL->info("Configuration is from:\t{}", configurationFileName);
	try
	{
		if (TReader.Read(configurationFileName))
		{
			auto zmc = TReader.GetContext().cbegin();
			const auto ed = TReader.GetContext().cend();
			SystemConfig.PostConfig.clear();
			Console->info("Read Configuration");
			string outfileName;
			while (zmc != ed)
			{
				const Card hxjCard = static_cast<Card>(stoi(*zmc++));
				switch (hxjCard)
				{
				case IM:
					++zmc; ++zmc;
					SystemConfig.MeshFileName = *zmc++;
					SystemConfig.ProjectDir = *zmc++;
					SystemConfig.ProjectName = *zmc++;
					SystemConfig.BasicFunctionFileName = SystemConfig.ProjectDir
						+ '\\' + SystemConfig.ProjectName + ".bf";

					outfileName = SystemConfig.ProjectDir + "\\" + SystemConfig.ProjectName + ".out";
					//ResultL
					ResultReport::InitialWriter(SystemConfig.ReportFileName);
					
					ResultL->info("The Project Name:\t{}", SystemConfig.ProjectName);
					ResultL->info("Mesh is From:\t{}", SystemConfig.MeshFileName);

					Console->info("Project Directory:\t{}", SystemConfig.ProjectDir);
					Console->info("Project Name:\t{}", SystemConfig.ProjectName);
					Console->info("Mesh File Path:\t{}", SystemConfig.MeshFileName);
					/*cout << "\n\tProject Directory:\t" << SystemConfig.ProjectDir
					<<"\n\tProject Name:\t"<< SystemConfig.ProjectName
					<<"\n\tMesh File Path:\t" << SystemConfig.MeshFileName << "\n";*/
					break;
				case FR:
					Frequency = stod(*zmc++);
					Console->info("Frequency:\t{:e}", Frequency);
					ResultL->info("Frequency:\t{:e}", Frequency);
					//cout << "\nSet Frequency\t=\t" << Frequency << " (Hz)\n";
					Omega = 2 * M_PI*Frequency;
					k = Omega / c0;
					Lambda = c0 / Frequency;
					break;
				case AM:
					
					SystemConfig.ImpConfig.GridOrder = stoi(*zmc++);
					SystemConfig.ImpConfig.Interval = stod(*zmc++);
					SystemConfig.ImpConfig.Threshold = stod(*zmc++);
					SystemConfig.ImpConfig.NearCorrectionEps = stod(*zmc++);
					SystemConfig.ImpConfig.Dimension = stoi(*zmc++);
					SystemConfig.ImpConfig.impType = EImpedance(AIM);
					SystemConfig.ImpConfig.FillingStrategy = stoi(*zmc++);
					SystemConfig.ImpConfig.VirtualGridTechnique = stoi(*zmc++);
					if(SystemConfig.ImpConfig.VirtualGridTechnique==1)
						Console->info("VirtualGrid Technique is used in AIM");
					else Console->info("Conventional AIM is used");
					break;
				case A0:SetA0(zmc);
					break;
				case FF:SetFF(zmc);
					break;
				case CG:SetCG(zmc);
					break;
				case OS:
					SystemConfig.BasicFunctionType=stoi(*zmc++);//RWG Type
					SystemConfig.BasicFunctionFileName = *zmc++;
					break;
				case EN:zmc = ed; break;
				default:break;
				}
			}

			MKL_Set_Dynamic(true);
			RuntimeL->info("Finish");
			RuntimeL->flush();
			return 0;
		}
		throw spd::spdlog_ex("Error in Read " + string(configurationFileName));
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return 1;
	}
	
}
//Plane wave
static void SetA0(list<string>::const_iterator& ptr)
{
	SystemConfig.SourceConfig.ThetaNum = stoi(*ptr++);
	SystemConfig.SourceConfig.PhiNum = stoi(*ptr++);
	SystemConfig.SourceConfig.ThetaStart = stod(*ptr++);
	SystemConfig.SourceConfig.PhiStart = stod(*ptr++);
	SystemConfig.SourceConfig.ThetaIncrement = stod(*ptr++);
	SystemConfig.SourceConfig.PhiIncrement = stod(*ptr++);
	SystemConfig.SourceConfig.Polarisation = stoi(*ptr++);
	SystemConfig.SourceConfig.Magnitude = stod(*ptr++);
	SystemConfig.SourceConfig.Phase = stod(*ptr++);
	SystemConfig.SourceConfig.Eta = stod(*ptr++);
	SystemConfig.SourceConfig.RotationX = stod(*ptr++);
	SystemConfig.SourceConfig.RotationY = stod(*ptr++);
	SystemConfig.SourceConfig.RotationZ = stod(*ptr++);
	SystemConfig.SourceConfig.ExcitationName = *ptr++;
	SystemConfig.SourceConfig.SetDirection();
	/*Ei = SystemConfig.SourceConfig.Ei;
	Ki = SystemConfig.SourceConfig.Ki;*/
}

static void SetFF(list<string>::const_iterator& ptr)
{
	Request::FarFieldConfiguration hxjFF;
	
	hxjFF.FieldType = stoi(*ptr++);
	hxjFF.ThetaNum = stoi(*ptr++);
	hxjFF.PhiNum = stoi(*ptr++);
	hxjFF.ThetaStart = stod(*ptr++);
	hxjFF.PhiStart = stod(*ptr++);
	hxjFF.ThetaIncrement = stod(*ptr++);
	hxjFF.PhiIncrement = stod(*ptr++);
	hxjFF.FarFileName = *ptr++;

	SystemConfig.PostConfig.push_back(hxjFF);
}

//Solver
static void SetCG(list<string>::const_iterator& ptr)
{
	//++ptr;
	SystemConfig.SolverConfig.Sol = Solution::ESolutionType(stoi(*ptr++));
	//++ptr;
	SystemConfig.SolverConfig.Precond = Solution::EPreconditionerType(stoi(*ptr++));
	SystemConfig.SolverConfig.Maxiteration = stoi(*ptr++);
	SystemConfig.SolverConfig.Tolerance = stod(*ptr++);
	SystemConfig.SolverConfig.MaxStopTolerance = stod(*ptr++);
}