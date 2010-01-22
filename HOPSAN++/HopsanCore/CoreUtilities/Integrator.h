//!
//! @file   Integrator.h
//! @author Björn Eriksson <bjorn.eriksson@liu.se>
//! @date   2010-01-22
//!
//! @brief Contains the Core Utility Integrator class
//!
//$Id$

#ifndef INTEGRATOR_H_INCLUDED
#define INTEGRATOR_H_INCLUDED

#include <deque>
#include "win32dll.h"

class DLLIMPORTEXPORT Integrator
{
public:
    Integrator();
    void initializeValues(double u0, double y0, double timestep, double &rTime);
    void update(double u, double y);
	double value(double u, double y);

private:
    Delay mDelayU, mDelayY;
    double mTimeStep;
    double *mpTime;
    double mLastTime;
	bool mIsInitialized;
};

#endif // INTEGRATOR_H_INCLUDED
