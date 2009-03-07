#ifndef LOADMODEL_H
#define LOADMODEL_H

#include <QProcess>
#include <QDialog>

#include "ui_loadmodel.h"

class LoadModel : public QDialog, public Ui::LoadModel
{
    Q_OBJECT

public:
    LoadModel(QWidget *parent = 0);
	QString getModelFile();

private slots:
    void on_browseButton_clicked();
    void runSimulation();

private:
    QString modelFile;
    QProcess ximage;

};

#endif
