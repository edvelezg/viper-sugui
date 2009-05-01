
#include <QtGui>
#include "velocitymodel.h"

VelocityModel::VelocityModel() {
}
VelocityModel::VelocityModel (QString modelName, QString modelFile ) : mModelName( modelName ), mModelFile( modelFile ) {

	QFile file(modelFile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
	     // QMessageBox::warning(this, tr("Simulation"),
	     //                      tr("Cannot read file %1:\n%2.")
	     //                      .arg(file.fileName())
	     //                      .arg(file.errorString()));
	        return;
	}
	
	// Counts the number of layers
	QTextStream in(&file);
	
	mNumLayers = 0;
    QString word;
    while (!in.atEnd()) {
        in >> word;
        if (word == "-99999") {
			++mNumLayers;
        }
    }
}

QString VelocityModel::modelName() const { return mModelName; }
QString VelocityModel::modelFile() const { return mModelFile; }
int VelocityModel::numLayers() { return mNumLayers; }
QStringList VelocityModel::velocities() const { return mVels; }
void VelocityModel::setModelName(QString modelName) { mModelName = modelName; }
void VelocityModel::setModelFile(QString modelFile) { mModelFile = modelFile; }
void VelocityModel::setVelocities(QStringList vels) { mVels = vels; }
void VelocityModel::clearVelocities() { 

	QFile file(mModelFile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
	     // QMessageBox::warning(this, tr("Simulation"),
	     //                      tr("Cannot read file %1:\n%2.")
	     //                      .arg(file.fileName())
	     //                      .arg(file.errorString()));
	        return;
	}
	
	// Counts the number of layers
	QTextStream in(&file);
	
	mNumLayers = 0;
    QString word;
    while (!in.atEnd()) {
        in >> word;
        if (word == "-99999") {
			++mNumLayers;
        }
    }
	
	mVels.clear(); 
}

