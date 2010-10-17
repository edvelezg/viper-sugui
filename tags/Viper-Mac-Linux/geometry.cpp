#include <QtGui>

#include "geometry.h"
#include "simulationmodel.h"

Geometry::Geometry(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

//  QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
//  lineEdit->setValidator(new QRegExpValidator(regExp, this));

    connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
    connect(buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
}

void Geometry::getParams(SimulationModel &vm) {
	sbWidth->setValue		(      vm.getWidth().toInt() );   
    sbHeight->setValue		(     vm.getHeight().toInt() );
    sbWidth_2->setValue		(    vm.getWidth_2().toInt() );   
    sbHeight_2->setValue	(   vm.getHeight_2().toInt() );
    sbWidthOff1->setValue	(  vm.getWidthOff1().toInt() );
    sbHeightOff1->setValue	( vm.getHeightOff1().toInt() );
    sbWidthOff2->setValue	(  vm.getWidthOff2().toInt() );
    sbHeightOff2->setValue	( vm.getHeightOff2().toInt() );
}


