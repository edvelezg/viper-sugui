#include <QtGui>
#include <QTextStream>
#include <QFile>
#include <QString>
#include <QMessageBox>
#include "model.h"

Model::Model() {
        this->width         = "450"                 ;
        this->height        = "450"                 ;
        this->width_2       = "450"                 ;
        this->height_2      = "450"                 ;
        this->widthoff1     = "530"                 ;
        this->heightoff1    = "50"                  ;
        this->widthoff2     = "50"                  ;
        this->heightoff2    = "50"                  ;

        this->n1            = "100"                 ;
        this->n2            = "100"                 ;
        this->d1            = "5"                   ;
        this->d2            = "5"                   ;
        this->cmap          = "gray"                ;
        this->legend        = "0"                   ;
        this->titulo        = "Modelo de Velocidad" ;
        this->method        = "spline"              ;
        this->modelFile     = ""                    ;

        this->distance      = "250"                 ;
        this->depth         = "20"                  ;
        this->fpeak         = "35"                  ;
        this->fmax          = "40"                  ;
        this->windowTitle   = "Propagacion de Onda" ;
        this->title         = "Onda Sismica P"      ;
        this->tMax          = "0.40"                ;
        this->loop          = "1"                   ;
        this->clip          = "1.0"                 ;
		this->velocities	= ""					;
}

QString Model::getWidth	     ()					        { 	return width	 ;				}
QString Model::getHeight	 ()					        { 	return height	 ;				}	
QString Model::getWidth_2	     ()					    { 	return width_2	 ;				}
QString Model::getHeight_2	 ()					        { 	return height_2	 ;				}	
QString Model::getWidthOff1  ()					        { 	return widthoff1 ;				}	
QString Model::getHeightOff1 ()					        { 	return heightoff1;				}
QString Model::getWidthOff2  ()					        { 	return widthoff2 ;				}
QString Model::getHeightOff2 ()					        { 	return heightoff2;				}

QString Model::getN1()					                { 	return n1;						}
QString Model::getN2()					                { 	return n2;						}	
QString Model::getD1()					                { 	return d1;					 	}	
QString Model::getD2()					                { 	return d2;						}
QString Model::getCmap()   		                        {   return cmap;        		    } 
QString Model::getLegend()   		                    {   return legend;      		    } 
QString Model::getTitulo()   		                    {   return titulo;      		    } 
QString Model::getMethod()				                { 	return method;					}
QString Model::getModelFile()				            { 	return modelFile;				}

QString Model::getDistance    ()                        {   return distance    ;            }
QString Model::getDepth       ()                        {   return depth       ;            }
QString Model::getFpeak       ()                        {   return fpeak       ;            }
QString Model::getFmax        ()                        {   return fmax        ;            }
QString Model::getWindowTitle ()                        {   return windowTitle ;            }
QString Model::getTitle       ()                        {   return title       ;            }
QString Model::getTMax        ()                        {   return tMax        ;            }
QString Model::getLoop        ()                        {   return loop        ;            }
QString Model::getClip        ()                        {   return clip        ;            }

QString Model::getVelocities()				            { 	return velocities;				}

void Model::setWidth	    (QString width	   )   		{	this->width	      = width	  ;	}
void Model::setHeight	    (QString height	   )   		{	this->height	  = height	  ;	}	
void Model::setWidth_2	    (QString width_2	   )   		{	this->width_2	      = width_2	  ;	}
void Model::setHeight_2	    (QString height_2	   )   		{	this->height_2	  = height_2	  ;	}	
void Model::setWidthOff1    (QString widthoff1 )   		{	this->widthoff1   = widthoff1 ;	}	
void Model::setHeightOff1   (QString heightoff1)   		{	this->heightoff1  = heightoff1;	}
void Model::setWidthOff2    (QString widthoff2 )   		{	this->widthoff2   = widthoff2 ;	}
void Model::setHeightOff2   (QString heightoff2)   		{	this->heightoff2  = heightoff2;	}
                                        		
void Model::setN1(QString n1)   		                {	this->n1 = n1;			        }
void Model::setN2(QString n2)   		                {	this->n2 = n2;			        }	
void Model::setD1(QString d1)   		                {	this->d1 = d1;			        }	
void Model::setD2(QString d2)   		                {	this->d2 = d2;			        }
void Model::setCmap(QString cmap)   		            {   this->cmap = cmap;              } 
void Model::setLegend(QString legend)   		        {   this->legend = legend;          } 
void Model::setTitulo(QString titulo)   		        {   this->titulo = titulo;          } 
void Model::setMethod(QString method)	                {	this->method = method; 	        }
void Model::setModelFile(QString modelFile)	            {	this->modelFile = modelFile; 	}

void Model::setDistance    (QString distance   )        {   this->distance    =   distance   ; }
void Model::setDepth       (QString depth      )        {   this->depth       =   depth      ; }
void Model::setFpeak       (QString fpeak      )        {   this->fpeak       =   fpeak      ; }
void Model::setFmax        (QString fmax       )        {   this->fmax        =   fmax       ; }
void Model::setWindowTitle (QString windowTitle)        {   this->windowTitle =   windowTitle; }
void Model::setTitle       (QString title      )        {   this->title       =   title      ; }
void Model::setTMax        (QString tMax       )        {   this->tMax        =   tMax       ; }
void Model::setLoop        (QString loop       )        {   this->loop        =   loop       ; }
void Model::setClip        (QString clip       )        {   this->clip        =   clip       ; }

void Model::setVelocities(QString velocities)	        {	this->velocities = velocities; 	}

bool Model::writeFile(const QString &fileName)
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
	out << qPrintable(QString("%1").arg( velocities  )) << endl;

    return true;
}

bool Model::readFile(const QString &fileName)
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
	velocities			= 	  in.readLine().simplified();
	
    return true;
}

void Model::clear() {
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
	this->velocities	= ""					;

}
