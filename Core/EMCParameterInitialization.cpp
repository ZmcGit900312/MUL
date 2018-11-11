#include "stdafx.h"
#include "CoreAPI.h"
#include "Const.h"
#include "Data.h"
#include "TxtReader.h"
#include "Card.h"
#include "FarField.h"
#include "Log.h"
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
			SystemConfiguration::PostConfig.clear();
			Console->info("Read Configuration");
			string outfileName;
			while (zmc != ed)
			{
				const Card hxjCard = static_cast<Card>(stoi(*zmc++));
				switch (hxjCard)
				{
				case IM:
					++zmc; ++zmc;
					SystemConfiguration::MeshFileName = *zmc++;
					SystemConfiguration::ProjectDir = *zmc++;
					SystemConfiguration::ProjectName = *zmc++;
					SystemConfiguration::BasicFunctionFileName = SystemConfiguration::ProjectDir
						+ '\\' + SystemConfiguration::ProjectName + ".bf";

					outfileName = SystemConfiguration::ProjectDir + "\\" + SystemConfiguration::ProjectName + ".out";
					remove(outfileName.c_str());
					ResultL = spd::basic_logger_mt("result", outfileName);
					ResultL->set_pattern("%v");
					
					ResultL->info("The Project Name:\t{}", SystemConfiguration::ProjectName);
					ResultL->info("Mesh is From:\t{}", SystemConfiguration::MeshFileName);

					Console->info("Project Directory:\t{}", SystemConfiguration::ProjectDir);
					Console->info("Project Name:\t{}", SystemConfiguration::ProjectName);
					Console->info("Mesh File Path:\t{}", SystemConfiguration::MeshFileName);
					/*cout << "\n\tProject Directory:\t" << SystemConfiguration::ProjectDir
					<<"\n\tProject Name:\t"<< SystemConfiguration::ProjectName
					<<"\n\tMesh File Path:\t" << SystemConfiguration::MeshFileName << "\n";*/
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
					Console->info("Use Method:\tAIM");
					ResultL->info("Use Method:\tAIM");
					SystemConfiguration::ImpConfig.GridOrder = stoi(*zmc++);
					SystemConfiguration::ImpConfig.Interval = stod(*zmc++);
					SystemConfiguration::ImpConfig.Threshold = stod(*zmc++);
					SystemConfiguration::ImpConfig.NearCorrectionEps = stod(*zmc++);
					SystemConfiguration::ImpConfig.Dimension = stoi(*zmc++);
					SystemConfiguration::ImpConfig.impType = AIM;
					SystemConfiguration::ImpConfig.FillingStrategy = stoi(*zmc++);
					break;
				case A0:SetA0(zmc);
					break;
				case FF:SetFF(zmc);
					break;
				case CG:SetCG(zmc);
					break;
				case EN:zmc = ed; break;
				default:break;
				}
			}
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
	ComponentList::RightHand.ThetaNum = stoi(*ptr++);
	ComponentList::RightHand.PhiNum = stoi(*ptr++);
	ComponentList::RightHand.ThetaStart = stod(*ptr++);
	ComponentList::RightHand.PhiStart = stod(*ptr++);
	ComponentList::RightHand.ThetaIncrement = stod(*ptr++);
	ComponentList::RightHand.PhiIncrement = stod(*ptr++);
	ComponentList::RightHand.Polarisation = stoi(*ptr++);
	ComponentList::RightHand.Magnitude = stod(*ptr++);
	ComponentList::RightHand.Phase = stod(*ptr++);
	ComponentList::RightHand.Eta = stod(*ptr++);
	ComponentList::RightHand.RotationX = stod(*ptr++);
	ComponentList::RightHand.RotationY = stod(*ptr++);
	ComponentList::RightHand.RotationZ = stod(*ptr++);
	ComponentList::RightHand.ExcitationName = *ptr++;
	ComponentList::RightHand.Compute();
	Ei = ComponentList::RightHand.GetEi();
	Ki = ComponentList::RightHand.GetKi();
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

	SystemConfiguration::PostConfig.push_back(hxjFF);
}

//Solver
static void SetCG(list<string>::const_iterator& ptr)
{
	++ptr;
	//SystemConfiguration::SolverConfig.Sol = Solution::ESolutionType(stoi(*ptr++));
	++ptr;
	//SystemConfiguration::SolverConfig.Precond = Solution::ILU;
	SystemConfiguration::SolverConfig.Maxiteration = stoi(*ptr++);
	SystemConfiguration::SolverConfig.Tolerance = stod(*ptr++);
	SystemConfiguration::SolverConfig.MaxStopTolerance = stod(*ptr++);
	ComponentList::Solver=FSolver(SystemConfiguration::SolverConfig, SystemConfiguration::ImpConfig.impType);
}