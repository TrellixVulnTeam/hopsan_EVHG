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

//$Id: OptionsWidget.cpp 1196 2010-04-01 09:55:04Z robbr48 $


#include <QtGui>
#include "OptionsWidget.h"
#include <QDebug>
#include "ProjectTabWidget.h"

class ProjectTabWidget;

OptionsWidget::OptionsWidget(MainWindow *parent)
    : QDialog(parent)
{
    this->mpParentMainWindow = parent;
    //Set the name and size of the main window
    this->setObjectName("OptionsWidget");
    this->resize(640,480);
    this->setWindowTitle("Options");


    cancelButton = new QPushButton(tr("&Cancel"));
    cancelButton->setAutoDefault(false);
    okButton = new QPushButton(tr("&Done"));
    okButton->setAutoDefault(true);

    buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(cancelButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(okButton, QDialogButtonBox::ActionRole);

    connect(cancelButton, SIGNAL(pressed()), this, SLOT(reject()));
    connect(okButton, SIGNAL(pressed()), this, SLOT(updateValues()));


    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    //mainLayout->addLayout(topLeftLayout, 0, 0);
    mainLayout->addWidget(buttonBox, 1, 0);
    //mainLayout->addWidget(extension, 1, 0, 1, 2);
    setLayout(mainLayout);
}

OptionsWidget::~OptionsWidget()
{
}



void OptionsWidget::updateValues()
{
    this->accept();
}
