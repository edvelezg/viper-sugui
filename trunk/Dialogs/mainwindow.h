#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include "ui_mainwindow.h"

class Geometry;
class ModelParams;
class VelocityModel;
class LoadModel;
class SimulParams;
class GetEnv;
class ListDialog;

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

    QString curFile;
	QString environment;

    QToolBar *fileToolBar;

    Geometry 	*dlgGeometry;
	LoadModel 	*dlgLoadModel;
    ModelParams *dlgModParams;
	SimulParams *dlgSimParams;
	GetEnv 		*dlgEnvironment;
    ListDialog  *dlgList;
	
    VelocityModel *vm;
    bool shows;
};

#endif