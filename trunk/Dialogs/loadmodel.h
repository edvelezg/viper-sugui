#ifndef _LOADMODEL_H_
#define _LOADMODEL_H_

#include <QProcess>
#include <QDialog>

#include "ui_loadmodel.h"

class Model;

class LoadModel : public QDialog, public Ui::LoadModel
{
    Q_OBJECT

public:
    LoadModel(QWidget *parent = 0);
	QString getModelFile();
	void getParams(Model &vm);

public slots:
    void on_browseButton_clicked();

private:
    QString modelFile;

};

#endif /* _LOADMODEL_H_ */
