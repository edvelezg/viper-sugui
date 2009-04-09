#include <QtGui>

#include "simulparams.h"
#include "model.h"

SimulParams::SimulParams(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
    connect(buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
}

void SimulParams::getParams(Model &vm) {

    sbDistance->setValue    (   vm.getDistance    ().toDouble() );
    sbDepth->setValue       (   vm.getDepth       ().toDouble() );
    sbFpeak->setValue       (   vm.getFpeak       ().toInt()    );
    sbFmax->setValue        (   vm.getFmax        ().toInt()    );
    leWindowTitle->setText  (   vm.getWindowTitle ()            );
    leTitle->setText        (   vm.getTitle       ()            );
    sbTMax->setValue        (   vm.getTMax        ().toDouble() );

	if (vm.getLoop() == "1") {
		chkLoop->setCheckState(Qt::Checked);
	} else {
		chkLoop->setCheckState(Qt::Unchecked);
	}
    sbClip->setValue        (   vm.getClip        ().toDouble() );
}
