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
    addPage(new OutputFilesPage);
    addPage(new ConclusionPage);
//! [0]

    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));

	setWindowTitle(tr("Velocity Model Wizard"));
//! [2]
}
//! [1] //! [2]

//! [3]
void ClassWizard::accept()
//! [3] //! [4]
{
    QString numLayers = field("numLayers").toString();
    QByteArray baseClass = field("baseClass").toByteArray();
    QByteArray macroName = field("macroName").toByteArray();
    QByteArray baseInclude = field("baseInclude").toByteArray();

    QString outputDir = field("outputDir").toString();
    QString header = field("header").toString();
    QString implementation = field("implementation").toString();

    QDialog::accept();
//! [5] //! [6]
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
//! [7]

//! [8] //! [9]
ClassInfoPage::ClassInfoPage(QWidget *parent)
    : QWizardPage(parent)
{
//! [8]
    setTitle(tr("Numero de Capas"));
    setSubTitle(tr("Especifique el numero de Capas que quiere en su Modelo de Velocidad ") );
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo1.png"));

//! [10]
    lblNumLayers = new QLabel(tr("&Numero de Interfaces:"));
    leNumLayers = new QSpinBox();
    lblNumLayers->setBuddy(leNumLayers);

    // connect(defaultCtorRadioButton, SIGNAL(toggled(bool)),
    //         copyCtorCheckBox, SLOT(setEnabled(bool)));

//! [11] //! [12]
    registerField("numLayers*", leNumLayers);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(lblNumLayers, 0, 0);
    layout->addWidget(leNumLayers, 0, 1);

    setLayout(layout);
//! [13]
}
//! [13]

//! [14]
CodeStylePage::CodeStylePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Velocidades de la Capas"));
    setSubTitle(tr("Aqui debe definir las Velocidades de cada Capa que escogio."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo2.png"));

    // registerField("comment", commentCheckBox);
    // registerField("protect", protectCheckBox);
    // registerField("macroName", macroNameLineEdit);
    // registerField("includeBase", includeBaseCheckBox);
    // registerField("baseInclude", baseIncludeLineEdit);
}
//! [15]

//! [16]
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
//! [16]

OutputFilesPage::OutputFilesPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Output Files"));
    setSubTitle(tr("Specify where you want the wizard to put the generated "
                   "skeleton code."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo3.png"));

    outputDirLabel = new QLabel(tr("&Output directory:"));
    outputDirLineEdit = new QLineEdit;
    outputDirLabel->setBuddy(outputDirLineEdit);

    headerLabel = new QLabel(tr("&Header file name:"));
    headerLineEdit = new QLineEdit;
    headerLabel->setBuddy(headerLineEdit);

    implementationLabel = new QLabel(tr("&Implementation file name:"));
    implementationLineEdit = new QLineEdit;
    implementationLabel->setBuddy(implementationLineEdit);

    registerField("outputDir*", outputDirLineEdit);
    registerField("header*", headerLineEdit);
    registerField("implementation*", implementationLineEdit);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(outputDirLabel, 0, 0);
    layout->addWidget(outputDirLineEdit, 0, 1);
    layout->addWidget(headerLabel, 1, 0);
    layout->addWidget(headerLineEdit, 1, 1);
    layout->addWidget(implementationLabel, 2, 0);
    layout->addWidget(implementationLineEdit, 2, 1);
    setLayout(layout);
}

//! [17]
void OutputFilesPage::initializePage()
{
    QString numLayers = field("numLayers").toString();
    headerLineEdit->setText(numLayers.toLower() + ".h");
    implementationLineEdit->setText(numLayers.toLower() + ".cpp");
    outputDirLineEdit->setText(QDir::convertSeparators(QDir::tempPath()));
}
//! [17]

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
