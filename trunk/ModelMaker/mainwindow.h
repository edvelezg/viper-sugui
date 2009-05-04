#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"

class CanvasPicker;
class Plot;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT 

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();    
	void canvasModified();
	
private:
    void createActions();
    void createToolBars();
    bool okToContinue();
    bool saveFile(const QString &fileName);
    bool loadFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);

    QString curFile;

    QToolBar *fileToolBar;
	CanvasPicker *canvas;
	Plot		 *plot;
};

#endif
