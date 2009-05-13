#include <QtGui>

#include "simulparams.h"
#include "simulationmodel.h"

SimulParams::SimulParams(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
    connect(buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
	
}

void SimulParams::getParams(SimulationModel &vm) {

    sbDistance->setValue    (   vm.getDistance    ().toDouble() );
    sbDepth->setValue       (   vm.getDepth       ().toDouble() );
    sbFpeak->setValue       (   vm.getFpeak       ().toInt()    );
    sbFmax->setValue        (   vm.getFmax        ().toInt()    );
    leTitle->setText        (   vm.getTitle       ()            );
    sbTMax->setValue        (   vm.getTMax        ().toDouble() );

	if (vm.getLoop() == "1") {
		chkLoop->setCheckState(Qt::Checked);
	} else {
		chkLoop->setCheckState(Qt::Unchecked);
	}
    sbSstrength->setValue        (   vm.getSstrength        ().toDouble() );
    
	if (vm.getTopBound() == "0") {
		chkTop->setCheckState(Qt::Checked);
	} else {
		chkTop->setCheckState(Qt::Unchecked);
	}

	if (vm.getBottomBound() == "0") {
		chkBottom->setCheckState(Qt::Checked);
	} else {
		chkBottom->setCheckState(Qt::Unchecked);
	}

	if (vm.getRightBound() == "0") {
		chkRight->setCheckState(Qt::Checked);
	} else {
		chkRight->setCheckState(Qt::Unchecked);
	}

	if (vm.getLeftBound() == "0") {
		chkLeft->setCheckState(Qt::Checked);
	} else {
		chkLeft->setCheckState(Qt::Unchecked);
	}

}
