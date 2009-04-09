#include <QStringList>
#include <QMessageBox>

#include "listdialog.h"
#include "editdialog.h"
#include "wizard.h"

ListDialog::ListDialog( QWidget *parent ) : QDialog( parent ) 
{
    ui.setupUi( this );
    ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect( ui.addButton, SIGNAL(clicked()), this, SLOT(addItem()) );
    connect( ui.editButton, SIGNAL(clicked()), this, SLOT(editItem()) );
    connect( ui.deleteButton, SIGNAL(clicked()), this, SLOT(deleteItem()) );
    connect( ui.list, SIGNAL(itemSelectionChanged()), this, SLOT(modelChanged()) );
	    
    ui.list->addItem ( "Modelo 1 -- model.out");
    ui.list->addItem ( "Modelo 2 -- model2.out");
    ui.list->addItem ( "Modelo 3 -- model3.out");
    ui.list->addItem ( "Modelo 4 -- model4.out");
}

void ListDialog::addItem()
{
//  ClassWizard *wizard = new ClassWizard( this );
//      wizard->show();
    EditDialog dlgEdit( this );
	
    if ( dlgEdit.exec() == Accepted )
        ui.list->addItem( dlgEdit.modelName() + " -- " + dlgEdit.modelLocation() );
}

void ListDialog::editItem()
{
    if ( !ui.list->currentItem() )
        return;

    QStringList parts = ui.list->currentItem()->text().split( "--" );

    EditDialog dlgEdit( this );
    dlgEdit.setModelName      ( parts[0].trimmed() );
    dlgEdit.setModelLocation  ( parts[1].trimmed() );

	dlgEdit.setVelocities(vels);

    if ( dlgEdit.exec() == Accepted ) {
        ui.list->currentItem()->setText( dlgEdit.modelName() + " -- " + dlgEdit.modelLocation() );
        vels = dlgEdit.velocities();
    }
}

void ListDialog::deleteItem()
{
    delete ui.list->currentItem();
}

void ListDialog::modelChanged()
{
	vels.clear();
	ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

const QString ListDialog::currentLocation() const
{
    QStringList parts = ui.list->currentItem()->text().split( "--" );
    return parts[1].trimmed();
}

const QStringList ListDialog::velocities() const {
    return vels;
}

void ListDialog::setVelocities(QString vels) {
	if (vels == "") {
	} else {
		vels.remove(0, 4);
		this->vels = vels.split(",");
	}
}
