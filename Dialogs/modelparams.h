
#ifndef MODELPARAMS_H
#define MODELPARAMS_H

#include <QDialog>

#include "ui_modelparams.h"

class SimulationModel;

class ModelParams : public QDialog, public Ui::ModelDialog
{
    Q_OBJECT

public:
    ModelParams(QWidget *parent = 0);
	void getParams(SimulationModel &vm);
};

#endif
