#include <QtGui>
#include "modelparams.h"
#include "simulationmodel.h"

ModelParams::ModelParams(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

//  QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
//  lineEdit->setValidator(new QRegExpValidator(regExp, this));

    connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
    connect(buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
}

void ModelParams::getParams(SimulationModel &vm) {

	double n1 = vm.getN1().toDouble();
	double n2 =	vm.getN2().toDouble();
	double d1 =	vm.getD1().toDouble();
	double d2 =	vm.getD2().toDouble();
	
	double depth    = d1 * n1;
	double distance = d2 * n2;
		
	sbDepth->setValue(depth);			
	sbDistance->setValue(distance);			

	if (vm.getCmap() == "hue") {
		cbColor->setCurrentIndex(0);
	} else {
		cbColor->setCurrentIndex(1);
	}
	
	if (vm.getLegend() == "1") {
		chkLeyenda->setCheckState(Qt::Checked);
	} else {
		chkLeyenda->setCheckState(Qt::Unchecked);
	}
	
	if (vm.getMethod() == "spline") {
		cbMethod->setCurrentIndex(0);
	} else 	if (vm.getMethod() == "linear") {
		cbMethod->setCurrentIndex(1);
	} else 	if (vm.getMethod() == "mono") {
		cbMethod->setCurrentIndex(2);
	} else {
		cbMethod->setCurrentIndex(3);
	}
	
	leTitulo->setText(vm.getTitulo());
}
