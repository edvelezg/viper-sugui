#ifndef _VELOCITYMODEL_H_
#define _VELOCITYMODEL_H_

#include <QString>
#include <QStringList>

class VelocityModel {

public:
    VelocityModel();
    VelocityModel ( QString modelName, QString modelFile );
    QString modelName() const;
    QString modelFile() const;

    QStringList velocities() const;

    void setModelName(QString modelName);
    void setModelFile(QString modelFile);

private:
    QString mModelName;   
    QString mModelFile;
    QStringList mVels;
};


#endif /* _VELOCITYMODEL_H_ */
