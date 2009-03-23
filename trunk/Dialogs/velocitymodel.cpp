#include <QtGui>
#include <QTextStream>
#include <QFile>
#include <QString>
#include <QMessageBox>
#include "velocitymodel.h"

VelocityModel::VelocityModel() {
        this->width         = "450"                 ;
        this->height        = "450"                 ;
        this->width_2       = "450"                 ;
        this->height_2      = "450"                 ;
        this->widthoff1     = "530"                 ;
        this->heightoff1    = "50"                  ;
        this->widthoff2     = "50"                  ;
        this->heightoff2    = "50"                  ;
                                                    ;
        this->n1            = "100"                 ;
        this->n2            = "100"                 ;
        this->d1            = "5"                   ;
        this->d2            = "5"                   ;
        this->cmap          = "gray"                ;
        this->legend        = "0"                   ;
        this->titulo        = "Modelo de Velocidad" ;
        this->method        = "spline"              ;
        this->modelFile     = ""                    ;

        this->distance      = "250"  ;
        this->depth         = "20"  ;
        this->fpeak         = "35"  ;
        this->fmax          = "40"  ;
        this->windowTitle   = "Random Title"  ;
        this->title         = "Another Title"  ;
        this->tMax          = "0.40"  ;
        this->loop          = "1"  ;
        this->clip          = "1.0"  ;
}

QString VelocityModel::getWidth	     ()					        { 	return width	 ;				}
QString VelocityModel::getHeight	 ()					        { 	return height	 ;				}	
QString VelocityModel::getWidth_2	     ()					        { 	return width_2	 ;				}
QString VelocityModel::getHeight_2	 ()					        { 	return height_2	 ;				}	
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

QString VelocityModel::getDistance    ()                        {   return distance    ;            }
QString VelocityModel::getDepth       ()                        {   return depth       ;            }
QString VelocityModel::getFpeak       ()                        {   return fpeak       ;            }
QString VelocityModel::getFmax        ()                        {   return fmax        ;            }
QString VelocityModel::getWindowTitle ()                        {   return windowTitle ;            }
QString VelocityModel::getTitle       ()                        {   return title       ;            }
QString VelocityModel::getTMax        ()                        {   return tMax        ;            }
QString VelocityModel::getLoop        ()                        {   return loop        ;            }
QString VelocityModel::getClip        ()                        {   return clip        ;            }

QVector<QString> VelocityModel::getVelocities()				    { 	return velocities;				}

void VelocityModel::setWidth	    (QString width	   )   		{	this->width	      = width	  ;	}
void VelocityModel::setHeight	    (QString height	   )   		{	this->height	  = height	  ;	}	
void VelocityModel::setWidth_2	    (QString width_2	   )   		{	this->width_2	      = width_2	  ;	}
void VelocityModel::setHeight_2	    (QString height_2	   )   		{	this->height_2	  = height_2	  ;	}	
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

void VelocityModel::setDistance    (QString distance   )        {   this->distance    =   distance   ; }
void VelocityModel::setDepth       (QString depth      )        {   this->depth       =   depth      ; }
void VelocityModel::setFpeak       (QString fpeak      )        {   this->fpeak       =   fpeak      ; }
void VelocityModel::setFmax        (QString fmax       )        {   this->fmax        =   fmax       ; }
void VelocityModel::setWindowTitle (QString windowTitle)        {   this->windowTitle =   windowTitle; }
void VelocityModel::setTitle       (QString title      )        {   this->title       =   title      ; }
void VelocityModel::setTMax        (QString tMax       )        {   this->tMax        =   tMax       ; }
void VelocityModel::setLoop        (QString loop       )        {   this->loop        =   loop       ; }
void VelocityModel::setClip        (QString clip       )        {   this->clip        =   clip       ; }

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
    out << qPrintable(QString("%1").arg(width_2		)) << endl;
    out << qPrintable(QString("%1").arg(height_2	)) << endl; 
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

	out << qPrintable(QString("%1").arg( distance    )) << endl;
	out << qPrintable(QString("%1").arg( depth       )) << endl;
	out << qPrintable(QString("%1").arg( fpeak       )) << endl;
	out << qPrintable(QString("%1").arg( fmax        )) << endl;
	out << qPrintable(QString("%1").arg( windowTitle )) << endl;
	out << qPrintable(QString("%1").arg( title       )) << endl;
	out << qPrintable(QString("%1").arg( tMax        )) << endl;
	out << qPrintable(QString("%1").arg( loop        )) << endl;
	out << qPrintable(QString("%1").arg( clip        )) << endl;

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
	width_2	  			=     in.readLine().simplified(); 
    height_2        	=     in.readLine().simplified(); 
    widthoff1       	=     in.readLine().simplified(); 
    heightoff1      	=     in.readLine().simplified(); 
    widthoff2       	=     in.readLine().simplified(); 
    heightoff2      	=     in.readLine().simplified(); 

	n1			        =     in.readLine().simplified(); 
    n2      	        =     in.readLine().simplified(); 
    d1      	        =     in.readLine().simplified(); 
    d2      	        =     in.readLine().simplified(); 
    cmap    	        =     in.readLine().simplified(); 
    legend  	        =     in.readLine().simplified(); 
	titulo              = 	  in.readLine().simplified(); 
	method              = 	  in.readLine().simplified(); 
	modelFile           = 	  in.readLine().simplified(); 

	distance            =     in.readLine().simplified();
	depth               =     in.readLine().simplified();
	fpeak               =     in.readLine().simplified();
	fmax                =     in.readLine().simplified();
	windowTitle         =     in.readLine().simplified();
	title               =     in.readLine().simplified();
	tMax                =     in.readLine().simplified();
	loop                =     in.readLine().simplified();
	clip                =     in.readLine().simplified();
	
    return true;
}

void VelocityModel::clear() {
    this->width         = "450"                 ;
    this->height        = "450"                 ;
    this->width_2       = "450"                 ;
    this->height_2      = "450"                 ;
    this->widthoff1     = "530"                  ;
    this->heightoff1    = "50"                 ;
    this->widthoff2     = "50"                  ;
    this->heightoff2    = "50"                 ;
                                                ;
    this->n1            = "100"                 ;
    this->n2            = "100"                 ;
    this->d1            = "5"                   ;
    this->d2            = "5"                   ;
    this->cmap          = "gray"                ;
    this->legend        = "0"                   ;
    this->titulo        = "Modelo de Velocidad" ;
    this->method        = "spline"              ;
    this->modelFile     = ""                    ;

    this->distance      = "250"  ;
    this->depth         = "20"  ;
    this->fpeak         = "35"  ;
    this->fmax          = "40"  ;
    this->windowTitle   = "Random Title"  ;
    this->title         = "Another Title"  ;
    this->tMax          = "0.40"  ;
    this->loop          = "1"  ;
    this->clip          = "1.0"  ;
}
