/*-----------------------------------------------------------------------------
 This source file is part of Hopsan NG

 Copyright (c) 2011 
    Mikael Axin, Robert Braun, Alessandro Dell'Amico, Björn Eriksson,
    Peter Nordin, Karl Pettersson, Petter Krus, Ingo Staack

 This file is provided "as is", with no guarantee or warranty for the
 functionality or reliability of the contents. All contents in this file is
 the original work of the copyright holders at the Division of Fluid and
 Mechatronic Systems (Flumes) at Linköping University. Modifying, using or
 redistributing any part of this file is prohibited without explicit
 permission from the copyright holders.
-----------------------------------------------------------------------------*/

//!
//! @file   HydraulicFixedDisplacementPump.hpp
//! @author Robert Braun <robert.braun@liu.se>
//! @date   2010-07-01
//!
//! @brief Contains a variable displacement hydraulic motor component with inertia load
//!
//$Id$

#ifndef HYDRAULICVARIABLEDISPLACEMENTMOTORQ_H
#define HYDRAULICVARIABLEDISPLACEMENTMOTORQ_H


#include <iostream>
#include "ComponentEssentials.h"
#include "ComponentUtilities.h"

namespace hopsan {

    //!
    //! @brief
    //! @ingroup HydraulicComponents
    //!
    class HydraulicVariableDisplacementMotorQ : public ComponentQ
    {
    private:
        double dp, Bm, cim, J, eps;

        double *mpND_p1, *mpND_q1, *mpND_c1, *mpND_Zc1, *mpND_p2, *mpND_q2, *mpND_c2, *mpND_Zc2, *mpND_t3, *mpND_a3, *mpND_w3, *mpND_c3, *mpND_Zx3, *mpND_eps;

        DoubleIntegratorWithDamping mIntegrator;
        Port *mpP1, *mpP2, *mpP3, *mpIn;

    public:
        static Component *Creator()
        {
            return new HydraulicVariableDisplacementMotorQ();
        }

        HydraulicVariableDisplacementMotorQ() : ComponentQ()
        {
            dp = 0.00005;
            Bm = 0;
            cim = 0;
            J = 0.1;
            eps = 1;

            mpP1 = addPowerPort("P1", "NodeHydraulic");
            mpP2 = addPowerPort("P2", "NodeHydraulic");
            mpP3 = addPowerPort("P3", "NodeMechanicRotational");
            mpIn = addReadPort("in", "NodeSignal", Port::NOTREQUIRED);

            registerParameter("D_m", "Displacement", "[m^3/rev]", dp);
            registerParameter("B_m", "Viscous Friction", "[Nms/rad]", Bm);
            registerParameter("C_i,m", "Leakage Coefficient", "[]", cim);
            registerParameter("J_m", "Inerteia Load", "[kgm^2]", J);
            registerParameter("epsilon_m", "Displacement Position", "[-]", eps);
        }


        void initialize()
        {
            mpND_eps = getSafeNodeDataPtr(mpIn, NodeSignal::VALUE, eps);

            mpND_p1 = getSafeNodeDataPtr(mpP1, NodeHydraulic::PRESSURE);
            mpND_q1 = getSafeNodeDataPtr(mpP1, NodeHydraulic::FLOW);
            mpND_c1 = getSafeNodeDataPtr(mpP1, NodeHydraulic::WAVEVARIABLE);
            mpND_Zc1 = getSafeNodeDataPtr(mpP1, NodeHydraulic::CHARIMP);

            mpND_p2 = getSafeNodeDataPtr(mpP2, NodeHydraulic::PRESSURE);
            mpND_q2 = getSafeNodeDataPtr(mpP2, NodeHydraulic::FLOW);
            mpND_c2 = getSafeNodeDataPtr(mpP2, NodeHydraulic::WAVEVARIABLE);
            mpND_Zc2 = getSafeNodeDataPtr(mpP2, NodeHydraulic::CHARIMP);

            mpND_t3 = getSafeNodeDataPtr(mpP3, NodeMechanicRotational::TORQUE);
            mpND_a3 = getSafeNodeDataPtr(mpP3, NodeMechanicRotational::ANGLE);
            mpND_w3 = getSafeNodeDataPtr(mpP3, NodeMechanicRotational::ANGULARVELOCITY);
            mpND_c3 = getSafeNodeDataPtr(mpP3, NodeMechanicRotational::WAVEVARIABLE);
            mpND_Zx3 = getSafeNodeDataPtr(mpP3, NodeMechanicRotational::CHARIMP);

            mIntegrator.initialize(mTimestep, 0, 0, 0, 0);
        }


        void simulateOneTimestep()
        {
            //Declare local variables
            double p1, q1, c1, Zc1, p2, q2, c2, Zc2, t3, a3, w3, c3, Zx3;
            double dpe, ble, gamma, c1a, c2a, ct, q1a, q2a, q1leak, q2leak;

            //Get variable values from nodes
            c1 = (*mpND_c1);
            Zc1 = (*mpND_Zc1);
            c2 = (*mpND_c2);
            Zc2 = (*mpND_Zc2);
            c3 = (*mpND_c3);
            Zx3 = (*mpND_Zx3);
            eps = (*mpND_eps);

            //Motor equations
            limitValue(eps, -1, 1);

            dpe = dp / (3.1415 * 2) * eps;
            ble = Bm + Zc1 * dpe*dpe + Zc2 * dpe*dpe + Zx3;
            gamma = 1 / (cim * (Zc1 + Zc2) + 1);
            c1a = (cim * Zc2 + 1) * gamma * c1 + cim * gamma * Zc1 * c2;
            c2a = (cim * Zc1 + 1) * gamma * c2 + cim * gamma * Zc2 * c1;
            ct = c1a * dpe - c2a * dpe - c3;
            mIntegrator.setDamping(ble / J * mTimestep);
            mIntegrator.integrateWithUndo(ct/J);
            w3 = mIntegrator.valueFirst();
            a3 = mIntegrator.valueSecond();

            //Ideal Flow
            q1a = -dpe * w3;
            q2a = -q1a;
            p1 = c1a + gamma * Zc1 * q1a;
            p2 = c2a + gamma * Zc2 * q2a;

            //Cavitation Check
            bool cav=false;
            if (p1 < 0.0)
            {
                c1 = 0.0;
                Zc1 = 0;
                cav = true;
            }
            if (p2 < 0.0)
            {
                c2 = 0.0;
                Zc2 = 0;
                cav = true;
            }
            if(cav)
            {
                ble = Bm + Zc1 * dpe*dpe + Zc2 * dpe*dpe + Zx3;
                gamma = 1 / (cim * (Zc1 + Zc2) + 1);
                c1a = (cim * Zc2 + 1) * gamma * c1 + cim * gamma * Zc1 * c2;
                c2a = (cim * Zc1 + 1) * gamma * c2 + cim * gamma * Zc2 * c1;
                ct = c1a * dpe - c2a * dpe - c3;
                mIntegrator.setDamping(ble / J * mTimestep);
                mIntegrator.redoIntegrate(ct/J);
                w3 = mIntegrator.valueFirst();
                a3 = mIntegrator.valueSecond();

                q1a = -dpe * w3;
                p1 = c1a + gamma * Zc1 * q1a;
                p2 = c2a + gamma * Zc2 * q2a;

                if(p1<=0)
                {
                    p1 = 0;
                    q1a = std::max(q1a, 0.0);
                    w3 = std::min(w3, 0.0);
                }
                if(p2<=0)
                {
                    p2 = 0;
                    q1a = std::min(q1a, 0.0);
                    w3 = std::max(w3, 0.0);
                }
                if(p1>0 && p2>0)
                {
                    a3 = mIntegrator.valueSecond();     //Only change a3 if w3 was not limited due to cavitation
                }
                mIntegrator.initializeValues(ct/J, a3, w3);
                q2a = -q1a;
            }

            //Leakage Flow
            q1leak = -cim * (p1 - p2);
            q2leak = -q1leak;

            //Effective Flow
            q1 = q1a + q1leak;
            q2 = q2a + q2leak;

            t3 = c3 + a3 * Zx3;

            //Write new values to nodes
            (*mpND_p1) = p1;
            (*mpND_q1) = q1;
            (*mpND_p2) = p2;
            (*mpND_q2) = q2;
            (*mpND_t3) = t3;
            (*mpND_a3) = a3;
            (*mpND_w3) = w3;
        }
    };
}

#endif // HYDRAULICVARIABLEDISPLACEMENTMOTORQ_H
