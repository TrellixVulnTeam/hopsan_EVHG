#ifndef DATAEXPLORER_H
#define DATAEXPLORER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QMap>
#include <QDialog>
#include <QPointer>

//Forward declarations
class LogDataHandler2;
class GenerationItem;

class DataExplorer : public QDialog
{
    Q_OBJECT
public:
    explicit DataExplorer(QWidget *parent = 0);

signals:

public slots:
    void setLogdataHandler(LogDataHandler2 *pLogDataHandler);
    void refresh();
    void openImportDataDialog();
    void openExportDataDialog();

private slots:
    void refreshGenerationList();
    void refreshDataList();
    void removeSelectedGenerations();
    void removeGeneration(int gen);
    void toggleSelectAllGenerations();

private:
    QVector<int> gensFromSelected();
    QPointer<LogDataHandler2> mpLogDataHandler;
    QWidget *mpGenerationsListWidget;
    QScrollArea *mpGenerationsScrollArea;
    QMap<int, GenerationItem*> mGenerationItemMap;
    bool mAllSelectedToggle;

};

#endif // DATAEXPLORER_H
