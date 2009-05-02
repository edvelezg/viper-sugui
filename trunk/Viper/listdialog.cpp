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
    connect( ui.list, SIGNAL(itemDoubleClicked (QListWidgetItem * item)), this, SLOT(accept()));
    connect( ui.velocityButton, SIGNAL(clicked()), this, SLOT(setVelocities()));


    ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
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
//          QMessageBox::information( this,
//                                    tr("Adicion Exitosa"),
//                                    tr("\"%1\" ha sido agregado a su lista de modelos")
//                                    .arg(dlgEdit.modelName() ) );
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
//              QMessageBox::information( this,
//                                        tr("Edicion Exitosa" ),
//                                        tr("\"%1\" ha sido editado." )
//                                        .arg(dlgEdit.modelName() ) );
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
	if ( !ui.list->currentItem() )
        return;

	QString key = ui.list->currentItem()->text();

    delete ui.list->currentItem();
    vmMap.remove(key);
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


const QVector<VelocityModel> ListDialog::getModels() const
{
    QVector<VelocityModel> modelVector;
    QMap<QString, VelocityModel>::ConstIterator ii;
    for ( ii = vmMap.constBegin(); ii != vmMap.constEnd(); ++ii ) {
        ii.value();
        modelVector.push_back(ii.value());
    }
    return modelVector;
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

void ListDialog::setCurrentModel(int currentRow) {
        ui.list->setCurrentRow( currentRow );
}

int ListDialog::getCurrentModel() {
    return ui.list->currentRow();
}

void ListDialog::setModels(const QVector<VelocityModel> &models) {
    ui.list->clear();
    vmMap.clear();
    for ( int i = 0; i < models.size(); ++i ) {
        vmMap.insert( models[i].modelName(), models[i] );
        ui.list->addItem( models[i].modelName() );
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
	
	VelocitySetter velocitySetter(&coordinates, this);
	velocitySetter.show();

    if (velocitySetter.exec()) {
		mVels = velocitySetter.velocities();
		vel.setVelocities(mVels);
		vmMap[key] = vel;
    } else {
	
	}
}


void ListDialog::setVelocities(QListWidgetItem * item)
{
	QString key = item->text();
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
	
	VelocitySetter velocitySetter(&coordinates, this);
	velocitySetter.show();

    if (velocitySetter.exec()) {
		mVels = velocitySetter.velocities();
		vel.setVelocities(mVels);
		vmMap[key] = vel;
    } else {
	
	}
}
