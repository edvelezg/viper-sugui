#ifndef _VELOCITYMODEL_H_
#define _VELOCITYMODEL_H_

#include <QString>
#include <QVector>

class VelocityModel
{

public:
	VelocityModel ();

	QString getN1();
	QString getN2();	
	QString getD1();	
	QString getD2();
	QString getCmap();
	QString getLegend();
	QString getTitulo();
	QString getMethod();
	QString getModelFile();
	QVector<QString> getVelocities();

	void setN1(QString);
	void setN2(QString);	
	void setD1(QString);	
	void setD2(QString);
	void setCmap(QString);
	void setLegend(QString);
	void setTitulo(QString);
	void setMethod(QString);
	void setModelFile(QString);
	void setVelocities(QVector<QString>);

    bool writeFile(const QString &fileName);
    bool readFile(const QString &fileName);

	void clear();
		
private:
	QString n1;
	QString n2;
	QString d1;
	QString d2;
	QString cmap;
	QString legend;
	QString titulo;
	QString method;
	QString modelFile;

	QVector<QString> velocities;
};


#endif /* _VELOCITYMODEL_H_ */
