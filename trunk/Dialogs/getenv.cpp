#include <QtGui>

#include "getenv.h"

GetEnv::GetEnv(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(leEnvironment, SIGNAL( textChanged(const QString &) ), this, SLOT( enableOkButton() ));
    connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
    connect(buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
}

void GetEnv::enableOkButton() {
	QString dirName = leEnvironment->text();
	environment = dirName;
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void GetEnv::on_browseButton_clicked()
{
    QString initialName = leEnvironment->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    QString dirName =
            QFileDialog::getExistingDirectory(this, tr("Escoja un Directorio"),
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
