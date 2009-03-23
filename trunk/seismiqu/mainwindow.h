#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class SeismicU;
class QAction;
class ConvertDialog;

class MainWindow : public QMainWindow 
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void openFile();

private:
    void createActions();
    void createMenus();
    void createToolBars();

	bool shows;

    QMenu *fileMenu;
    QToolBar *fileToolBar;
    QAction *exitAction;
    QAction *newAction;
    QAction *openAction;
    SeismicU *suDialog;
    ConvertDialog *dialog;
};

#endif
