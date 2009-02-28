#include "velocitymodel.h"

VelocityModel::VelocityModel() {
	this->n1 = "100";
	this->n2 = "100";
	this->d1 = "8";
	this->d2 = "8";
	this->method = "lime";
}

QString VelocityModel::getN1()					{ 	return n1;				}
QString VelocityModel::getN2()					{ 	return n2;				}	
QString VelocityModel::getD1()					{ 	return d1;				}	
QString VelocityModel::getD2()					{ 	return d2;				}
QString VelocityModel::getMethod()				{ 	return method;			}
                                        		
void VelocityModel::setN1		(QString n1)		{	this->n1 = n1;			}
void VelocityModel::setN2		(QString n2)		{	this->n2 = n2;			}	
void VelocityModel::setD1		(QString d1)		{	this->d1 = d1;			}	
void VelocityModel::setD2		(QString d2)		{	this->d2 = d2;			}
void VelocityModel::setMethod	(QString method)	{	this->method = method; 	}