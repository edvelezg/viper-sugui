#include <QtGui>

#include "simulparams.h"
#include "velocitymodel.h"

SimulParams::SimulParams(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
    connect(buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
}

void SimulParams::getParams(VelocityModel &vm) {
}

