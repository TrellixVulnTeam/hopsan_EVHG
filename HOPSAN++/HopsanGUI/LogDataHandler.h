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
//! @file   LogDataHandler.h
//! @author Flumes <flumes@lists.iei.liu.se>
//! @date   2012-12-18
//!
//! @brief Contains the LogData classes
//!
//$Id$

#ifndef LOGDATAHANDLER_H
#define LOGDATAHANDLER_H

#include <QVector>
#include <QMap>
#include <QString>
#include <QColor>
#include <QObject>
#include <QDir>

#include "LogVariable.h"

// Forward Declaration
class PlotWindow;
class ContainerObject;

class LogDataHandler : public QObject
{
    Q_OBJECT

public:
    LogDataHandler(ContainerObject *pParent);
    ~LogDataHandler();

    typedef QMap< QString, QPointer<LogVariableContainer> > LogDataMapT;
    typedef QList<QVector<double> > TimeListT;
    typedef QList<VariableCommonDescription> FavoriteListT;

    void setParentContainerObject(ContainerObject *pParent);

    void collectLogDataFromModel(bool overWriteLastGeneration=false);
    void exportGenerationToPlo(const QString &rFilePath, const int gen, const int version=-1) const;
    void exportToPlo(const QString &rFilePath, const QStringList &rVariables, const int version=-1) const;
    void exportToPlo(const QString &rFilePath, const QVector<SharedLogVariableDataPtrT> &rVariables, int version=-1) const;
    void exportToCSV(const QString &rFilePath, const QVector<SharedLogVariableDataPtrT> &rVariables) const;
    void exportGenerationToCSV(const QString &rFilePath, const int gen) const;
    void importFromPlo(QString rImportFilePath=QString());
    void importFromCsv(QString rImportFilePath=QString());
    void importTimeVariablesFromCSVColumns(const QString csvFilePath, QVector<int> columns, QStringList names, const int timeColumnId=0);

    SharedLogVariableDataPtrT defineNewVariable(const QString desiredname);
    SharedLogVariableDataPtrT defineNewVariable(const QString desiredname, const QString &rUnit, const QString &rDescription);
    //SharedLogVariableDataPtrT defineNewVariable(const QString desiredname, QVector<double> x, QVector<double> y);
    SharedLogVariableDataPtrT defineTempVariable(QString desiredname);

    bool deleteVariable(SharedLogVariableDataPtrT a);
    bool deleteVariable(const QString &a);

    bool isEmpty();
    void clear();

    const SharedLogVariableDataPtrT getTimeVectorPtr(int generation) const;
    QVector<double> copyTimeVector(const int generation) const;
    QVector<double> copyLogDataVariableValues(int generation, QString componentName, QString portName, QString dataName); //!< @deprecated
    QVector<double> copyLogDataVariableValues(const QString &rName, const int generation);
    SharedLogVariableDataPtrT getLogVariableDataPtr(int generation, QString componentName, QString portName, QString dataName); //!< @deprecated
    SharedLogVariableDataPtrT getLogVariableDataPtr(const QString &rName, const int generation) const;
    QVector<SharedLogVariableDataPtrT> getMultipleLogVariableDataPtrs(const QRegExp &rNameExp, const int generation=-1) const;
    bool hasLogVariableData(const QString &rFullName, const int generation=-1);
    QVector<SharedLogVariableDataPtrT> getAllVariablesAtNewestGeneration();
    QVector<SharedLogVariableDataPtrT> getAllVariablesAtGeneration(const int generation) const;
    QStringList getLogDataVariableNames(const QString &rSeparator, const int generation=-1) const;
    QList<QString> getImportedVariablesFileNames() const;
    QList<SharedLogVariableDataPtrT> getImportedVariablesForFile(const QString &rFileName);

    void definePlotAlias(QString fullName);
    bool definePlotAlias(const QString alias, const QString fullName);
    void undefinePlotAlias(const QString &rAlias);

    QString getFullNameFromAlias(QString alias);
    QString getAliasFromFullName(QString fullName);

    QList<int> getGenerations() const;
    int getLowestGenerationNumber() const;
    int getHighestGenerationNumber() const;
    int getLatestGeneration() const; //!< @todo Name is a bit unclear, espicially compared to getHighestGenerationNumber()
    int getNumberOfGenerations() const;
    void limitPlotGenerations();
    void preventGenerationAutoRemoval(const int gen);
    void allowGenerationAutoRemoval(const int gen);
    void removeGeneration(const int gen);

    ContainerObject *getParentContainerObject();
    const QList<QDir> &getCacheDirs() const;
    SharedMultiDataVectorCacheT getOrCreateGenerationMultiCache(const int gen);

    void incrementOpenPlotCurves();
    void decrementOpenPlotCurves();
    bool hasOpenPlotCurves();
    void closePlotsWithCurvesBasedOnOwnedData();

    FavoriteListT getFavoriteVariableList();
    void setFavoriteVariable(QString componentName, QString portName, QString dataName, QString dataUnit);
    void removeFavoriteVariableByComponentName(QString componentName);

    QString plotVariable(const QString plotName, const QString fullVarName, const int gen, const int axis, QColor color=QColor());
    QString plotVariable(const QString plotName, const QString &rFullNameX, const QString &rFullNameY, const int gen, const int axis, QColor color=QColor());
    PlotWindow *plotVariable(PlotWindow *pPlotWindow, const QString fullVarName, const int gen, const int axis, QColor color=QColor());

    SharedLogVariableDataPtrT addVariableWithScalar(const SharedLogVariableDataPtrT a, const double x);
    SharedLogVariableDataPtrT subVariableWithScalar(const SharedLogVariableDataPtrT a, const double x);
    SharedLogVariableDataPtrT mulVariableWithScalar(const SharedLogVariableDataPtrT a, const double x);
    SharedLogVariableDataPtrT divVariableWithScalar(const SharedLogVariableDataPtrT a, const double x);
    QString addVariableWithScalar(const QString &a, const double x);
    QString subVariableWithScalar(const QString &a, const double x);
    QString mulVariableWithScalar(const QString &a, const double x);
    QString divVariableWithScalar(const QString &a, const double x);

    SharedLogVariableDataPtrT addVariables(const SharedLogVariableDataPtrT a, const SharedLogVariableDataPtrT b);
    SharedLogVariableDataPtrT subVariables(const SharedLogVariableDataPtrT a, const SharedLogVariableDataPtrT b);
    SharedLogVariableDataPtrT multVariables(const SharedLogVariableDataPtrT a, const SharedLogVariableDataPtrT b);
    SharedLogVariableDataPtrT divVariables(const SharedLogVariableDataPtrT a, const SharedLogVariableDataPtrT b);
    QString addVariables(const QString &a, const QString &b);
    QString subVariables(const QString &a, const QString &b);
    QString multVariables(const QString &a, const QString &b);
    QString divVariables(const QString &a, const QString &b);

    SharedLogVariableDataPtrT diffVariables(const SharedLogVariableDataPtrT a, const SharedLogVariableDataPtrT b);
    QString diffVariables(const QString &a, const QString &b);

    SharedLogVariableDataPtrT lowPassFilterVariable(const SharedLogVariableDataPtrT a, const SharedLogVariableDataPtrT b, const double freq);
    QString lowPassFilterVariable(const QString &a, const QString &b, const double freq);

    SharedLogVariableDataPtrT fftVariable(const SharedLogVariableDataPtrT a, const SharedLogVariableDataPtrT b, const bool doPowerSpectrum);
    QString fftVariable(const QString &a, const QString &b, const bool doPowerSpectrum);

    SharedLogVariableDataPtrT assignVariable(SharedLogVariableDataPtrT dst, const SharedLogVariableDataPtrT src);
    QString assignVariable(const QString &dst, const QString &src);
    QString assignVariable(const QString &dst, const QVector<double> &src);

    double pokeVariable(SharedLogVariableDataPtrT a, const int index, const double value);
    double pokeVariable(const QString &a, const int index, const double value);

    double peekVariable(SharedLogVariableDataPtrT a, const int b);
    double peekVariable(const QString &a, const int index);

    SharedLogVariableDataPtrT saveVariable(SharedLogVariableDataPtrT a);
    QString saveVariable(const QString &currName, const QString &newName);

    void appendVariable(const QString &a, const double x, const double y);

    void takeOwnershipOfData(LogDataHandler *pOtherHandler, int generation=-2);

public slots:
    void registerAlias(const QString &rFullName, const QString &rAlias);
    void unregisterAlias(const QString &rAlias);


signals:
    void newDataAvailable();
    void dataRemoved();
    void closePlotsWithOwnedData();

private slots:
    void forgetImportedLogDataVariable(SharedLogVariableDataPtrT pData);

private:
    typedef QMap< QString, QMap<QString,SharedLogVariableDataPtrT> > ImportedLogDataMapT;
    SharedLogVariableDataPtrT insertTimeVariable(QVector<double> &rTimeVector, VariableUniqueDescription *pVarUniqDesc);
    SharedLogVariableDataPtrT insertVariableBasedOnDescription(VariableCommonDescription &rVarComDesc, VariableUniqueDescription *pVarUniqDesc, SharedLogVariableDataPtrT pTimeVector, QVector<double> &rDataVector);
    QString getNewCacheName();
    void rememberIfImported(SharedLogVariableDataPtrT pData);

    ContainerObject *mpParentContainerObject;

    LogDataMapT mLogDataMap;
    ImportedLogDataMapT mImportedLogDataMap;

    FavoriteListT mFavoriteVariables;
    QMap<int, SharedMultiDataVectorCacheT> mGenerationCacheMap;

    int mnPlotCurves;
    int mGenerationNumber;
    quint64 mTempVarCtr;
    QList<QDir> mCacheDirs;
    quint64 mCacheSubDirCtr;
};

#endif // LOGDATAHANDLER_H
