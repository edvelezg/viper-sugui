
#include <QtGui>
#include "velocitymodel.h"

VelocityModel::VelocityModel() {
}
VelocityModel::VelocityModel (QString modelName, QString modelFile ) : mModelName( modelName ), mModelFile( modelFile ) {
    mVels = modelFile.split(",");
}
QString VelocityModel::modelName() const { return mModelName; }
QString VelocityModel::modelFile() const { return mModelFile; }
QStringList VelocityModel::velocities() const { return mVels; }
void VelocityModel::setModelName(QString modelName) { mModelName = modelName; }
void VelocityModel::setModelFile(QString modelFile) { mModelFile = modelFile; }

