/*
 * This file is part of OpenModelica.
 *
 * Copyright (c) 1998-CurrentYear, Linköping University,
 * Department of Computer and Information Science,
 * SE-58183 Linköping, Sweden.
 *
 * All rights reserved.
 *
 * THIS PROGRAM IS PROVIDED UNDER THE TERMS OF GPL VERSION 3 
 * AND THIS OSMC PUBLIC LICENSE (OSMC-PL). 
 * ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS PROGRAM CONSTITUTES RECIPIENT'S  
 * ACCEPTANCE OF THE OSMC PUBLIC LICENSE.
 *
 * The OpenModelica software and the Open Source Modelica
 * Consortium (OSMC) Public License (OSMC-PL) are obtained
 * from Linköping University, either from the above address,
 * from the URLs: http://www.ida.liu.se/projects/OpenModelica or  
 * http://www.openmodelica.org, and in the OpenModelica distribution. 
 * GNU version 3 is obtained from: http://www.gnu.org/copyleft/gpl.html.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without
 * even the implied warranty of  MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE, EXCEPT AS EXPRESSLY SET FORTH
 * IN THE BY RECIPIENT SELECTED SUBSIDIARY LICENSE CONDITIONS
 * OF OSMC-PL.
 *
 * See the full OSMC Public License conditions for more details.
 *
 */

/*
 * Main Authors 2009-2010:  Robert Braun, Björn Eriksson, Peter Nordin
 * Contributors 2009-2010:  Mikael Axin, Alessandro Dell'Amico, Karl Pettersson, Ingo Staack
 * Fluid and Mechatronic Systems, Department of Management and Engineering, Linköping University
 * Based on: HopsanGUI
 */

#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QPlainTextEdit>
#include "HopsanCore.h"


class MainWindow;

class MessageWidget : public QTextEdit
{
    Q_OBJECT
private:
    //*****Core Interaction*****
    HopsanEssentials *mpHopsanCore;
    //**************************
    void setMessageColor(int type);

public:
    MessageWidget(MainWindow *pParent=0);
    void printCoreMessages();
    void printGUIMessage(QString message);
    void printGUIErrorMessage(QString message);
    void printGUIWarningMessage(QString message);
    void printGUIInfoMessage(QString message);
    QSize sizeHint() const;

    MainWindow *mpParentMainWindow;

public slots:
    void checkMessages();
};

#endif // MESSAGEWIDGET_H
