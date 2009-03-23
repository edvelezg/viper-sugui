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
	QString getWidth_2	 ();
	QString getHeight_2	 ();	
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

	QString getDistance    ();
	QString getDepth       ();
	QString getFpeak       ();
	QString getFmax        ();
	QString getWindowTitle ();
	QString getTitle       ();
	QString getTMax        ();
	QString getLoop        ();
	QString getClip        ();

	void setWidth	 (QString);
	void setHeight	 (QString);	
	void setWidth_2	 (QString);
	void setHeight_2	 (QString);	
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

	void setDistance    (QString);
	void setDepth       (QString);
	void setFpeak       (QString);
	void setFmax        (QString);
	void setWindowTitle (QString);
	void setTitle       (QString);
	void setTMax        (QString);
	void setLoop        (QString);
	void setClip        (QString);

	bool writeFile(const QString &fileName);
	bool readFile(const QString &fileName);

	void clear();
		
private:
	QString width		;	
	QString height		;   
	QString width_2		;	
	QString height_2	;   
	QString widthoff1	;   
	QString heightoff1	;   
	QString widthoff2	;   
	QString heightoff2	;   

	QString n1			;	
	QString n2			;   
	QString d1			;   
	QString d2			;   
	QString cmap		;   
	QString legend		;   
	QString titulo		;   
	QString method		;   
	QString modelFile	;   

	QString distance    ;
	QString depth       ;
	QString fpeak       ;
	QString fmax        ;
	QString windowTitle ;
	QString title       ;
	QString tMax        ;
	QString loop        ;
	QString clip        ;

	QVector<QString> velocities;
};


#endif /* _VELOCITYMODEL_H_ */
