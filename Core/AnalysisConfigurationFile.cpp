#include "stdafx.h"
#include "CoreAPI.h"
#include "Const.h"
#include "Data.h"
#include "FarField.h"
#include "Log.h"
#include "ResultReport.h"
#include <mkl.h>
#include "tinyxml2/tinyxml2.h"

using namespace tinyxml2;
using namespace std;

static int SetFileMod(XMLElement* mod);
static int SetMethodMod(XMLElement* mod);
static int SetEMCParameterMod(XMLElement* mod);
static int SetExcitationMod(XMLElement* mod);
static int SetSolutionMod(XMLElement* mod);
static int SetRequestMod(XMLElement* mod);


//Major Function to Analysis the file
int Core::AnalysisConfigurationFile(char* filename)
{
	int status = 0;
	try
	{
		XMLDocument doc;
		doc.LoadFile(filename);
		if (doc.Error()) {
			status = doc.ErrorID();
			throw spd::spdlog_ex(doc.ErrorStr());
		}

		XMLElement* root = doc.FirstChildElement();

		Console->info("Read Configuration");
		RuntimeLog->info("{:*^45}", "Read Configuration");
		Console->debug("{0} = {1}", root->FirstAttribute()->Name(), root->FirstAttribute()->Value());

		

		status = SetFileMod(root->FirstChildElement("File"));
		if(status)throw spd::spdlog_ex("Error in Read File Module");

		status = SetMethodMod(root->FirstChildElement("Method"));
		if (status)throw spd::spdlog_ex("Error in Read Method Module");

		status = SetEMCParameterMod(root->FirstChildElement("EMCPara"));
		if (status)throw spd::spdlog_ex("Error in Read EMCParameter Module");

		status = SetExcitationMod(root->FirstChildElement("Excitation"));
		if (status)throw spd::spdlog_ex("Error in Read Excitation Module");

		status = SetSolutionMod(root->FirstChildElement("Solution"));
		if (status)throw spd::spdlog_ex("Error in Read Solution Module");

		status = SetRequestMod(root->FirstChildElement("Request"));
		if (status)throw spd::spdlog_ex("Error in Read Request Module");

		MKL_Set_Dynamic(true);

		return status;
	}
	catch (spd::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		return status;
	}
	
}

//Assist Functions---------------------------------------------------------------
static int SetFileMod(XMLElement* mod)
{
	XMLElement* card = mod->FirstChildElement("IN");
	Console->info("{:*^45}", "File Input");
	if(card->FirstAttribute()->BoolValue())
	{
		XMLElement* leaf = card->FirstChildElement("Mesh");
		//Meshtype
		SystemConfig.MeshFileName = leaf->FirstChildElement("FilePath")->GetText();

		leaf = card->FirstChildElement("Project");
		SystemConfig.ProjectName = leaf->FirstChildElement("Name")->GetText();
		SystemConfig.ProjectDir = leaf->FirstChildElement("Directory")->GetText();
		SystemConfig.ReportFileName = leaf->FirstChildElement("Report")->GetText();

		Console->info("Mesh File Path:\t{}", SystemConfig.MeshFileName);
		Console->info("Project Directory:\t{}", SystemConfig.ProjectDir);
		Console->info("Project Name:\t{}", SystemConfig.ProjectName);
		Console->info("Output FileName:\t"+ SystemConfig.ReportFileName);

		RuntimeLog->info("Mesh File Path:\t{}", SystemConfig.MeshFileName);
		RuntimeLog->info("Project Directory:\t{}", SystemConfig.ProjectDir);
		RuntimeLog->info("Project Name:\t{}", SystemConfig.ProjectName);
		RuntimeLog->info("Output FileName:\t" + SystemConfig.ReportFileName);
		//ResultLog
		ResultReport::InitialWriter(SystemConfig.ReportFileName);
	}

	card = mod->FirstChildElement("OS");

	if (card->FirstAttribute()->BoolValue())
	{
		SystemConfig.BasicFunctionType = card->FirstChildElement("Type")->IntText();//RWG Type
		SystemConfig.BasicFunctionFileName = card->FirstChildElement("FilePath")->GetText();
	}
	else SystemConfig.BasicFunctionFileName = SystemConfig.ProjectDir
		+ '\\' + SystemConfig.ProjectName + ".bf";

	Console->info("BasicFunction File Path:\t{}",SystemConfig.BasicFunctionFileName);
	RuntimeLog->info("BasicFunction File Path:\t{}", SystemConfig.BasicFunctionFileName);
	return 0;
}


static int SetMethodMod(XMLElement* mod)
{
	SystemConfig.ImpConfig.FillingStrategy = mod->FirstChildElement("TriangleFillingStrategy")->IntText();
	XMLElement* card = mod->FirstChildElement("AIM");


	//SystemConfig.IEConfig.type = CFIE;
	//SystemConfig.IEConfig.Alpha = 0.7;
	//SystemConfig.IEConfig.Eta = 120* M_PI;

	if (card->FirstAttribute()->BoolValue())
	{
		SystemConfig.ImpConfig.GridOrder = card->FirstChildElement("Order")->IntText();
		SystemConfig.ImpConfig.Interval = card->FirstChildElement("Interval")->DoubleText();
		SystemConfig.ImpConfig.Threshold = card->FirstChildElement("Threshold")->DoubleText();
		SystemConfig.ImpConfig.NearCorrectionEps = card->FirstChildElement("NearTolerance")->DoubleText();
		SystemConfig.ImpConfig.Dimension = card->FirstChildElement("Dimension")->IntText();
		SystemConfig.ImpConfig.impType = EImpedance(AIM);
		SystemConfig.ImpConfig.VirtualGridTechnique = card->FirstChildElement("VirtualGrid")->IntText();

		Console->debug("{:*^45}", "AIM Parameters::");
		Console->debug("Order:\t{}", SystemConfig.ImpConfig.GridOrder);
		Console->debug("Interval:\t{}", SystemConfig.ImpConfig.Interval);
		Console->debug("Threshold:\t{}", SystemConfig.ImpConfig.Threshold);
		Console->debug("NearTolerance:\t{}", SystemConfig.ImpConfig.NearCorrectionEps);
		Console->debug("Dimension:\t{}", SystemConfig.ImpConfig.Dimension);
		Console->debug("VirtualGrid:\t{}", SystemConfig.ImpConfig.VirtualGridTechnique);
		Console->debug("TFS:\t{}", SystemConfig.ImpConfig.FillingStrategy);
	}

	card = mod->FirstChildElement("MoM");

	if (card->FirstAttribute()->BoolValue())SystemConfig.ImpConfig.impType=EImpedance(MoM);

	card = mod->FirstChildElement("IE");

	if(card->FirstAttribute()->BoolValue())
	{
		SystemConfig.IEConfig.type = IETYPE(card->FirstChildElement("Type")->IntText());
		SystemConfig.IEConfig.Alpha = card->FirstChildElement("Alpha")->DoubleText();
		SystemConfig.IEConfig.Eta = card->FirstChildElement("Eta")->DoubleText();
		return 0;
	}

	return 1;
	
}


static int SetEMCParameterMod(XMLElement* mod)
{
	XMLElement* card = mod->FirstChildElement("FR");
	if (card->FirstAttribute()->BoolValue())
	{
		Frequency = card->FirstChildElement("SingleFrequency")->DoubleText();
		Omega = 2 * M_PI*Frequency;
		k = Omega / c0;
		Lambda = c0 / Frequency;
		Console->info("Frequency:\t{:e}", Frequency);
		RuntimeLog->info("Frequency:\t{:e}", Frequency);
		ResultLog->info("Frequency:\t{:e}", Frequency);
	}

	return 0;
}
static int SetExcitationMod(XMLElement* mod)
{

	Console->debug("{:*^45}", "Excitation");

	for (XMLElement* card = mod->FirstChildElement();card != nullptr;card = card->NextSiblingElement())
	{
		string name=card->FindAttribute("Name")->Value();
		if (card->Name() == string{ "A0" } )
		{
			int pol = card->FirstChildElement("Polarisation")->IntText();
			XMLElement* leaf = card->FirstChildElement("Theta");

			int thn = leaf->FirstChildElement("ThetaNumber")->IntText();
			double ths = leaf->FirstChildElement("ThetaStart")->DoubleText(),
				thi = leaf->FirstChildElement("ThetaIncrement")->DoubleText();

			leaf = card->FirstChildElement("Phi");
			int phn = leaf->FirstChildElement("PhiNumber")->IntText();
			double phs = leaf->FirstChildElement("PhiStart")->DoubleText(),
				phi = leaf->FirstChildElement("PhiIncrement")->DoubleText();

			leaf = card->FirstChildElement("Rotation");
			double rx = leaf->FirstChildElement("x")->DoubleText(),
				ry = leaf->FirstChildElement("y")->DoubleText(),
				rz = leaf->FirstChildElement("z")->DoubleText();

			leaf = card->FirstChildElement("EField");
			
			double mag = leaf->FirstChildElement("Magenitude")->DoubleText(),
				phase = leaf->FirstChildElement("Phase")->DoubleText(),
				eta = leaf->FirstChildElement("Eta")->DoubleText(),
				ell = leaf->FirstChildElement("Ellipticity")->DoubleText();
			//Source::SourceType st = pol > 0 ? Source::SourceType::EXCITATION_CIRC_RIGHT : pol < 0 ? Source::SourceType::EXCITATION_CIRC_LEFT : Source::SourceType::EXCITATION_LINEAR;

			SystemConfig.SourceConfig= new Source::PlaneWaveLinear(name, thn, phn, ths, phs,thi,  phi, mag,  phase,  eta, rx,  ry, rz);
			return 0;
		}
		
		if(card->Name()== string{ "AE" })
		{

			auto vg = new Source::VoltageGap(name);			
			for (XMLElement* leaf = card->FirstChildElement();leaf != nullptr;leaf = leaf->NextSiblingElement())
			{
				unsigned long ns=leaf->FirstChildElement("NegativeSide")->Int64Text(),
				ps=leaf->FirstChildElement("PositiveSide")->Int64Text();				
				double mag = leaf->FirstChildElement("Magenitude")->DoubleText(),
					phase = leaf->FirstChildElement("Phase")->DoubleText();
				vg->VoltageVector.push_back({ns,ps,mag*exp(1i*phase)});
			}
			SystemConfig.SourceConfig = vg;
			return 0;
		}
	}	
	return 4;
}
static int SetSolutionMod(XMLElement* mod)
{
	XMLElement* card = mod->FirstChildElement("CG");
	if (card->FirstAttribute()->BoolValue())
	{
		SystemConfig.SolverConfig.Sol = Solution::BiCGStab;
		SystemConfig.SolverConfig.Precond = Solution::EPreconditionerType(card->FirstChildElement("Preconditioning")->IntText());
		SystemConfig.SolverConfig.Maxiteration = card->FirstChildElement("Iterations")->IntText();
		SystemConfig.SolverConfig.Tolerance = card->FirstChildElement("Residum")->DoubleText();
		SystemConfig.SolverConfig.MaxStopTolerance = card->FirstChildElement("StopTolerance")->DoubleText();

		Console->debug("{:*^45}", "Solution Type::\tBiCG");
		return 0;
	}

	return 5;

}
static int SetRequestMod(XMLElement* mod)
{
	Console->debug("{:*^45}", "Request");
	for(XMLElement* card=mod->FirstChildElement();card != nullptr;card = card->NextSiblingElement())
	{
		if(string{ "FF" }==card->Name())
		{
			Request::FarFieldConfiguration ff;

			ff.FarFileName = card->FirstAttribute()->Value();

			XMLElement* leaf = card->FirstChildElement("Theta");
			ff.ThetaNum = leaf->FirstChildElement("ThetaNumber")->IntText();
			ff.ThetaStart = leaf->FirstChildElement("ThetaStart")->DoubleText();
			ff.ThetaIncrement = leaf->FirstChildElement("ThetaIncrement")->DoubleText();

			leaf = card->FirstChildElement("Phi");
			ff.PhiNum = leaf->FirstChildElement("PhiNumber")->IntText();
			ff.PhiStart = leaf->FirstChildElement("PhiStart")->DoubleText();
			ff.PhiIncrement = leaf->FirstChildElement("PhiIncrement")->DoubleText();

			ff.FieldType = 0;//RCS

			SystemConfig.PostConfig.push_back(ff);
			Console->debug("Far Field Request::\t" + ff.FarFileName);
		}
	}

	return 0;
}