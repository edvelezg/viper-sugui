#include <QtGui>
#include "wizard.h"

ClassWizard::ClassWizard(QWidget *parent)
    : QWizard(parent)
{
    addPage(new NumObjectsPage);
    addPage(new ObjectsPage);

	setWindowTitle(tr("Wizard"));
}


NumObjectsPage::NumObjectsPage(QWidget *parent)
    : QWizardPage(parent)
{
    lblNumObjs = new QLabel(tr("Number of Objects:"));
    leNumObjs = new QSpinBox();
    lblNumObjs->setBuddy(leNumObjs);

    registerField("numObjs*", leNumObjs);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(lblNumObjs, 0, 0);
    layout->addWidget(leNumObjs, 0, 1);

    setLayout(layout);
}

ObjectsPage::ObjectsPage(QWidget *parent)
    : QWizardPage(parent)
{
}

void ObjectsPage::cleanupPage()
{
	for (int i = 0; i < sbObjs.size(); ++i) {
        delete (sbObjs[i]);
        delete (lblObjs[i]);
    }
    delete layout;
    sbObjs.clear();
    lblObjs.clear();
}

void ObjectsPage::initializePage()
{
    layout	               = new QGridLayout;
	QSpinBox* sbVel        = NULL;
	QLabel* lblVel         = NULL;
	
    QString numLayers = field("numObjs").toString();
	int num           = numLayers.toInt();

	for (int i = 0; i < num; ++i) {
		sbVel = new QSpinBox();
		lblVel = new QLabel;
		lblVel->setText("Capa " + QString::number(i+1));
		sbObjs.push_back(sbVel);
		lblObjs.push_back(lblVel);
	    layout->addWidget(lblVel, i, 0);
	    layout->addWidget(sbVel, i, 1);
	}
	
	setLayout(layout);
    
}
