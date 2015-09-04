/*-----------------------------------------------------------------------------
 This source file is a part of Hopsan

 Copyright (c) 2009 to present year, Hopsan Group

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

 For license details and information about the Hopsan Group see the files
 GPLv3 and HOPSANGROUP in the Hopsan source code root directory

 For author and contributor information see the AUTHORS file
-----------------------------------------------------------------------------*/

//!
//! @file   Ops.h
//! @author Robert Braun <robert.braun@liu.se>
//! @date   2015-08-31
//!
//! @brief Contains the optimization worker class for the Nelder-Mead algorithm
//!
//$Id: SymHop.cc 8138 2015-06-17 12:27:23Z petno25 $

#ifndef OPSWORKERNELDERMEAD_H
#define OPSWORKERNELDERMEAD_H

#include "OpsWorkerSimplex.h"

namespace Ops {

class WorkerNelderMead : public WorkerSimplex
{
    Q_OBJECT
public:
    WorkerNelderMead(Evaluator *pEvaluator);

    AlgorithmT getAlgorithm();

    void run();

    void setReflectionFactor(double value); //alpha
    void setExpansionFactor(double value);  //gamma
    void setContractionFactor(double value);//rho
    void setReductionFactor(double value); //sigma

private:
    void reduce();

    double mAlpha, mGamma, mRho, mSigma;
};

}

#endif // OPSWORKERNELDERMEAD_H
