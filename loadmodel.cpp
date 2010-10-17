#include <QtGui>

#include "loadmodel.h"
#include "simulationmodel.h"

LoadModel::LoadModel(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    QPushButton *loadButton =
            propagationModelButtonBox->button(QDialogButtonBox::Ok);
    loadButton->setEnabled(false);

    connect(propagationModelButtonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
    connect(propagationModelButtonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
}

void LoadModel::on_browseButton_clicked()
{
    QString initialName = sourceFileEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Choose File"),
                                         initialName,
										 tr("Model Files (*.out)"));
    fileName = QDir::toNativeSeparators(fileName);
    if (!fileName.isEmpty()) {
        sourceFileEdit->setText(fileName);
        propagationModelButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        modelFile = sourceFileEdit->text();
    }
}

void LoadModel::getParams(SimulationModel &vm) {
    sourceFileEdit->setText( vm.getModelFile() );
}

QString LoadModel::getModelFile() {
	return modelFile;
}

