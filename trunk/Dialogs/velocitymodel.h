#ifndef _VELOCITYMODEL_H_
#define _VELOCITYMODEL_H_

#include <QString>
#include <QVector>

class VelocityModel
{

public:
	VelocityModel ();

	QString getWidth	 ();
	QString getHeight	 ();	
	QString getWidthOff1 ();	
	QString getHeightOff1();
	QString getWidthOff2 ();
	QString getHeightOff2();

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

	void setWidth	 (QString);
	void setHeight	 (QString);	
	void setWidthOff1 (QString);	
	void setHeightOff1(QString);
	void setWidthOff2 (QString);
	void setHeightOff2(QString);

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
	QString width		;			// Tamaño de ventana
	QString height		;         // Tamaño de ventana
	QString widthoff1	;      // Tamaño de ventana
	QString heightoff1	;     // Tamaño de ventana
	QString widthoff2	;      // Tamaño de ventana
	QString heightoff2	;     // Tamaño de ventana

	QString n1			;				// Parametros de Velocidad
	QString n2			;             // Parametros de Velocidad
	QString d1			;             // Parametros de Velocidad
	QString d2			;             // Parametros de Velocidad
	QString cmap		;           // Parametros de Velocidad
	QString legend		;         // Parametros de Velocidad
	QString titulo		;         // Parametros de Velocidad
	QString method		;         // Parametros de Velocidad
	QString modelFile	;      // Parametros de Velocidad
	
	QVector<QString> velocities;
};


#endif /* _VELOCITYMODEL_H_ */
