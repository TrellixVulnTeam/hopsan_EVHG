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
//! @file   GUIUtilities.h
//! @author Flumes <flumes@lists.iei.liu.se>
//! @date   2010-10-09
//!
//! @brief Contains a class for misc utilities
//!
//$Id$


#ifndef GUIUTILITIES_H
#define GUIUTILITIES_H

#include <QPointF>
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>
#include <complex>
#include <QDomElement>

QString readName(QTextStream &rTextStream);
QString readName(QString namestring);
QString addQuotes(QString str);
QString relativePath(QFileInfo pathtochange, QDir basedir);
qreal deg2rad(const qreal deg);
qreal rad2deg(const qreal rad);
qreal normDeg180(const qreal deg);
qreal normDeg360(const qreal deg);
qreal normRad(const qreal rad);
qreal dist(const QPointF &rPoint);
qreal dist(const QPointF &rPoint1, const QPointF &rPoint2);
double dist(double x1,double y1, double x2, double y2);
bool fuzzyEqual(const qreal first, const qreal second, const qreal eps);
bool fuzzyLT(const qreal first, const qreal second, const qreal eps);
void replaceWord(QString &string, QString before, QString after);
QString parseVariableDescription(QString input);
QString parseVariableUnit(QString input);
QVector< std::complex<double> > realToComplex(QVector<double> realVector);
void FFT(QVector< std::complex<double> > &data);
void reduceVectorSize(QVector<double> &vector, int newSize);
void removeDir(QString path);
void copyIncludeFilesToDir(QString path);
double normalDistribution(double average, double sigma);

//Component generation
class ComponentDescription
{
public:
    ComponentDescription(QString typeName, QString displayName, QString cqsType);
    QString typeName;
    QString displayName;
    QString cqsType;
    QStringList utilities;
    QStringList utilityNames;
    QStringList parNames;
    QStringList parInits;
    QStringList parDisplayNames;
    QStringList parDescriptions;
    QStringList parUnits;
    QStringList varNames;
    QStringList varTypes;
    QStringList portNames;
    QStringList portTypes;
    QStringList portNodeTypes;
    QStringList portDefaults;
    QList<bool> portNotReq;
    QStringList initEquations;
    QStringList simEquations;
    QStringList finalEquations;
};
void generateComponentSourceCode(QString outputFile, QDomElement &rDomElement);
void generateComponentSourceCode(QString typeName, QString displayName, QString cqsType, QStringList sysEquations, QStringList stateVars, QStringList jacobian);
void generateComponentSourceCode(QString outputFile, ComponentDescription comp, bool overwriteStartValues=false);
void identifyVariables(QString equation, QStringList &leftSideVariables, QStringList &righrSideVariables);

//Optimization
void reflectWorst(QVector< QVector<double> > &vector, int worst, double alpha=1.3);
double sum(QVector< QVector<double> > vector, int i);

#endif // GUIUTILITIES_H
