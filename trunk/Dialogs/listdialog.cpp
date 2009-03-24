#include <QStringList>

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
    connect( ui.list, SIGNAL(currentTextChanged ( const QString &)), this, SLOT(enableButton()) );

    ui.list->addItem ( "Modelo 1 -- model.out");
    ui.list->addItem ( "Modelo 2 -- model2.out");
    ui.list->addItem ( "Modelo 3 -- model3.out");
    ui.list->addItem ( "Modelo 4 -- model4.out");

}

void ListDialog::addItem()
{
//  ClassWizard *wizard = new ClassWizard( this );
//      wizard->show();
    EditDialog dlg( this );

    if ( dlg.exec() == Accepted )
        ui.list->addItem( dlg.modelName() + " -- " + dlg.modelLocation() );
}

void ListDialog::editItem()
{
    if ( !ui.list->currentItem() )
        return;

    QStringList parts = ui.list->currentItem()->text().split( "--" );

    EditDialog dlg( this );
    dlg.setModelName      ( parts[0].trimmed() );
    dlg.setModelLocation  ( parts[1].trimmed() );

    if ( dlg.exec() == Accepted ) {
        ui.list->currentItem()->setText( dlg.modelName() + " -- " + dlg.modelLocation() );
        vels = dlg.velocities();
    }
}

void ListDialog::deleteItem()
{
    delete ui.list->currentItem();
}

void ListDialog::enableButton()
{
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
