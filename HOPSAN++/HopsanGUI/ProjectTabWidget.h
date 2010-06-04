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
 * HopsanGUI
 * Fluid and Mechatronic Systems, Department of Management and Engineering, Linköping University
 * Main Authors 2009-2010:  Robert Braun, Björn Eriksson, Peter Nordin
 * Contributors 2009-2010:  Mikael Axin, Alessandro Dell'Amico, Karl Pettersson, Ingo Staack
 */

//!
//! @file   ProjectTabWidget.h
//! @author Björn Eriksson <bjorn.eriksson@liu.se>
//! @date   2010-02-05
//!
//! @brief Contain classes for Project Tabs
//!
//$Id$

#ifndef PROJECTTABWIDGET_H
#define PROJECTTABWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTabWidget>
#include <map>

#include "GUIRootSystem.h"

#include "MessageWidget.h"
#include "AppearanceData.h"
#include "UndoStack.h"


class GUIPort;
class GUIConnector;
class ProjectTab;
class UndoStack;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GraphicsScene(ProjectTab *parent = 0);
    qreal TestVar;

    ProjectTab *mpParentProjectTab;
};


class HopsanEssentials; //Forward declaration
class ComponentSystem;
class GUIComponent;
class GUIObject;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    GraphicsView(ProjectTab *parent = 0);
    //~GraphicsView();
    bool mIsCreatingConnector;
    bool mPortsHidden;
    GUIObject *getGUIObject(QString name);
    GUIConnector *getTempConnector();

    ProjectTab *mpParentProjectTab;
    QMap<QString, GUIObject *> mGUIObjectMap;
    QVector<GUIConnector *> mConnectorVector;
    QAction *systemPortAction;
    QMenu *menuInsert;
    QColor mBackgroundColor;
    //QPen getPen(QString situation, QString type, QString style);
    UndoStack *undoStack;
    qreal mZoomFactor;

signals:
    void keyPressDelete();
    void keyPressR();
    void keyPressShiftK();
    void keyPressShiftL();
    void keyPressCtrlUp();
    void keyPressCtrlDown();
    void keyPressCtrlLeft();
    void keyPressCtrlRight();
    void viewClicked();
    void checkMessages();
    void systemPortSignal(QPoint position);
    void zoomChange();

public slots:
    void addGUIObject(QString componentTypeName, AppearanceData appearanceData, QPoint position, qreal rotation = 0, QString name=QString(), bool startSelected=true, bool doNotRegisterUndo = false);
    void deleteGUIObject(QString componentName);
    bool haveGUIObject(QString name);
    void renameGUIObject(QString oldName, QString newName);
    void addSystemPort();
    void addConnector(GUIPort *pPort, bool doNotRegisterUndo = false);
    void removeConnector(GUIConnector* pConnector, bool doNotRegisterUndo = false);
    void cutSelected();
    void copySelected();
    void paste();
    void selectAll();
    void setScale(const QString &scale);
    void resetZoom();
    void zoomIn();
    void zoomOut();
    void hideNames();
    void showNames();
    void hidePorts(bool doIt);
    void undo();
    void redo();
    void clearUndo();

protected:
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dropEvent(QDropEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

    void contextMenuEvent ( QContextMenuEvent * event );

private:
    GUIObject *mpTempGUIObject;
    GUIConnector *mpTempConnector;
    QString *mpCopyData;
    //! @todo QMap no good means problem if we rename need to loop around the rename like in coore
    bool isObjectSelected();
    bool isConnectorSelected();
    void createActions();
    void createMenus();
    void addSystemPort(QPoint position, QString name=QString(), bool startSelected = false);
    bool mJustStoppedCreatingConnector;
    bool mCtrlKeyPressed;
};


class ProjectTabWidget; //Forward declaration

class ProjectTab : public QWidget
{
    Q_OBJECT

public:
    ProjectTab(ProjectTabWidget *parent = 0);
    bool mIsSaved;
    QString mModelFileName;
    bool useIsoGraphics;
    ProjectTabWidget *mpParentProjectTabWidget;
    GUIRootSystem mGUIRootSystem;
    GraphicsView *mpGraphicsView;
    GraphicsScene *mpGraphicsScene;
    double getStartTime();
    double getTimeStep();
    double getStopTime();

public slots:
    void hasChanged();
    void updateStartTime();
    void updateTimeStep();
    void updateStopTime();

signals:
    void checkMessages();

private:
    double mStartTime;
    double mStopTime;
    double mTimeStep;
};


class MainWindow;

class ProjectTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    ProjectTabWidget(MainWindow *parent = 0);

    ProjectTab *getCurrentTab();
    MainWindow *mpParentMainWindow;
    size_t mNumberOfUntitledTabs;

public slots:
    void addProjectTab(ProjectTab *projectTab, QString tabName="Untitled");
    void addNewProjectTab(QString tabName="Untitled");
    void saveProjectTab();
    void saveProjectTabAs();
    void saveProjectTab(int index, bool saveAs);
    bool closeProjectTab(int index);
    bool closeAllProjectTabs();
    void simulateCurrent();
    void loadModel();
    void saveModel(bool saveAs);
    void setIsoGraphics(bool value);
    void resetZoom();
    void zoomIn();
    void zoomOut();
    void hideNames();
    void showNames();
    void updateSimulationSetupWidget();
    void updateCurrentStartTime();
    void updateCurrentTimeStep();
    void updateCurrentStopTime();
    void hidePortsInCurrentTab(bool doIt);

signals:
    void checkMessages();

};

#endif // PROJECTTABWIDGET_H
