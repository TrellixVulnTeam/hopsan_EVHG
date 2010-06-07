//!
//! @file   FirstOrderFilter.cc
//! @author Björn Eriksson <bjorn.eriksson@liu.se>
//! @date   2010-01-23
//!
//! @brief Contains the Core First Order Filter class
//!
//$Id$

#include <iostream>
#include <cassert>
#include <math.h>
#include "../HopsanCore.h"
#include "FirstOrderFilter.h"

//! @class FirstOrderFilter
//! @brief The FirstOrderFilter class implements a first order filter using bilinear transform
//!
//! To declare a filter like \f[G=\frac{a_1 s + a_0}{b_1 s + b_0}\f]
//! the syntax is myFilter.setNumDen(num, den)
//! where \f$num=\{a_1, a_0\}\f$
//! and \f$den=\{b_1, b_0\}\f$
//!

FirstOrderFilter::FirstOrderFilter()
{
    mLastTime = -1.0;
    mIsInitialized = false;
}


void FirstOrderFilter::initialize(double &rTime, double timestep, double num[2], double den[2], double u0, double y0, double min, double max)
{
    mMin = min;
    mMax = max;
    mValue = y0;
    mDelayU.setStepDelay(1);
    mDelayY.setStepDelay(1);
    mDelayU.initialize(rTime, u0);
    mDelayY.initialize(rTime, std::max(std::min(y0, mMax), mMin));

    mTimeStep = timestep;
    mpTime = &rTime;
    mIsInitialized = true;
    mLastTime = -mTimeStep;

    setNumDen(num, den);
}


void FirstOrderFilter::setMinMax(double min, double max)
{
    mMin = min;
    mMax = max;
}


void FirstOrderFilter::setNumDen(double num[2], double den[2])
{
//num =
//(T + T*q)*(2*a + T*b - 2*a*q + T*b*q)
//den =
//(T + T*q)*(2*A - 2*A*q + B*T + B*T*q)

    mCoeffU[0] = num[1]*mTimeStep-2.0*num[0];
    mCoeffU[1] = num[1]*mTimeStep+2.0*num[0];

    mCoeffY[0] = den[1]*mTimeStep-2.0*den[0];
    mCoeffY[1] = den[1]*mTimeStep+2.0*den[0];
}


void FirstOrderFilter::initializeValues(double u0, double y0)
{
    mDelayU.initializeValues(u0);
    mDelayY.initializeValues(y0);
    mValue = y0;
}


void FirstOrderFilter::update(double u)
{
    if (!mIsInitialized)
    {
        std::cout << "Integrator function has to be initialized" << std::endl;
        assert(false);
    }
    else if (mLastTime != *mpTime)
    {
        //Filter equation
        //Bilinear transform is used

        mValue = 1.0/mCoeffY[1]*(mCoeffU[1]*u + mCoeffU[0]*mDelayU.value(u) - mCoeffY[0]*mDelayY.value());
//cout << "FILTER: " << "  u: " << u << "  y: " << mValue << endl;

        if (mValue > mMax)
        {
            mDelayY.initializeValues(mMax);
            mDelayU.initializeValues(mMax);
            mValue = mMax;
        }
        else if (mValue < mMin)
        {
            mDelayY.initializeValues(mMin);
            mDelayU.initializeValues(mMin);
            mValue = mMin;
        }
        else
        {
            mDelayY.update(mValue);
            mDelayU.update(u);
        }

        mLastTime = *mpTime;
    }
}


double FirstOrderFilter::value(double u)
{
    update(u);

    return mValue;
}


double FirstOrderFilter::value()
//! Observe that a call to this method has to be followed by another call to value(double u) or to update(double u)
//! @return The filtered actual value.
//! @see value(double u)
{
    update(mDelayU.valueIdx(1));

    return mValue;
}
