#include  "stdafx.h"
#include "Const.h"

using namespace Core;

double  Core::Mur = 1.0;
double  Core::Epr = 1.0;
double  Core::Mu = Mu0 * Core::Mur;
double  Core::Epsilon = Epsilon0 * Core::Epr;
double  Core::Frequency = 3e8;
double  Core::Omega = 2 * M_PI*Core::Frequency;
double  Core::k = Core::Omega / c0;
double  Core::Lambda = c0 / Core::Frequency;
double  Core::eta = eta0 * Core::Mur / Core::Epr;
double  Core::alpha = 0.3;