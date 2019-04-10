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
		static void WriteMeshInformation(Mesh*mesh,string filename);
		static void WriteGeometeryInformation(Assist::ModalGeometry *geo);
		static void WriteBasicFunctionInformation(size_t unknowns);
		static void WriteGreenFunctionInformation(GreenConfiguration *config);
		static void WriteExcitationInformation(Source::ExcitationConfiguration*config);
		static void WriteSolutionInformation(Solution::SolverConfiguration*config);
		static void WriteMethodInformation(ImpConfiguration* config);
		static void WriteRequestInformation(Request::FarFieldConfiguration*config);
		//void SummaryTime();

	};
	
}



#endif