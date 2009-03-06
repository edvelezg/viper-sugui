#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include "ui_mainwindow.h"

class Geometry;
class ModelParams;
class VelocityModel;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT 

public:
    MainWindow();

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void run();
    void sizeSettings();
    void modelParams();

private:
    void createActions();
    void createToolBars();
    bool okToContinue();
    void setCurrentFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);

    QProcess unif2;
    QProcess ximage;
    QString sourceFile;
    QString curFile;

    QToolBar *fileToolBar;

    Geometry *dlgGeometry;
    ModelParams *dlgModParams;
    VelocityModel *vm;

};

#endif
