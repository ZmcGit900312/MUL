#ifndef RSEULTREPORT_H
#define RSEULTREPORT_H

#include "Mesh.h"
#include "ModalGeometry.h"
#include "Log.h"
#include "IImpedance.h"
#include "Green.h"
#include "RequestConfiguration.h"
#include "ISolver.h"
#include "Excitation.h"
namespace Core
{

	struct ResultReport
	{

		//double time[12] = { 0.0 };

		static void InitialWriter(string filename);
		static void WriteMeshInformation(Mesh*mesh,string filename, shared_ptr<spd::logger>log);
		static void WriteGeometeryInformation(Assist::ModalGeometry *geo, shared_ptr<spd::logger>log);
		static void WriteBasicFunctionInformation(size_t unknowns, shared_ptr<spd::logger>log);
		static void WriteGreenFunctionInformation(GreenConfiguration *config, shared_ptr<spd::logger>log);
		static void WriteExcitationInformation(Source::ISource*config, shared_ptr<spd::logger>log);
		static void WriteSolutionInformation(Solution::SolverConfiguration*config,shared_ptr<spd::logger>log);
		static void WriteMethodInformation(ImpConfiguration* config, shared_ptr<spd::logger>log);
		static void WriteRequestInformation(Request::FarFieldConfiguration*config, shared_ptr<spd::logger>log);
		//void SummaryTime();

	};
	
}



#endif