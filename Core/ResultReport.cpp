#include "stdafx.h"
#include "ResultReport.h"


void Core::ResultReport::InitialWriter(string filename)
{
	remove(filename.c_str());
	ResultLog = spd::basic_logger_mt("result", filename);
	ResultLog->info("Begin to Calculate\n");
	ResultLog->set_pattern("%v");
}

void Core::ResultReport::WriteMeshInformation(Mesh * mesh, string filename, shared_ptr<spd::logger>log)
{
	log->info("{:*^45}", filename);
	log->info("Number of Nodes:\t\t\t{:12d}", mesh->NodeSize());
	log->info("Number of Triangles:\t\t\t{:12d}", mesh->TriangleSize());
	log->info("Number of Segments:\t\t\t{:12d}", mesh->SegmentSize());
	log->info("Number of Cuboids:\t\t\t{:12d}", mesh->CuboidSize());
	log->info("Number of Tetrahedras:\t\t\t{:12d}\n", mesh->TetrahedraSize());
}

void Core::ResultReport::WriteGeometeryInformation(Assist::ModalGeometry *geo, shared_ptr<spd::logger>log)
{
	log->info("{:*^45})", "Geometry Info");
	log->info("Lower Point of Box is ({0:+5.3f},{1:+5.3f},{2:+5.3f})", 
		geo->GetLimitationBoundary(0).x(), 
		geo->GetLimitationBoundary(0).y(),
		geo->GetLimitationBoundary(0).z());
	log->info("Upper Point of Box is ({0:+5.3f},{1:+5.3f},{2:+5.3f})", 
		geo->GetLimitationBoundary(7).x(),
		geo->GetLimitationBoundary(7).y(),
		geo->GetLimitationBoundary(7).z());
}

void Core::ResultReport::WriteBasicFunctionInformation(size_t unknowns, shared_ptr<spd::logger>log)
{
	log->info("{:*^45})", "Basic Function Info");
	log->info("Type:\t\t\tRWG");
	log->info("Number of Unknowns:\t\t\t{:12d}\n",unknowns);
}

void Core::ResultReport::WriteGreenFunctionInformation(GreenConfiguration * config, shared_ptr<spd::logger>log)
{
	switch (config->GreenType)
	{
	case 1:
		throw spdlog::spdlog_ex("Other GreenFunction is not developing!");
	default:
		log->info("Free Space Green Function is choosen\n");

	}
}

void Core::ResultReport::WriteExcitationInformation(Source::ISource * config, shared_ptr<spd::logger>log)
{
	log->info("{:*^45}", "Excitation Info");
	switch (config->GetSourceType())
	{
	case Source::SourceType::EXCITATION_LINEAR:
		log->info("Source Type:\t\tPlanewaveLinear");
		log->info("Direction:[{0:5.3f} {1:5.3f} {2:5.3f}]",
			static_cast<Source::Planewave*>(config)->Ki.x(), 
			static_cast<Source::Planewave*>(config)->Ki.y(), 
			static_cast<Source::Planewave*>(config)->Ki.z());
		log->info("Polarization:[{0:5.3f} {1:5.3f} {2:5.3f}]\n",
			static_cast<Source::Planewave*>(config)->Ei.x(), 
			static_cast<Source::Planewave*>(config)->Ei.y(), 
			static_cast<Source::Planewave*>(config)->Ei.z());
		return;
	case Source::SourceType::EXCITATION_CIRC_LEFT:
		log->info("Source Type:\t\tPlanewaveLeft");
		log->info("Direction:[{0:5.3f} {1:5.3f} {2:5.3f}]",
			static_cast<Source::Planewave*>(config)->Ki.x(),
			static_cast<Source::Planewave*>(config)->Ki.y(),
			static_cast<Source::Planewave*>(config)->Ki.z());
		log->info("Polarization:[{0:5.3f} {1:5.3f} {2:5.3f}]\n",
			static_cast<Source::Planewave*>(config)->Ei.x(),
			static_cast<Source::Planewave*>(config)->Ei.y(),
			static_cast<Source::Planewave*>(config)->Ei.z());
		return;
	case Source::SourceType::EXCITATION_CIRC_RIGHT:
		log->info("Source Type:\t\tPlanewaveRight");
		log->info("Direction:[{0:5.3f} {1:5.3f} {2:5.3f}]",
			static_cast<Source::Planewave*>(config)->Ki.x(),
			static_cast<Source::Planewave*>(config)->Ki.y(),
			static_cast<Source::Planewave*>(config)->Ki.z());
		log->info("Polarization:[{0:5.3f} {1:5.3f} {2:5.3f}]\n",
			static_cast<Source::Planewave*>(config)->Ei.x(),
			static_cast<Source::Planewave*>(config)->Ei.y(),
			static_cast<Source::Planewave*>(config)->Ei.z());
		return;
	case Source::SourceType::EXCITATION_VOLTAGE:
		log->info("Source Type:\t\tVoltage");
		return;
	case Source::SourceType::EXCITATION_VOLTAGE_DISC:
		log->info("Source Type:\t\tVoltageDisc");
		return;
	default: throw new spdlog::spdlog_ex("Error in Output Excitation");
	}
}

void Core::ResultReport::WriteSolutionInformation(Solution::SolverConfiguration * config, shared_ptr<spd::logger>log)
{
	log->info("{:*^45}", "Solution Info");

	switch (config->Sol)
	{
	case Solution::LU: throw spd::spdlog_ex("LU solver is not developed");
	case Solution::BiCGStab: 
		log->info("Solution Type:\tBiCGSTAB");break;
	default:
		throw spd::spdlog_ex("Solution Type is error");
	}

	switch (config->Precond)
	{
	case Solution::Identity:
		log->info("Preconditioning Type:\t\tNo");
		break;
	case Solution::Jacobi:
		log->info("Preconditioning Type:\t\tJacobi");
		break;
	case Solution::ILU:
		log->info("Preconditioning Type:\t\tIncomplete LU");
		break;
	default:throw spd::spdlog_ex("Precondition is error");
	}
	log->info("The Maxiteration is:\t\t{:d}", config->Maxiteration);
	log->info("The Tolerance is:\t\t{:e}", config->Tolerance);
}

void Core::ResultReport::WriteMethodInformation(ImpConfiguration*config, shared_ptr<spd::logger>log)
{
	switch (config->impType)
	{
	case AIM:
		log->info("{:*^45}","AIM Parameters");
		log->info("Order:\t{}", config->GridOrder);
		log->info("Interval:\t{:4.2f}", config->Interval);
		log->info("Threshold:\t{:4.2f}", config->Threshold);
		log->info("NearTolerance:\t{:e}", config->NearCorrectionEps);
		log->info("Dimension:\t{}", config->Dimension);
		log->info("VirtualGrid:\t{}", config->VirtualGridTechnique);
		log->info("TFS:\t{}", config->FillingStrategy);
		log->info("AIMBox is from ({0:+5.3f},{1:+5.3f},{2:+5.3f}) to ({3:+5.3f},{4:+5.3f},{5:+5.3f})",
			config->Box[0].x(), config->Box[0].y(), config->Box[0].z(),
			config->Box[1].x(), config->Box[1].y(), config->Box[1].z());
		log->info("LayerNumber:\t({0},{1},{2})\n", config->xNumber,config->yNumber, config->zNumber);
		return;
	case MUL:
		throw spd::spdlog_ex("Mul Algorithm is not Developing");
	default:
		log->info("{:*^45}", "MoM");
	}
	
}

void Core::ResultReport::WriteRequestInformation(Request::FarFieldConfiguration * config, shared_ptr<spd::logger>log)
{
	log->info("RCS of Far Field::\t" + config->FarFileName);
}

void Core::ResultReport::WirteIEInformation(IEConfiguration * config, shared_ptr<spd::logger> log)
{
	switch (config->type)
	{
	case EFIE: log->info("EFIE is choosen");break;
	case MFIE: log->info("MFIE is choosen");break;
	case CFIE: log->info("CFIE is choosen");
		log->info("Alpha is {}", config->Alpha);
		log->info("Eta is {}", config->Eta);
		break;
	case IBCEFIE: throw spdlog::spdlog_ex("IBCEFIE is not developing!");
	case IBCMFIE: throw spdlog::spdlog_ex("IBCMFIE is not developing!");
	case IBCCFIE:throw spdlog::spdlog_ex("IBCCFIE is not developing!");
	default: throw spdlog::spdlog_ex("Other IE is not developing!");
	}
	

}
