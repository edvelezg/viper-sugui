#include <QtGui>

#include "GetEnv.h"

GetEnv::GetEnv(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
    connect(buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
}

void GetEnv::on_browseButton_clicked()
{
    QString initialName = leEnvironment->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    QString dirName =
            QFileDialog::getExistingDirectory(this, tr("Choose Directory"),
                                         initialName);
    dirName = QDir::toNativeSeparators(dirName);
    if (!dirName.isEmpty()) {
        leEnvironment->setText(dirName);
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		environment = dirName;
    }
}

QString GetEnv::getEnvironment() {
	return environment;
}

