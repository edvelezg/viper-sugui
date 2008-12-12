#ifndef SEISMICU_H
#define SEISMICU_H

#include <QDialog>
#include <QProcess>

#include "ui_seismicu.h"

class SeismicU : public QDialog, private Ui::SeismicU
{
    Q_OBJECT

public:
    SeismicU(QWidget *parent = 0);

private slots:
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void runScript();
    void updateOutputTextEdit();
    void processError(QProcess::ProcessError error);


private:
    QProcess process;
    QString targetFile;
};

#endif
