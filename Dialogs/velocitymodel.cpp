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

    in >> n1;
    in >> n2; 
    in >> d1; 
    in >> d2; 
    in >> cmap; 
    in >> legend; 
    in >> titulo; 
    in >> method; 
    in >> modelFile; 

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
