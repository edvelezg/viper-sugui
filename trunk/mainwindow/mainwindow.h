#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include <QProcess>

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void newFile();
    bool save();
    bool saveAs();
    void runSimulation();
    void on_browseButton_clicked();

private:
    void setCurrentFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);
    void createActions();
    void createToolBars();
    QProcess process;
    int distancia;
    int profundidad;
    QString sourceFile;
    QString curFile;
    QToolBar *fileToolBar;
    QStringList recentFiles;
};

#endif