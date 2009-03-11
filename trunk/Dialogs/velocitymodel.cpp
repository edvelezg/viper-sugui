#include <QtGui>
#include <QTextStream>
#include <QFile>
#include <QString>
#include <QMessageBox>
#include "velocitymodel.h"

VelocityModel::VelocityModel() {
	this->n1 = "100";
	this->n2 = "100";
	this->d1 = "5";
	this->d2 = "5";
	this->method = "spline";
    velocities.push_back( "2900" );
    velocities.push_back( "3300" );
    velocities.push_back( "3600" );
}

QString VelocityModel::getWidth	     ()					        { 	return width	 ;				}
QString VelocityModel::getHeight	 ()					        { 	return height	 ;				}	
QString VelocityModel::getWidthOff1  ()					        { 	return widthoff1 ;				}	
QString VelocityModel::getHeightOff1 ()					        { 	return heightoff1;				}
QString VelocityModel::getWidthOff2  ()					        { 	return widthoff2 ;				}
QString VelocityModel::getHeightOff2 ()					        { 	return heightoff2;				}

QString VelocityModel::getN1()					                { 	return n1;						}
QString VelocityModel::getN2()					                { 	return n2;						}	
QString VelocityModel::getD1()					                { 	return d1;					 	}	
QString VelocityModel::getD2()					                { 	return d2;						}
QString VelocityModel::getCmap()   		                        {   return cmap;        		    } 
QString VelocityModel::getLegend()   		                    {   return legend;      		    } 
QString VelocityModel::getTitulo()   		                    {   return titulo;      		    } 
QString VelocityModel::getMethod()				                { 	return method;					}
QString VelocityModel::getModelFile()				            { 	return modelFile;				}
QVector<QString> VelocityModel::getVelocities()				    { 	return velocities;				}

void VelocityModel::setWidth	    (QString width	   )   		{	this->width	      = width	  ;	}
void VelocityModel::setHeight	    (QString height	   )   		{	this->height	  = height	  ;	}	
void VelocityModel::setWidthOff1    (QString widthoff1 )   		{	this->widthoff1   = widthoff1 ;	}	
void VelocityModel::setHeightOff1   (QString heightoff1)   		{	this->heightoff1  = heightoff1;	}
void VelocityModel::setWidthOff2    (QString widthoff2 )   		{	this->widthoff2   = widthoff2 ;	}
void VelocityModel::setHeightOff2   (QString heightoff2)   		{	this->heightoff2  = heightoff2;	}
                                        		
void VelocityModel::setN1(QString n1)   		                {	this->n1 = n1;			        }
void VelocityModel::setN2(QString n2)   		                {	this->n2 = n2;			        }	
void VelocityModel::setD1(QString d1)   		                {	this->d1 = d1;			        }	
void VelocityModel::setD2(QString d2)   		                {	this->d2 = d2;			        }
void VelocityModel::setCmap(QString cmap)   		            {   this->cmap = cmap;              } 
void VelocityModel::setLegend(QString legend)   		        {   this->legend = legend;          } 
void VelocityModel::setTitulo(QString titulo)   		        {   this->titulo = titulo;          } 
void VelocityModel::setMethod(QString method)	                {	this->method = method; 	        }
void VelocityModel::setModelFile(QString modelFile)	            {	this->modelFile = modelFile; 	}
void VelocityModel::setVelocities(QVector<QString> velocities)	{	this->velocities = velocities; 	}

bool VelocityModel::writeFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//      QMessageBox::warning(this, tr("Simulation"),
//                           tr("Cannot write file %1:\n%2.")
//                           .arg(file.fileName())
//                           .arg(file.errorString()));
//      return false;
    }

    QTextStream out(&file);

    out << qPrintable(QString("%1").arg(width		)) << endl;
    out << qPrintable(QString("%1").arg(height		)) << endl; 
    out << qPrintable(QString("%1").arg(widthoff1	)) << endl; 
    out << qPrintable(QString("%1").arg(heightoff1	)) << endl; 
    out << qPrintable(QString("%1").arg(widthoff2	)) << endl; 
    out << qPrintable(QString("%1").arg(heightoff2	)) << endl; 

    out << qPrintable(QString("%1").arg(n1)) << endl;
    out << qPrintable(QString("%1").arg(n2)) << endl; 
    out << qPrintable(QString("%1").arg(d1)) << endl; 
    out << qPrintable(QString("%1").arg(d2)) << endl; 
    out << qPrintable(QString("%1").arg(cmap)) << endl; 
    out << qPrintable(QString("%1").arg(legend)) << endl;
    out << qPrintable(QString("%1").arg(titulo)) << endl; 
    out << qPrintable(QString("%1").arg(method)) << endl; 
    out << qPrintable(QString("%1").arg(modelFile)) << endl; 

    return true;
}

bool VelocityModel::readFile(const QString &fileName)
{
    QVector<QString> vals;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//      QMessageBox::warning(this, tr("Simulation"),
//                           tr("Cannot read file %1:\n%2.")
//                           .arg(file.fileName())
//                           .arg(file.errorString()));
        return false;
    }

    QTextStream in(&file);

	width	  			=     in.readLine().simplified(); 
    height	        	=     in.readLine().simplified(); 
    widthoff1       	=     in.readLine().simplified(); 
    heightoff1      	=     in.readLine().simplified(); 
    widthoff2       	=     in.readLine().simplified(); 
    heightoff2      	=     in.readLine().simplified(); 

	n1			=     in.readLine().simplified(); 
    n2      	=     in.readLine().simplified(); 
    d1      	=     in.readLine().simplified(); 
    d2      	=     in.readLine().simplified(); 
    cmap    	=     in.readLine().simplified(); 
    legend  	=     in.readLine().simplified(); 
	titulo      = 	  in.readLine().simplified(); 
	method      = 	  in.readLine().simplified(); 
	modelFile   = 	  in.readLine().simplified(); 
	
    return true;
}

void VelocityModel::clear() {
	this->n1 = "100";
	this->n2 = "100";
	this->d1 = "5";
	this->d2 = "5";
	this->method = "spline";
    velocities.push_back( "2900" );
    velocities.push_back( "3300" );
    velocities.push_back( "3600" );
}
