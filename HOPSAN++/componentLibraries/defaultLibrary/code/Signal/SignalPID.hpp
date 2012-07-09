#ifndef SIGNALPID_HPP_INCLUDED
#define SIGNALPID_HPP_INCLUDED

#include <iostream>
#include "ComponentEssentials.h"
#include "ComponentUtilities.h"
#include "math.h"

//!
//! @file SignalPID.hpp
//! @author Petter Krus <petter.krus@liu.se>
//! @date Sun 8 Jul 2012 20:55:26
//! @brief PID controller
//! @ingroup SignalComponents
//!
//This component is generated by COMPGEN for HOPSAN-NG simulation 
//from 
/*{, C:, Documents and Settings, petkr14, My Documents, \
CompgenNG}/SignalControlAeroNG.nb*/

using namespace hopsan;

class SignalPID : public ComponentSignal
{
private:
     double mKp;
     double mKI;
     double mKd;
     double mumin;
     double mumax;
     double myref;
     double my;
     double mdy;
     Port *mpPyref;
     Port *mpPy;
     Port *mpPdy;
     Port *mpPu;
     double delayParts1[9];
     double delayParts2[9];
     Matrix jacobianMatrix;
     Vec systemEquations;
     Matrix delayedPart;
     int i;
     int iter;
     int mNoiter;
     double jsyseqnweight[4];
     int order[1];
     int mNstep;
     //inputVariables
     double yref;
     double y;
     double dy;
     //outputVariables
     double u;
     //Delay declarations
     //inputVariables pointers
     double *mpND_yref;
     double *mpND_y;
     double *mpND_dy;
     //outputVariables pointers
     double *mpND_u;
     Delay mDelayedPart10;
     Delay mDelayedPart11;
     EquationSystemSolver *mpSolver;

public:
     static Component *Creator()
     {
        return new SignalPID();
     }

     SignalPID() : ComponentSignal()
     {
        const double Kp = 1.;
        const double KI = 1.;
        const double Kd = 1.;
        const double umin = -1.;
        const double umax = 1.;
        const double yref = 0.;
        const double y = 0.;
        const double dy = 0.;

        mNstep=9;
        jacobianMatrix.create(1,1);
        systemEquations.create(1);
        delayedPart.create(2,6);
        mNoiter=2;
        jsyseqnweight[0]=1;
        jsyseqnweight[1]=0.67;
        jsyseqnweight[2]=0.5;
        jsyseqnweight[3]=0.5;

        mKp = Kp;
        mKI = KI;
        mKd = Kd;
        mumin = umin;
        mumax = umax;
        myref = yref;
        my = y;
        mdy = dy;

        //Add ports to the component

        //Add inputVariables ports to the component
        mpPyref=addReadPort("Pyref","NodeSignal", Port::NOTREQUIRED);
        mpPy=addReadPort("Py","NodeSignal", Port::NOTREQUIRED);
        mpPdy=addReadPort("Pdy","NodeSignal", Port::NOTREQUIRED);

        //Add outputVariables ports to the component
        mpPu=addWritePort("Pu","NodeSignal", Port::NOTREQUIRED);

        //Register changable parameters to the HOPSAN++ core
        registerParameter("Kp", "Proportional gain", "", mKp);
        registerParameter("KI", "Integral gain", "", mKI);
        registerParameter("Kd", "Differential gain", "", mKd);
        registerParameter("umin", "Minium output signal", "", mumin);
        registerParameter("umax", "Maximum output signal", "", mumax);
        registerParameter("yref", "Reference value", "", myref);
        registerParameter("y", "Actual value", "", my);
        registerParameter("dy", "Differential of actual value", "", mdy);
        mpSolver = new EquationSystemSolver(this,1);
     }

    void initialize()
     {
        //Read port variable pointers from nodes
        //Read inputVariables pointers from nodes
        mpND_yref=getSafeNodeDataPtr(mpPyref, NodeSignal::VALUE,myref);
        mpND_y=getSafeNodeDataPtr(mpPy, NodeSignal::VALUE,my);
        mpND_dy=getSafeNodeDataPtr(mpPdy, NodeSignal::VALUE,mdy);
        //Read outputVariable pointers from nodes
        mpND_u=getSafeNodeDataPtr(mpPu, NodeSignal::VALUE);

        //Read variables from nodes

        //Read inputVariables from nodes
        yref = (*mpND_yref);
        y = (*mpND_y);
        dy = (*mpND_dy);

        //Read outputVariables from nodes
        u = mpPu->getStartValue(NodeSignal::VALUE);



        //Initialize delays
        delayParts1[1] = (-2*dy*mKd - 2*u - 2*mKp*y + mKI*mTimestep*y + \
2*mKp*yref - mKI*mTimestep*yref)/2.;
        mDelayedPart11.initialize(mNstep,delayParts1[1]);

        delayedPart[1][1] = delayParts1[1];
     }
    void simulateOneTimestep()
     {
        Vec stateVar(1);
        Vec stateVark(1);
        Vec deltaStateVar(1);

        //Read variables from nodes

        //Read inputVariables from nodes
        yref = (*mpND_yref);
        y = (*mpND_y);
        dy = (*mpND_dy);

        //LocalExpressions

        //Initializing variable vector for Newton-Raphson
        stateVark[0] = u;

        //Iterative solution using Newton-Rapshson
        for(iter=1;iter<=mNoiter;iter++)
        {
         //PID
         //Differential-algebraic system of equation parts

          //Assemble differential-algebraic equations
          systemEquations[0] =dy*mKd + u + ((2*mKp + mKI*mTimestep)*(y - \
yref))/2. + delayedPart[1][1];

          //Jacobian matrix
          jacobianMatrix[0][0] = 1;

          //Solving equation using LU-faktorisation
          mpSolver->solve(jacobianMatrix, systemEquations, stateVark, iter);
          u=stateVark[0];
        }

        //Calculate the delayed parts
        delayParts1[1] = (-2*dy*mKd - 2*u - 2*mKp*y + mKI*mTimestep*y + \
2*mKp*yref - mKI*mTimestep*yref)/2.;

        delayedPart[1][1] = delayParts1[1];

        //Write new values to nodes
        //outputVariables
        (*mpND_u)=u;

        //Update the delayed variabels
        mDelayedPart11.update(delayParts1[1]);

     }
};
#endif // SIGNALPID_HPP_INCLUDED
