#include <QtGui>

#include "listdialog.h"
#include "editdialog.h"
#include "coordinatesetter.h"
#include <QtDebug>
//#include "wizard.h"

ListDialog::ListDialog( QWidget *parent ) : QDialog( parent ) 
{
    ui.setupUi( this );

    connect( ui.addButton, SIGNAL(clicked()), this, SLOT(addItem()) );
    connect( ui.editButton, SIGNAL(clicked()), this, SLOT(editItem()) );
    connect( ui.deleteButton, SIGNAL(clicked()), this, SLOT(deleteItem()) );
    connect( ui.list, SIGNAL(itemSelectionChanged ()), this, SLOT(modelChanged()) );
    connect( ui.velocityButton, SIGNAL(clicked()), this, SLOT(setVelocities()));


    ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    VelocityModel vm1( "Modelo 1", QDir::currentPath() + "/model.out"  );
	vmMap.insert( "Modelo 1", vm1 );
    ui.list->addItem ( "Modelo 1");

    VelocityModel vm2( "Modelo 2", QDir::currentPath() + "/model2.out" );
	vmMap.insert( "Modelo 2", vm2 );
    ui.list->addItem ( "Modelo 2");

    VelocityModel vm3( "Modelo 3", QDir::currentPath() + "/model3.out" );
	vmMap.insert( "Modelo 3", vm3 );
    ui.list->addItem ( "Modelo 3");

    VelocityModel vm4( "Modelo 4", QDir::currentPath() + "/model4.out" );
	vmMap.insert( "Modelo 4", vm4 );
    ui.list->addItem ( "Modelo 4");

    VelocityModel vm5( "Modelo 5", QDir::currentPath() + "/model5.out" );
	vmMap.insert( "Modelo 5", vm5 );
    ui.list->addItem ( "Modelo 5");
}

void ListDialog::addItem()
{
//  ClassWizard *wizard = new ClassWizard( this );
//      wizard->show();
    EditDialog dlgEdit( this );
	
    if ( dlgEdit.exec() == Accepted ) {
        if ( !vmMap.contains( dlgEdit.modelName() ) ) {
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
			vel.clearVelocities();
			mVels = vel.velocities();
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
	if ( !ui.list->currentItem() )
        return;
    
	QString key = ui.list->currentItem()->text();
    VelocityModel vel = vmMap.value(key);
	mVels = vel.velocities();
	ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

const QString ListDialog::currentLocation() const
{
	if ( !ui.list->currentItem() )
        return "";
	
    QString key = ui.list->currentItem()->text();
    VelocityModel vel = vmMap.value(key);

	return vel.modelFile();
}

const QStringList ListDialog::velocities() const {
    return mVels;
}

void ListDialog::setVelocities(QString vels) {
	if (vels == "") {
	} else {
		vels.remove(0, 4);
		mVels = vels.split(",");
	}
}

void ListDialog::setVelocities( const QStringList &vels ) {
	mVels = vels;
}

void ListDialog::setVelocities()
{
	if ( !ui.list->currentItem() )
        return;
	
	QString key = ui.list->currentItem()->text();
    VelocityModel vel = vmMap.value(key);
	int numLayers = vel.numLayers();
	
	QList<double> coordinates;
	if (mVels.size() == 0) {
		double inc = 2500.0 / (numLayers - 1.0);
	    for (int i = 0; i < numLayers - 1 ; ++i ) {
	        coordinates << 1500.0 + i*inc;
	    }
	}
	
	QString text;
	foreach( text, mVels ) {
		coordinates << text.toDouble();
		qDebug() << text;
	}
	
	CoordinateSetter coordinateSetter(&coordinates, this);
	coordinateSetter.show();

	// CoordinateSetter *coordinateSetter = 0;
	//   
	// if (!coordinateSetter) {
	//         coordinateSetter = new CoordinateSetter(&coordinates, this);
	//     } else {
	//         coordinateSetter->show();
	//     }

    if (coordinateSetter.exec()) {
		mVels = coordinateSetter.velocities();
		vel.setVelocities(mVels);
		vmMap[key] = vel;
    } else {
	
	}
    
    // if (!wizard) {
    //        wizard = new ClassWizard( this );
    //    } else {
    //        wizard->show();
    //    }
    // 
    //    if ( wizard->exec() ) {
    //        QString numObjs = wizard->field("numObjs").toString();
    // 
    //        qDebug() << "number of objects: " << numObjs;
    // 
    //        int num = numObjs.toInt();
    //        for ( int i = 0; i < num; ++i ) {
    //            QString strVel = wizard->field("sbVel" + QString::number(i)).toString();
    //            qDebug() << "velocity "  + QString::number(i) << strVel;
    //            vels << strVel;
    //        }
    //        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    //    } else {
    // 		qDebug() << "cancelled action";
    //    }
}
