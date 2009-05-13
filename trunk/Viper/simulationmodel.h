
#ifndef SIMULATIONMODEL_H
#define SIMULATIONMODEL_H

#include <QString>
#include <QVector>
#include "velocitymodel.h"

class SimulationModel
{

public:
	SimulationModel ();

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
	QString getVelocities();

	QString getDistance    ();
	QString getDepth       ();
	QString getFpeak       ();
	QString getFmax        ();
	QString getTitle       ();
	QString getTMax        ();
	QString getLoop        ();
	QString getSstrength   ();
	QString getRightBound  ();
	QString getLeftBound   ();
	QString getTopBound    ();
	QString getBottomBound ();
	
    QVector<VelocityModel> getModelsVector();

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
	void setVelocities(QString);
	void setModelsVector(QVector<VelocityModel>);

	void setDistance    (QString);
	void setDepth       (QString);
	void setFpeak       (QString);
	void setFmax        (QString);
	void setTitle       (QString);
	void setTMax        (QString);
	void setLoop        (QString);
	void setSstrength   (QString);
	void setRightBound 	(QString);
	void setLeftBound  	(QString);    
	void setTopBound   	(QString);    
	void setBottomBound	(QString);    
	

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
	QString title       ;
	QString tMax        ;
	QString loop        ;
	QString sstrength   ;
	QString rightBound  ;
	QString leftBound   ;
	QString topBound    ;
	QString bottomBound ;   
	QString velocities  ;
	QVector<VelocityModel> models  ;
};

#endif /* SIMULATIONMODEL_H */
