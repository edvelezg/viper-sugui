#include <QtGui>

#include "listdialog.h"
#include "editdialog.h"
//#include "wizard.h"

ListDialog::ListDialog( QWidget *parent ) : QDialog( parent ) 
{
    ui.setupUi( this );

    connect( ui.addButton, SIGNAL(clicked()), this, SLOT(addItem()) );
    connect( ui.editButton, SIGNAL(clicked()), this, SLOT(editItem()) );
    connect( ui.deleteButton, SIGNAL(clicked()), this, SLOT(deleteItem()) );
    connect( ui.list, SIGNAL(itemSelectionChanged ()), this, SLOT(modelChanged()) );

    ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	    
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
	
    if ( dlgEdit.exec() == Accepted ) {
        if ( vmMap.contains( dlgEdit.modelName() ) ) {
            VelocityModel vm( dlgEdit.modelName(), dlgEdit.modelLocation() );
            vmMap.insert( dlgEdit.modelName(), vm );
            ui.list->addItem( dlgEdit.modelName() );
            QMessageBox::information( this, 
                                      tr("Adicion Exitosa"),  
                                      tr("\"%1\" ha sido agregado a su lista de modelos")
                                      .arg(dlgEdit.modelName() ) );
        } else {
            QMessageBox::information( this,  
                                      tr("Error" ), 
                                      tr("El modelo con nombre \"%1\" ya existe en su lista de modelos")
                                      .arg( dlgEdit.modelName() ) );

            return;
        }
    }
}

void ListDialog::editItem()
{
    if ( !ui.list->currentItem() )
        return;

    EditDialog dlgEdit( this );

    VelocityModel vel;
    QString key = ui.list->currentItem()->text();

    if (vmMap.contains( key )) {
        vel = vmMap.value(key);
        dlgEdit.setModelName( vel.modelName() );
        dlgEdit.setModelLocation( vel.modelFile() );
    } else {
        return;
    }

    if (dlgEdit.exec() == Accepted) {
        if (vel.modelName() != dlgEdit.modelName() ) {
            if ( !vmMap.contains( dlgEdit.modelName() ) ) {
                vel.setModelName( dlgEdit.modelName() );
                vel.setModelFile( dlgEdit.modelLocation() );
                vmMap.remove( key );
                vmMap.insert( dlgEdit.modelName(),  vel );
                ui.list->currentItem()->setText( dlgEdit.modelName() );
                QMessageBox::information( this,  
                                          tr("Edicion Exitosa" ), 
                                          tr("\"%1\" ha sido editado." )
                                          .arg(dlgEdit.modelName() ) );
            } else {
                QMessageBox::information( this, 
                                          tr("Error" ), 
                                          tr("\"%1\" ya existe dentro de la lista de modelos." )
                                          .arg(dlgEdit.modelName() ) );
            }
        } else {
            vel.setModelFile( dlgEdit.modelLocation() );
            vmMap[key] = vel;
            ui.list->currentItem()->setText( dlgEdit.modelName() );
        }
    }
//  dlgEdit.setModelName      ( parts[0].trimmed() );
//  dlgEdit.setModelLocation  ( parts[1].trimmed() );
//
//  dlgEdit.setVelocities(mVels);
//
//  if ( dlgEdit.exec() == Accepted ) {
//      ui.list->currentItem()->setText( dlgEdit.modelName() + " -- " + dlgEdit.modelLocation() );
//      mVels = dlgEdit.velocities();
//  }
}

void ListDialog::deleteItem()
{
    delete ui.list->currentItem();
}

void ListDialog::modelChanged()
{
	mVels.clear();
	ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

const QString ListDialog::currentLocation() const
{
    QStringList parts = ui.list->currentItem()->text().split( "--" );
    return parts[1].trimmed();
}

const QStringList ListDialog::velocities() const {
    return mVels;
}

void ListDialog::setVelocities(QString vels) {
	if (vels == "") {
	} else {
		vels.remove(0, 4);
		this->mVels = vels.split(",");
	}
}
