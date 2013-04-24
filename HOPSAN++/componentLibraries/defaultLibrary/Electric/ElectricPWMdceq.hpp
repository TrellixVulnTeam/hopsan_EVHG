#ifndef ELECTRICPWMDCEQ_HPP_INCLUDED
#define ELECTRICPWMDCEQ_HPP_INCLUDED

#include <iostream>
#include "ComponentEssentials.h"
#include "ComponentUtilities.h"
#include "math.h"

//!
//! @file ElectricPWMdceq.hpp
//! @author Petter Krus <petter.krus@liu.se>
//! @date Wed 24 Apr 2013 20:39:49
//! @brief This is an imaginary DC analog component that represents an \
equivalent PWM controller.
//! @ingroup ElectricComponents
//!
//==This code has been autogenerated using Compgen==
//from 
/*{, C:, HopsanTrunk, HOPSAN++, CompgenModels}/ElectricComponents.nb*/

using namespace hopsan;

class ElectricPWMdceq : public ComponentQ
{
private:
     double resist0;
     double resist1;
     double umin;
     double umax;
     Port *mpPel1;
     Port *mpPel2;
     double delayParts1[9];
     double delayParts2[9];
     double delayParts3[9];
     double delayParts4[9];
     double delayParts5[9];
     Matrix jacobianMatrix;
     Vec systemEquations;
     Matrix delayedPart;
     int i;
     int iter;
     int mNoiter;
     double jsyseqnweight[4];
     int order[5];
     int mNstep;
     //Port Pel1 variable
     double uel1;
     double iel1;
     double cel1;
     double Zcel1;
     //Port Pel2 variable
     double uel2;
     double iel2;
     double cel2;
     double Zcel2;
//==This code has been autogenerated using Compgen==
     //inputVariables
     double tfac;
     //outputVariables
     double uel10;
     //Port Pel1 pointer
     double *mpND_uel1;
     double *mpND_iel1;
     double *mpND_cel1;
     double *mpND_Zcel1;
     //Port Pel2 pointer
     double *mpND_uel2;
     double *mpND_iel2;
     double *mpND_cel2;
     double *mpND_Zcel2;
     //Delay declarations
//==This code has been autogenerated using Compgen==
     //inputVariables pointers
     double *mptfac;
     //outputVariables pointers
     double *mpuel10;
     Delay mDelayedPart10;
     Delay mDelayedPart20;
     Delay mDelayedPart30;
     EquationSystemSolver *mpSolver;

public:
     static Component *Creator()
     {
        return new ElectricPWMdceq();
     }

     void configure()
     {
//==This code has been autogenerated using Compgen==

        mNstep=9;
        jacobianMatrix.create(5,5);
        systemEquations.create(5);
        delayedPart.create(6,6);
        mNoiter=2;
        jsyseqnweight[0]=1;
        jsyseqnweight[1]=0.67;
        jsyseqnweight[2]=0.5;
        jsyseqnweight[3]=0.5;


        //Add ports to the component
        mpPel1=addPowerPort("Pel1","NodeElectric");
        mpPel2=addPowerPort("Pel2","NodeElectric");
        //Add inputVariables to the component
            addInputVariable("tfac","Tc/Tp (time fraction \
on)","",1.,&mptfac);

        //Add outputVariables to the component
            addOutputVariable("uel10","middle voltage","V",0.,&mpuel10);

//==This code has been autogenerated using Compgen==
        //Add constants/parameters
            addConstant("resist0", "loss resistans (at 2)", "ohm", \
0.01,resist0);
            addConstant("resist1", "loss resistans (at 2)", "ohm", \
10.,resist1);
            addConstant("umin", "minimum voltage difference", "V", \
0.01,umin);
            addConstant("umax", "u max limit", "V", 1000.,umax);
        mpSolver = new EquationSystemSolver(this,5);
     }

    void initialize()
     {
        //Read port variable pointers from nodes
        //Port Pel1
        mpND_uel1=getSafeNodeDataPtr(mpPel1, NodeElectric::Voltage);
        mpND_iel1=getSafeNodeDataPtr(mpPel1, NodeElectric::Current);
        mpND_cel1=getSafeNodeDataPtr(mpPel1, NodeElectric::WaveVariable);
        mpND_Zcel1=getSafeNodeDataPtr(mpPel1, NodeElectric::CharImpedance);
        //Port Pel2
        mpND_uel2=getSafeNodeDataPtr(mpPel2, NodeElectric::Voltage);
        mpND_iel2=getSafeNodeDataPtr(mpPel2, NodeElectric::Current);
        mpND_cel2=getSafeNodeDataPtr(mpPel2, NodeElectric::WaveVariable);
        mpND_Zcel2=getSafeNodeDataPtr(mpPel2, NodeElectric::CharImpedance);

        //Read variables from nodes
        //Port Pel1
        uel1 = (*mpND_uel1);
        iel1 = (*mpND_iel1);
        cel1 = (*mpND_cel1);
        Zcel1 = (*mpND_Zcel1);
        //Port Pel2
        uel2 = (*mpND_uel2);
        iel2 = (*mpND_iel2);
        cel2 = (*mpND_cel2);
        Zcel2 = (*mpND_Zcel2);

        //Read inputVariables from nodes
        tfac = (*mptfac);

        //Read outputVariables from nodes
        uel10 = (*mpuel10);

//==This code has been autogenerated using Compgen==


        //Initialize delays

        delayedPart[1][1] = delayParts1[1];
        delayedPart[2][1] = delayParts2[1];
        delayedPart[3][1] = delayParts3[1];
        delayedPart[4][1] = delayParts4[1];
        delayedPart[5][1] = delayParts5[1];
     }
    void simulateOneTimestep()
     {
        Vec stateVar(5);
        Vec stateVark(5);
        Vec deltaStateVar(5);

        //Read variables from nodes
        //Port Pel1
        cel1 = (*mpND_cel1);
        Zcel1 = (*mpND_Zcel1);
        //Port Pel2
        cel2 = (*mpND_cel2);
        Zcel2 = (*mpND_Zcel2);

        //Read inputVariables from nodes
        tfac = (*mptfac);

        //LocalExpressions

        //Initializing variable vector for Newton-Raphson
        stateVark[0] = uel2;
        stateVark[1] = uel10;
        stateVark[2] = iel1;
        stateVark[3] = uel1;
        stateVark[4] = iel2;

        //Iterative solution using Newton-Rapshson
        for(iter=1;iter<=mNoiter;iter++)
        {
         //PWMdceq
         //Differential-algebraic system of equation parts

          //Assemble differential-algebraic equations
          systemEquations[0] =uel2 - uel10*limit(tfac,0.,1.);
          systemEquations[1] =-(iel1*resist1) - uel1 + uel10 + iel1*(-resist0 \
+ resist1)*limit(tfac,0.,1.);
          systemEquations[2] =iel1 + \
(iel2*limit(uel2,umin,umax))/limit(uel10,umin,umax);
          systemEquations[3] =-cel1 + uel1 - iel1*Zcel1;
          systemEquations[4] =iel2 + (cel2 - uel2)/Zcel2;

          //Jacobian matrix
          jacobianMatrix[0][0] = 1;
          jacobianMatrix[0][1] = -limit(tfac,0.,1.);
          jacobianMatrix[0][2] = 0;
          jacobianMatrix[0][3] = 0;
          jacobianMatrix[0][4] = 0;
          jacobianMatrix[1][0] = 0;
          jacobianMatrix[1][1] = 1;
          jacobianMatrix[1][2] = -resist1 + (-resist0 + \
resist1)*limit(tfac,0.,1.);
          jacobianMatrix[1][3] = -1;
          jacobianMatrix[1][4] = 0;
          jacobianMatrix[2][0] = \
(iel2*dxLimit(uel2,umin,umax))/limit(uel10,umin,umax);
          jacobianMatrix[2][1] = \
-((iel2*dxLimit(uel10,umin,umax)*limit(uel2,umin,umax))/Power(limit(uel10,umi\
n,umax),2));
          jacobianMatrix[2][2] = 1;
          jacobianMatrix[2][3] = 0;
          jacobianMatrix[2][4] = \
limit(uel2,umin,umax)/limit(uel10,umin,umax);
          jacobianMatrix[3][0] = 0;
          jacobianMatrix[3][1] = 0;
          jacobianMatrix[3][2] = -Zcel1;
          jacobianMatrix[3][3] = 1;
          jacobianMatrix[3][4] = 0;
          jacobianMatrix[4][0] = -(1/Zcel2);
          jacobianMatrix[4][1] = 0;
          jacobianMatrix[4][2] = 0;
          jacobianMatrix[4][3] = 0;
          jacobianMatrix[4][4] = 1;
//==This code has been autogenerated using Compgen==

          //Solving equation using LU-faktorisation
          mpSolver->solve(jacobianMatrix, systemEquations, stateVark, iter);
          uel2=stateVark[0];
          uel10=stateVark[1];
          iel1=stateVark[2];
          uel1=stateVark[3];
          iel2=stateVark[4];
        }

        //Calculate the delayed parts

        delayedPart[1][1] = delayParts1[1];
        delayedPart[2][1] = delayParts2[1];
        delayedPart[3][1] = delayParts3[1];
        delayedPart[4][1] = delayParts4[1];
        delayedPart[5][1] = delayParts5[1];

        //Write new values to nodes
        //Port Pel1
        (*mpND_uel1)=uel1;
        (*mpND_iel1)=iel1;
        //Port Pel2
        (*mpND_uel2)=uel2;
        (*mpND_iel2)=iel2;
        //outputVariables
        (*mpuel10)=uel10;

        //Update the delayed variabels

     }
    void deconfigure()
    {
        delete mpSolver;
    }
};
#endif // ELECTRICPWMDCEQ_HPP_INCLUDED
