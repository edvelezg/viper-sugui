#include <QtGui>
#include "modelparams.h"

ModelParams::ModelParams(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

//  QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
//  lineEdit->setValidator(new QRegExpValidator(regExp, this));

    connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
    connect(buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
}

