#include <QtGui>
#include <iostream>
#include "classwizard.h"

using namespace std;

//! [0] //! [1]
ClassWizard::ClassWizard(QWidget *parent)
    : QWizard(parent)
{
    addPage(new IntroPage);
    addPage(new ClassInfoPage);
    addPage(new CodeStylePage);
    addPage(new ConclusionPage);

    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));

	setWindowTitle(tr("Velocity Model Wizard"));
}

void ClassWizard::accept()
{
    QString numLayers = field("numLayers").toString();

    QDialog::accept();
}
//! [6]

//! [7]
IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Introduccion"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark1.png"));

    label = new QLabel(tr("Este wizard le ayudara a crear su modelo de velocidad "
                          "Usted simplemente debe especificar algunos parametros. "
                          ));
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

ClassInfoPage::ClassInfoPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Numero de Capas"));
    setSubTitle(tr("Especifique el numero de Capas que quiere en su Modelo de Velocidad ") );
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo1.png"));

    lblNumLayers = new QLabel(tr("&Numero de Interfaces:"));
    leNumLayers = new QSpinBox();
    lblNumLayers->setBuddy(leNumLayers);

    registerField("numLayers*", leNumLayers);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(lblNumLayers, 0, 0);
    layout->addWidget(leNumLayers, 0, 1);

    setLayout(layout);
}

CodeStylePage::CodeStylePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Velocidades de la Capas"));
    setSubTitle(tr("Aqui debe definir las Velocidades de cada Capa que escogio."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo2.png"));

}

void CodeStylePage::initializePage()
{
    QGridLayout *layout	   = new QGridLayout;
	QSpinBox* sbVel        = NULL;
	QLabel* lblVel         = NULL;
	
    QString numLayers = field("numLayers").toString();
	int num                = numLayers.toInt();

	for (int i = 0; i < num; ++i) {
		sbVel = new QSpinBox();
		lblVel = new QLabel;
		lblVel->setText("Capa " + QString::number(i+1));
		sbVelocities.push_back(sbVel);
		lblVelocities.push_back(lblVel);
	    layout->addWidget(lblVel, i, 0);
	    layout->addWidget(sbVel, i, 1);
	}
	
	setLayout(layout);
    
}

ConclusionPage::ConclusionPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Conclusion"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark2.png"));

    label = new QLabel;
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

void ConclusionPage::initializePage()
{
    QString finishText = wizard()->buttonText(QWizard::FinishButton);
    finishText.remove('&');
    label->setText(tr("Click %1 to generate the class skeleton.")
                   .arg(finishText));
}
