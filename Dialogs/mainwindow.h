#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include "ui_mainwindow.h"
#include "listdialog.h"
#include "simulationmodel.h"

class Geometry;
class ModelParams;
class LoadModel;
class SimulParams;
class GetEnv;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT 

public:
    MainWindow();
    bool wantsToShow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void run();
    void preview();
    void sizeSettings();
    void modelParams();
    void loadModel();
    void simParams();
	void viewTraces();
	void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void updateOutputTextEdit();
	void processError(QProcess::ProcessError error);

private:
    void createActions();
    void createToolBars();
    bool okToContinue();
    void readSettings();
    void writeSettings();
    bool saveFile(const QString &fileName);
    bool loadFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);

    QProcess unif2;
    QProcess ximage;
    QProcess sufdmod2;
    QProcess suxmovie;
    QProcess suxwigb;

    QString curFile;
	QString environment;

    QToolBar *fileToolBar;

    Geometry 	*dlgGeometry;
	LoadModel 	*dlgLoadModel;
    ModelParams *dlgModParams;
	SimulParams *dlgSimParams;
	GetEnv 		*dlgEnvironment;
    ListDialog  *dlgList;
	
    SimulationModel *model;
    bool shows;
};

#endif
