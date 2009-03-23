#include <QtGui>

#include "editdialog.h"

EditDialog::EditDialog( QWidget *parent ) : QDialog( parent )
{
  	setupUi( this );
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	// connect(ui.browseButton, SIGNAL(clicked()), this, SLOT(on_browseButton_clicked()));
}

const QString EditDialog::modelName() const
{
   return nameEdit->text().replace("--","").trimmed();
}

void EditDialog::setModelName( const QString &modelName )
{
   nameEdit->setText( modelName );
}

const QString EditDialog::modelLocation() const
{
  return sourceFileEdit->text().replace("--","").trimmed();
}

void EditDialog::setModelLocation( const QString &modelLocation )
{
  sourceFileEdit->setText( modelLocation );
}

void EditDialog::on_browseButton_clicked()
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
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

