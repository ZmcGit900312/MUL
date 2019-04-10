#include "stdafx.h"
#include "ResultReport.h"


void Core::ResultReport::InitialWriter(string filename)
{
	remove(filename.c_str());
	ResultL = spd::basic_logger_mt("result", filename);
	ResultL->info("Begin to Calculate\n");
	ResultL->set_pattern("%v");
}

void Core::ResultReport::WriteMeshInformation(Mesh * mesh, string filename)
{
	ResultL->info("{:*^45}", filename);
	ResultL->info("Number of Nodes:\t\t\t{:12d}", mesh->NodeSize());
	ResultL->info("Number of Triangles:\t\t\t{:12d}", mesh->TriangleSize());
	ResultL->info("Number of Segments:\t\t\t{:12d}", mesh->SegmentSize());
	ResultL->info("Number of Cuboids:\t\t\t{:12d}", mesh->CuboidSize());
	ResultL->info("Number of Tetrahedras:\t\t\t{:12d}\n", mesh->TetrahedraSize());
}

void Core::ResultReport::WriteGeometeryInformation(Assist::ModalGeometry *geo)
{
	ResultL->info("{:*^45})", "Geometry Info");
	ResultL->info("Lower Point of Box is ({0:+5.3f},{1:+5.3f},{2:+5.3f})", 
		geo->GetLimitationBoundary(0).x(), 
		geo->GetLimitationBoundary(0).y(),
		geo->GetLimitationBoundary(0).z());
	ResultL->info("Upper Point of Box is ({0:+5.3f},{1:+5.3f},{2:+5.3f})", 
		geo->GetLimitationBoundary(7).x(),
		geo->GetLimitationBoundary(7).y(),
		geo->GetLimitationBoundary(7).z());
}

void Core::ResultReport::WriteBasicFunctionInformation(size_t unknowns)
{
	ResultL->info("{:*^45})", "Basic Function Info");
	ResultL->info("Type:\t\t\tRWG");
	ResultL->info("Number of Unknowns:\t\t\t{:12d}\n",unknowns);
}

void Core::ResultReport::WriteGreenFunctionInformation(GreenConfiguration * config)
{
	switch (config->GreenType)
	{
	case 1:
		throw spdlog::spdlog_ex("Other GreenFunction is not developing!");
	default:
		ResultL->info("Free Space Green Function is choosen\n");

	}
}

void Core::ResultReport::WriteExcitationInformation(Source::ExcitationConfiguration * config)
{
	ResultL->info("{:*^45}", "Excitation Info");
	if (config->sourceType == 0)ResultL->info("Source Type:\t\tPlanewave");
	ResultL->info("Direction:[{0:5.3f} {1:5.3f} {2:5.3f}]", 
		config->Ki.x(), config->Ki.y(), config->Ki.z());
	ResultL->info("Polarization:[{0:5.3f} {1:5.3f} {2:5.3f}]\n", 
		config->Ei.x(), config->Ei.y(), config->Ei.z());
}

void Core::ResultReport::WriteSolutionInformation(Solution::SolverConfiguration * config)
{
	ResultL->info("{:*^45}", "Solution Info");

	switch (config->Sol)
	{
	case Solution::LU: throw spd::spdlog_ex("LU solver is not developed");
	case Solution::BiCGStab: 
		ResultL->info("Solution Type:\tBiCGSTAB");break;
	default:
		throw spd::spdlog_ex("Solution Type is error");
	}

	switch (config->Precond)
	{
	case Solution::Identity:
		ResultL->info("Preconditioning Type:\t\tNo");
		break;
	case Solution::Jacobi:
		ResultL->info("Preconditioning Type:\t\tJacobi");
		break;
	case Solution::ILU:
		ResultL->info("Preconditioning Type:\t\tIncomplete LU");
		break;
	default:throw spd::spdlog_ex("Precondition is error");
	}
	ResultL->info("The Maxiteration is:\t\t{:d}", config->Maxiteration);
	ResultL->info("The Tolerance is:\t\t{:e}", config->Tolerance);
}

void Core::ResultReport::WriteMethodInformation(ImpConfiguration*config)
{
	switch (config->impType)
	{
	case AIM:
		ResultL->info("{:*^45}","AIM Parameters");
		ResultL->info("Order:\t{}", config->GridOrder);
		ResultL->info("Interval:\t{}", config->Interval);
		ResultL->info("Threshold:\t{}", config->Threshold);
		ResultL->info("NearTolerance:\t{}", config->NearCorrectionEps);
		ResultL->info("Dimension:\t{}", config->Dimension);
		ResultL->info("VirtualGrid:\t{}", config->VirtualGridTechnique);
		ResultL->info("TFS:\t{}", config->FillingStrategy);
		ResultL->info("AIMBox is from ({0:+5.3f},{1:+5.3f},{2:+5.3f}) to ({3:+5.3f},{4:+5.3f},{5:+5.3f})",
			config->Box[0].x(), config->Box[0].y(), config->Box[0].z(),
			config->Box[1].x(), config->Box[1].y(), config->Box[1].z());
		ResultL->info("LayerNumber:\t({0},{1},{2})\n", config->xNumber,config->yNumber, config->zNumber);
		return;
	case MUL:
		throw spd::spdlog_ex("Mul Algorithm is not Developing");
	default:
		ResultL->info("{:*^45}", "MoM");
	}
	
}

void Core::ResultReport::WriteRequestInformation(Request::FarFieldConfiguration * config)
{
	ResultL->info("RCS of Far Field::\t" + config->FarFileName);
}
