#include <QtGui>

#include "geometry.h"
#include "velocitymodel.h"

Geometry::Geometry(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

//  QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
//  lineEdit->setValidator(new QRegExpValidator(regExp, this));

    connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
    connect(buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
}

void Geometry::getParams(VelocityModel &vm) {
	// sbN1->setValue(vm.getN1().toInt());			
	// sbN2->setValue(vm.getN2().toInt());			
	// sbD1->setValue(vm.getD1().toInt());			
	// sbD2->setValue(vm.getD2().toInt());
	// 
	// if (vm.getCmap() == "hue") {
	// 	cbColor->setCurrentIndex(0);
	// } else {
	// 	cbColor->setCurrentIndex(1);
	// }
	// 
	// if (vm.getLegend() == "1") {
	// 	chkLeyenda->setCheckState(Qt::Checked);
	// } else {
	// 	chkLeyenda->setCheckState(Qt::Unchecked);
	// }
	// 
	// leTitulo->setText(vm.getTitulo());
}
