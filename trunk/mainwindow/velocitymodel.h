#ifndef _VELOCITYMODEL_H_
#define _VELOCITYMODEL_H_

#include <QString>

class VelocityModel
{
public:
	VelocityModel ();
	
	QString getN1();
	QString getN2();	
	QString getD1();	
	QString getD2();
	QString getMethod();

	void setN1(QString);
	void setN2(QString);	
	void setD1(QString);	
	void setD2(QString);
	void setMethod(QString);
		
private:
	QString n1;
	QString n2;
	QString d1;
	QString d2;
	QString method;
};


#endif /* _VELOCITYMODEL_H_ */
