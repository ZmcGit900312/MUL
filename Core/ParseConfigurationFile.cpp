#include "stdafx.h"
#include "CoreAPI.h"
#include "Const.h"
#include "Data.h"
#include "FarField.h"
#include "Log.h"
#include "ResultReport.h"
#include <mkl.h>
#include "rapidjson/document.h"
#include <rapidjson/istreamwrapper.h>
#include <iostream>
#include "rapidjson/pointer.h"


using namespace std;
namespace rj = rapidjson;


static rj::Document ReadJson(string filename)
{
	// read a JSON file

	std::ifstream ifs(filename);

	if (ifs.is_open())
	{
		rj::Document d;
		rj::IStreamWrapper isw(ifs);
		d.ParseStream(isw);

		ifs.clear();
		ifs.close();

		return d;
	}
	else throw spdlog::spdlog_ex("Read Error");

}

static int JsonInfo(rj::Document& jd);
static int SetFileMod(rj::Document& jd);
static int SetImpedanceMod(rj::Document& jd);
static int SetIEMod(rj::Document&jd);
static int SetSolverMod(rj::Document&jd);
static int SetExcitationMod(rj::Document&jd);
static int SetRequestMod(rj::Document & jd);
static int SetSweepMod(rj::Document& jd);

//The main function of read configuration
int Core::ParseConfiguratoinFile(char* filename)
{
	int status = 0;
	try
	{
		rj::Document jd = ReadJson(filename);

		status+= JsonInfo(jd);

		status+= SetFileMod(jd);

		status += SetImpedanceMod(jd);

		status += SetIEMod(jd);

		status += SetExcitationMod(jd);

		status += SetSolverMod(jd);

		status += SetSweepMod(jd);

		status += SetRequestMod(jd);

		MKL_Set_Dynamic(true);
	}
	catch(spdlog::spdlog_ex&ex)
	{
		Assist::LogException(ex);
		status = 1;
		
	}
	return status;
}

#pragma region AssisFun

int static JsonInfo(rj::Document& jd)
{
	
	if(jd["version"].IsString()&& jd["date"].IsString())
	{

		Console->info("version:\t{0:<10}", jd["version"].GetString());
		Console->info("date:   \t{0:<10}",jd["date"].GetString());

		return 0;
	}
	throw spdlog::spdlog_ex("Json version or Data is wrong!");

}

int SetFileMod(rj::Document & jd)
{

	Console->info("{:*^45}", "File Input");
	if(jd.HasMember("File"))
	{
		rj::Value& jfile = jd["File"];
		//Project
		if(jfile.HasMember("Project"))
		{
			rj::Value& jpro = jfile["Project"];

			if(jpro.HasMember("Name"))
			{
				SystemConfig.ProjectName = jpro["Name"].GetString();
			}
			else throw spdlog::spdlog_ex("Error in json:\t./File/Project/Name");

			if (jpro.HasMember("Directory"))
			{
				SystemConfig.ProjectDir = jpro["Directory"].GetString();
			}
			else throw spdlog::spdlog_ex("Error in json:\t./File/Project/Name");

			if (jpro.HasMember("Report"))
			{
				SystemConfig.ReportFilePath = jpro["Report"].GetString();
			}
			else throw spdlog::spdlog_ex("Error in json:\t./File/Project/Name");

			Console->info("Project Directory:{:<40}", SystemConfig.ProjectDir);
			Console->info("Project Name:     {:<40}", SystemConfig.ProjectName);
			Console->info("Report File Path: {:<40}", SystemConfig.ReportFilePath);
			//Report initial
			ResultReport::InitialWriter(SystemConfig.ReportFilePath);
		}
		else throw spdlog::spdlog_ex("Error in json:\t./File/Project");

		//Mesh
		if (jfile.HasMember("Mesh"))
		{
			rj::Value& jmesh = jfile["Mesh"];

			if (jmesh.HasMember("Category"))
			{
				int category = jmesh["Category"].GetInt64();
			}
			else throw spdlog::spdlog_ex("Error in json:\t./File/Mesh/Category");

			if (jmesh.HasMember("FilePath"))
			{
				SystemConfig.MeshFilePath = jmesh["FilePath"].GetString();
			}
			else throw spdlog::spdlog_ex("Error in json:\t./File/Project/Name");

			Console->info("Mesh File Path   : {:<40}", SystemConfig.MeshFilePath);

		}
		else throw spdlog::spdlog_ex("Error in json:\t./File/Mesh");

		//OS
		if (jfile.HasMember("OS"))
		{
			rj::Value& jos = jfile["OS"];
			if(jos.IsNull())
			{
				SystemConfig.BasisFunctionType = 0;
				SystemConfig.BasisFunctionFilePath = SystemConfig.ProjectDir
					+ '\\' + SystemConfig.ProjectName + ".bf";
				SystemConfig.CurrentFilePath = SystemConfig.ProjectDir
					+ '\\' + SystemConfig.ProjectName + ".cu";
			}
			else
			{
				if (jos.HasMember("Category"))
				{
					SystemConfig.BasisFunctionType = jos["Category"].GetInt64();
				}
				else throw spdlog::spdlog_ex("Error in json:\t./File/OS/Category");

				if (jos.HasMember("BasisFunctionFilePath"))
				{
					SystemConfig.BasisFunctionFilePath = jos["BasisFunctionFilePath"].GetString();
				}
				else throw spdlog::spdlog_ex("Error in json:\t./File/OS/BasisFunctionFilePath");

				if (jos.HasMember("CurrentFilePath"))
				{
					SystemConfig.CurrentFilePath = jos["CurrentFilePath"].GetString();
				}
				else throw spdlog::spdlog_ex("Error in json:\t./File/OS/CurrentFilePath");
			}
			
			Console->info("BasisFunction File Path: {:<40}", SystemConfig.BasisFunctionFilePath);
			Console->info("Current File Path:       {:<40}", SystemConfig.BasisFunctionFilePath);
		}
		else throw spdlog::spdlog_ex("Error in json:\t./File/OS");
	}
	else throw spdlog::spdlog_ex("Error in json:\t./File");

	return 0;
}

int SetImpedanceMod(rj::Document & jd)
{
	Console->debug("{:*^45}", "IMPEDANCE PARAMETERS");
	if(jd.HasMember("Impedance"))
	{
		rj::Value& jimp = jd["Impedance"];
		if(jimp.HasMember("Category"))
		{
			SystemConfig.ImpConfig.ImpType=EImpedance(jimp["Category"].GetInt64());
		}
		else throw spdlog::spdlog_ex("Error in json:\t./Impedance/Category");

		if(jimp.HasMember("TriangleFillingStrategy"))
		{
			SystemConfig.ImpConfig.FillingStrategy = jimp["TriangleFillingStrategy"].GetBool() ? 1 : 0;
		}
		else throw spdlog::spdlog_ex("Error in json:\t./Impedance/TriangleFillingStrategy");

		if (jimp.HasMember("VirtualGridTechnique"))
		{
			SystemConfig.ImpConfig.VirtualGridTechnique = jimp["VirtualGridTechnique"].GetBool() ? 1 : 0;
		}
		else throw spdlog::spdlog_ex("Error in json:\t./Impedance/VirtualGridTechnique");

		if(jimp.HasMember("Dimension"))
		{
			SystemConfig.ImpConfig.Dimension = jimp["Dimension"].GetInt64();
		}
		else throw spdlog::spdlog_ex("Error in json:\t./Impedance/Dimension");

		if (jimp.HasMember("GridOrder"))
		{
			SystemConfig.ImpConfig.GridOrder = jimp["GridOrder"].GetInt64();
		}
		else throw spdlog::spdlog_ex("Error in json:\t./Impedance/GridOrder");

		if (jimp.HasMember("ArrayNumX"))
		{
			SystemConfig.ImpConfig.ArrayNumX = jimp["ArrayNumX"].GetInt64();
		}
		else throw spdlog::spdlog_ex("Error in json:\t./Impedance/ArrayNumX");

		if (jimp.HasMember("ArrayNumY"))
		{
			SystemConfig.ImpConfig.ArrayNumY = jimp["ArrayNumY"].GetInt64();
		}
		else throw spdlog::spdlog_ex("Error in json:\t./Impedance/ArrayNumY");

		if (jimp.HasMember("Threshold"))
		{
			SystemConfig.ImpConfig.Threshold = jimp["Threshold"].GetDouble();
		}
		else throw spdlog::spdlog_ex("Error in json:\t./Impedance/Threshold");

		if (jimp.HasMember("NearThresholdTolerance"))
		{
			SystemConfig.ImpConfig.NearCorrectionEps = jimp["NearThresholdTolerance"].GetDouble();
		}
		else throw spdlog::spdlog_ex("Error in json:\t./Impedance/NearThresholdTolerance");

		if (jimp.HasMember("ArrayIntervalX"))
		{
			SystemConfig.ImpConfig.ArrayIntervalX = jimp["ArrayIntervalX"].GetDouble();
		}
		else throw spdlog::spdlog_ex("Error in json:\t./Impedance/ArrayIntervalX");

		if (jimp.HasMember("ArrayIntervalY"))
		{
			SystemConfig.ImpConfig.ArrayIntervalY = jimp["ArrayIntervalY"].GetDouble();
		}
		else throw spdlog::spdlog_ex("Error in json:\t./Impedance/ArrayIntervalY");

		
		Console->debug("ImpedanceType: \t{:>10}",SystemConfig.ImpConfig.ImpType);
		Console->debug("VGT:           \t{:>10}", SystemConfig.ImpConfig.VirtualGridTechnique);
		Console->debug("TFS:           \t{:>10}", SystemConfig.ImpConfig.FillingStrategy);
		Console->debug("Order:         \t{:>10}", SystemConfig.ImpConfig.GridOrder);
		Console->debug("Interval:      \t{:>10}", SystemConfig.ImpConfig.Interval);
		Console->debug("Threshold:     \t{:>10}", SystemConfig.ImpConfig.Threshold);
		Console->debug("NearTolerance: \t{:>10}", SystemConfig.ImpConfig.NearCorrectionEps);
		Console->debug("Dimension:     \t{:>10}", SystemConfig.ImpConfig.Dimension);
		Console->debug("ArrayNumX:     \t{:>10}", SystemConfig.ImpConfig.ArrayNumX);
		Console->debug("ArrayNumY:     \t{:>10}", SystemConfig.ImpConfig.ArrayNumY);
		Console->debug("ArrayIntervalX:\t{:>10}", SystemConfig.ImpConfig.ArrayIntervalX);
		Console->debug("ArrayIntervalY:\t{:>10}", SystemConfig.ImpConfig.ArrayIntervalY);

		
	}
	else throw spdlog::spdlog_ex("Error in json:\t./Impedance");
	return 0;


}

int SetIEMod(rj::Document & jd)
{

	Console->debug("{:*^45}", "INTEGRAL EQUATION");

	if(jd.HasMember("IE"))
	{
		rj::Value& jie = jd["IE"];

		if(jie.HasMember("Category"))
		{
			SystemConfig.IEConfig.type = IETYPE(jie["Category"].GetInt64());
		}
		else throw spdlog::spdlog_ex("Error in json:\t./IE/Category");

		if (jie.HasMember("Alpha"))
		{
			SystemConfig.IEConfig.Alpha = jie["Alpha"].GetDouble();
		}
		else throw spdlog::spdlog_ex("Error in json:\t./IE/Alpha");

		if (jie.HasMember("Eta"))
		{
			SystemConfig.IEConfig.Eta = jie["Eta"].GetDouble();
		}
		else throw spdlog::spdlog_ex("Error in json:\t./IE/Eta");

		if (jie.HasMember("Zs"))
		{
			double zs = jie["Zs"].GetDouble();
		}
		else throw spdlog::spdlog_ex("Error in json:\t./IE/Zs");

		Console->debug("Integral Equation:\t{:>10}",SystemConfig.IEConfig.type);
		Console->debug("Alpha:            \t{:>10}", SystemConfig.IEConfig.Alpha);
		Console->debug("Eta:              \t{:>10}", SystemConfig.IEConfig.Eta);
	}
	else throw spdlog::spdlog_ex("Error in json:\t./IE");
	return 0;
}

int SetSolverMod(rj::Document& jd)
{
	Console->debug("{:*^45}", "SOLVER");
	if(jd.HasMember("Solver"))
	{
		rj::Value& jsolver = jd["Solver"];
		if (jsolver.HasMember("SolutionType"))
		{
			SystemConfig.SolverConfig.SolutionType = 
				Solution::ESolutionType(jsolver["SolutionType"].GetInt64());
		}
		else throw spdlog::spdlog_ex("Error in json:\t./Solver/SolutionType");

		if (jsolver.HasMember("Preconditioning"))
		{
			SystemConfig.SolverConfig.Precond = Solution::EPreconditionerType(jsolver["Preconditioning"].GetInt64());
		}
		else throw spdlog::spdlog_ex("Error in json:\t./Solver/Preconditioning");

		if (jsolver.HasMember("StopTolerance"))
		{
			SystemConfig.SolverConfig.StopTolerance = jsolver["StopTolerance"].GetDouble();
		}
		else throw spdlog::spdlog_ex("Error in json:\t./Solver/StopTolerance");

		if (jsolver.HasMember("Residum"))
		{
			SystemConfig.SolverConfig.Residum = jsolver["Residum"].GetDouble();
		}
		else throw spdlog::spdlog_ex("Error in json:\t./Solver/Residum");

		if (jsolver.HasMember("Iterations"))
		{
			SystemConfig.SolverConfig.Maxiteration = jsolver["Iterations"].GetInt64();
		}
		else throw spdlog::spdlog_ex("Error in json:\t./Solver/Category");

		Console->debug("Solution Type:     \t{:>10}", SystemConfig.SolverConfig.SolutionType);
		Console->debug("Precondition Type: \t{:>10}", SystemConfig.SolverConfig.Precond);
		Console->debug("StopTolerance:     \t{:>10}", SystemConfig.SolverConfig.StopTolerance);
		Console->debug("Residum:           \t{:>10}", SystemConfig.SolverConfig.Residum);
		Console->debug("Maxiteration:      \t{:>10}", SystemConfig.SolverConfig.Maxiteration);
	}
	else throw spdlog::spdlog_ex("Error in json:\t./Solver");
	
	return 0;
}

int SetExcitationMod(rj::Document& jd)
{
	Console->debug("{:*^45}", "EXCITATION");
	if (jd.HasMember("Excitation"))
	{
		Source::SourceType category;
		string name;

		rj::Value& jsource = jd["Excitation"];
		if (jsource.HasMember("Category"))
		{//NEED ADD PROPERTY IN ISOURCE
			category = Source::SourceType(jsource["Category"].GetInt64());
		}
		else throw spdlog::spdlog_ex("Error in json:\t./Excitation/Category");


		if (jsource.HasMember("Name"))
		{
			name = jsource["Name"].GetString();
		}
		else throw spdlog::spdlog_ex("Error in json:\t./Excitation/Name");

		if(category==Source::EXCITATION_LINEAR)
		{
			int pol = -1;
			int nt = 0, np = 0;
			double st = -1, it = -1, sp = -1, ip = -1;
			double rx = 0, ry = 0, rz = 0;
			double eta = 0.0, mag = 1.0, phase = 0.0, ell = 0.0;

			if (jsource.HasMember("Polarisation"))
			{//NEED ADD PROPERTY IN ISOURCE
				pol = jsource["Polarisation"].GetInt64();
			}
			else throw spdlog::spdlog_ex("Error in json:\t./Excitation/Polarisation");

			//Theta
			rj::Value& theta = jsource["Theta"];
			if (theta.IsArray())
			{
				nt = theta[0].GetInt64();
				st = theta[1].GetDouble(), it = theta[2].GetDouble();
			}
			else throw spdlog::spdlog_ex("Error in json:\t./Excitation/Theta");

			//Phi
			rj::Value& phi = jsource["Phi"];
			if (phi.IsArray())
			{
				np = phi[0].GetInt64();
				sp = phi[1].GetDouble(), ip = phi[2].GetDouble();
			}
			else throw spdlog::spdlog_ex("Error in json:\t./Excitation/Phi");

			//Rotation
			rj::Value& jr = jsource["Rotation"];
			if (jr.IsArray())
			{
				rx = jr[0].GetDouble();
				ry = jr[1].GetDouble();
				rz = jr[2].GetDouble();	
			}
			else throw spdlog::spdlog_ex("Error in json:\t./Excitation/Rotation");

			rj::Value& jfield = jsource["EField"];
			if (jfield.IsObject())
			{

				eta = jfield["Eta"].GetDouble();
				mag = jfield["Magnitude"].GetDouble();
				phase = jfield["Phase"].GetDouble();
				ell = jfield["Ellipticity"].GetDouble();
			}
			else throw spdlog::spdlog_ex("Error in json:\t./Excitation/EField");

			//Initial Source
			SystemConfig.SourceConfig = new Source::PlaneWaveLinear
			(name, nt, np, st, sp, it, ip, mag, phase, eta, rx, ry, rz);
			
			Console->debug("{:-^45}", SystemConfig.SourceConfig->Name);
			Console->debug("Theta:\tNum={0:<10} \tStart={1:<10}\tIncrement={2:<10}",
				nt, st, it);
			Console->debug("Phi:  \tNum={0:<10} \tStart={1:<10}\tIncrement={2:<10}",
				np, sp, ip);
			Console->debug("Rotation:    \t({0},{1},{2})", rx, ry, rz);
			Console->debug("Eta:         \t{0}", eta);
			Console->debug("Magnitude:   \t{0}", mag);
			Console->debug("Phase:       \t{0}", phase);
			Console->debug("Ellipticity: \t{0}", ell);
			return 0;
		}

		if(category == Source::EXCITATION_VOLTAGE)
		{
			auto vg = new Source::VoltageGap(name);

			if(jsource.HasMember("Port")&&jsource["Port"].IsArray())
			{
				for (rj::Value::ValueIterator pt=jsource["Port"].Begin();
					pt!=jsource["Port"].End();++pt)
				{
					rj::Value& zmcpt = *pt;
					Source::VoltageGap::Vol port;

					port.NegativeSide = zmcpt["NegativeSide"].GetUint64();
					port.PositiveSide = zmcpt["PositiveSide"].GetUint64();
					double mag = zmcpt["Magnitude"].GetDouble(), 
					phase = zmcpt["Phase"].GetDouble();
					port.Voltage = mag * exp(1i*phase);
					vg->VoltageVector.push_back(port);
				}
				SystemConfig.SourceConfig = vg;
			}
			else throw spdlog::spdlog_ex("Error in json:\t./Excitation/Port");
			
			return 0;
		}

		if (category == Source::EXCITATION_CIRC_LEFT)throw spdlog::spdlog_ex("New Excitation is not Developed!");
		if (category == Source::EXCITATION_CIRC_RIGHT)throw spdlog::spdlog_ex("New Excitation is not Developed!");
		if (category == Source::EXCITATION_CURRENT)throw spdlog::spdlog_ex("New Excitation is not Developed!");
		if (category == Source::EXCITATION_VOLTAGE_DISC)throw spdlog::spdlog_ex("New Excitation is not Developed!");

		return 1;

	}
	else throw spdlog::spdlog_ex("Error in json:\t./Excitation");
	
}

int SetRequestMod(rj::Document& jd)
{
	Console->debug("{:*^45}", "REQUEST");

	if(jd.HasMember("Request"))
	{
		rj::Value& jrequest = jd["Request"];

		for (rj::Value::ValueIterator vp = jrequest.Begin();vp != jrequest.End();++vp)
		{
			rj::Value& val = *vp;

			Request::FarFieldConfiguration ff;
			ff.FieldType = 0;//RCS
			ff.FarFileName = val["Name"].GetString();

			//Theta
			rj::Value& theta = val["Theta"];
			if (theta.IsArray())
			{
				ff.ThetaNum = theta[0].GetInt64();
				ff.ThetaStart = theta[1].GetDouble();
				ff.ThetaIncrement = theta[2].GetDouble();
			}

			//Phi
			rj::Value& phi = val["Phi"];
			if (phi.IsArray())
			{

				ff.PhiNum = phi[0].GetInt64();
				ff.PhiStart = phi[1].GetDouble();
				ff.PhiIncrement = phi[2].GetDouble();
			}

			Console->debug("{:-^45}", ff.FarFileName);
			Console->debug("Theta:\tNum={0:<10} \tStart={1:<10}\tIncrement={2:<10}", 
				ff.ThetaNum, ff.ThetaStart, ff.ThetaIncrement);
			Console->debug("Phi:  \tNum={0:<10} \tStart={1:<10}\tIncrement={2:<10}",
				ff.PhiNum, ff.PhiStart, ff.PhiIncrement);
			SystemConfig.PostConfig.push_back(ff);
		}

		return 0;
	}
	else throw spdlog::spdlog_ex("Error in json:\t./Request");
}

int SetSweepMod(rj::Document& jd)
{
	Console->debug("{0:*^45}", "SWEEP");

	if (jd.HasMember("Sweep"))
	{
		rj::Value& jsweep = jd["Sweep"];
		auto info = Solution::CurrentInfo::GetInstance();
		info->Current.clear();
		info->category = SystemConfig.ImpConfig.ImpType;

		for (rj::Value::ValueIterator sp = jsweep.Begin();sp != jsweep.End();++sp)
		{
			rj::Value& rsp = *sp;

			Solution::ArrayCurrent* zmc_config = nullptr;

			switch (info->category)
			{
			case MoM:
			case AIM: 
				info->Current.push_back(new Solution::ElementCurrent(0, rsp["Frequency"].GetDouble(), rsp["Tag"].GetString()));
				break;
			case EImpedance::Array: 
				zmc_config = new Solution::ArrayCurrent(
					0, 
					rsp["Frequency"].GetDouble(), 
					rsp["Tag"].GetString(), 
					rsp["Number"].GetInt64(),
					SystemConfig.ImpConfig.ArrayIntervalX,
					SystemConfig.ImpConfig.ArrayIntervalY);

				for (rj::Value::ValueIterator ap = rsp["ArrayLocation"].Begin();
					ap != rsp["ArrayLocation"].End();++ap)
				{
					Vector2i al{ (*ap)[0].GetInt64(), (*ap)[1].GetInt64() };
					zmc_config->_arrayLocation.push_back(al);
				}
				info->Current.push_back(zmc_config);
				break;
			default: throw spdlog::spdlog_ex("Not developing Impedance Method among AIM|MoM|Array");
			}	
			Console->debug("Tag:      \t{0:<15}", info->Current.back()->Tag);
			Console->debug("Frequency:\t{0:<15}", info->Current.back()->_frequency);
		}
		info->_numberOfConfig = info->Current.size();

		return 0;
	}
	else throw spdlog::spdlog_ex("Error in json:\t./Sweep");
}


#pragma endregion
