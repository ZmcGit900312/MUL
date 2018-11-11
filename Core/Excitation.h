#pragma once
#include <Eigen/Core>
#include <map>

using namespace std;
using namespace Eigen;

namespace Core
{
	namespace Internal
	{
		class Excitation
		{
		typedef double azimuth;
		typedef double elevation;
		typedef double amplitutde;
		typedef double phase;
		typedef tuple<azimuth, elevation, amplitutde, phase> T4;

		public:
		static T4 PlaneWave;
		static map<Index, T4> Radiation;
		static void SetExcitation(VectorXcd& righthand);
		Excitation() {};
		~Excitation() {};
		};
		
	}
}


