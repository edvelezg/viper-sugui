#ifndef GOTOCELLDIALOG_H
#define GOTOCELLDIALOG_H

#include <QDialog>

#include "ui_geometry.h"

class Model;

class Geometry : public QDialog, public Ui::GeoDialog
{
    Q_OBJECT

public:
    Geometry(QWidget *parent = 0);
	void getParams(Model &vm);
};

#endif
