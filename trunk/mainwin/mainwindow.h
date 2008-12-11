#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;

class MainWindow : public QMainWindow 
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();

private:
    void createActions();
    void createMenus();
    void createToolBars();

    QMenu *fileMenu;
    QToolBar *fileToolBar;
    QAction *exitAction;
    QAction *newAction;
};

#endif
