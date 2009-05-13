#include <QtGui>
#include <QTextStream>
#include <QFile>
#include <QString>
#include <QMessageBox>
#include "simulationmodel.h"

SimulationModel::SimulationModel() {
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
        this->titulo        = "Ondas Sismicas P" ;
        this->method        = "spline"              ;
        this->modelFile     = ""                    ;

        this->distance      = "250"                 ;
        this->depth         = "20"                  ;
        this->fpeak         = "35"                  ;
        this->fmax          = "40"                  ;
        this->title         = "Ondas Sismicas P"      ;
        this->tMax          = "0.40"                ;
        this->loop          = "1"                   ;
        this->sstrength     = "1.0"                 ;
		this->velocities	= ""					;
		
		this->rightBound 	= "1"	;
		this->leftBound  	= "1"	;
		this->topBound   	= "0"	;
        this->bottomBound	= "1"	;

        models.push_back(VelocityModel("Modelo 1", QDir::currentPath() + "/model.out"));
        models.push_back(VelocityModel("Modelo 2", QDir::currentPath() + "/model2.out"));
        models.push_back(VelocityModel("Modelo 3", QDir::currentPath() + "/model3.out"));
        models.push_back(VelocityModel("Modelo 4", QDir::currentPath() + "/model4.out"));
        models.push_back(VelocityModel("Modelo 5", QDir::currentPath() + "/model5.out"));
}

QString SimulationModel::getWidth	     ()					        { 	return width	 ;				}
QString SimulationModel::getHeight	 ()					        { 	return height	 ;				}	
QString SimulationModel::getWidth_2	     ()					    { 	return width_2	 ;				}
QString SimulationModel::getHeight_2	 ()					        { 	return height_2	 ;				}	
QString SimulationModel::getWidthOff1  ()					        { 	return widthoff1 ;				}	
QString SimulationModel::getHeightOff1 ()					        { 	return heightoff1;				}
QString SimulationModel::getWidthOff2  ()					        { 	return widthoff2 ;				}
QString SimulationModel::getHeightOff2 ()					        { 	return heightoff2;				}

QString SimulationModel::getN1()					                { 	return n1;						}
QString SimulationModel::getN2()					                { 	return n2;						}	
QString SimulationModel::getD1()					                { 	return d1;					 	}	
QString SimulationModel::getD2()					                { 	return d2;						}
QString SimulationModel::getCmap()   		                        {   return cmap;        		    } 
QString SimulationModel::getLegend()   		                    {   return legend;      		    } 
QString SimulationModel::getTitulo()   		                    {   return titulo;      		    } 
QString SimulationModel::getMethod()				                { 	return method;					}
QString SimulationModel::getModelFile()				            	{ 	return modelFile;				}

QString SimulationModel::getDistance    ()                        {   return distance   ;            }
QString SimulationModel::getDepth       ()                        {   return depth      ;            }
QString SimulationModel::getFpeak       ()                        {   return fpeak      ;            }
QString SimulationModel::getFmax        ()                        {   return fmax       ;            }
QString SimulationModel::getTitle       ()                        {   return title      ;            }
QString SimulationModel::getTMax        ()                        {   return tMax       ;            }
QString SimulationModel::getLoop        ()                        {   return loop       ;            }
QString SimulationModel::getSstrength   ()                        {   return sstrength  ;            }
QString SimulationModel::getRightBound  ()     					  {   return rightBound ; 			  }
QString SimulationModel::getLeftBound   ()     					  {   return leftBound  ; 			  }
QString SimulationModel::getTopBound    ()     					  {   return topBound   ; 			  }
QString SimulationModel::getBottomBound ()     					  {   return bottomBound; 			  }


QString SimulationModel::getVelocities()				            { 	return velocities;				}
QVector<VelocityModel> SimulationModel::getModelsVector()				            { 	return models;				}

void SimulationModel::setWidth	    (QString width	   )   		{	this->width	      = width	  ;	}
void SimulationModel::setHeight	    (QString height	   )   		{	this->height	  = height	  ;	}	
void SimulationModel::setWidth_2	    (QString width_2	   )   		{	this->width_2	      = width_2	  ;	}
void SimulationModel::setHeight_2	    (QString height_2	   )   		{	this->height_2	  = height_2	  ;	}	
void SimulationModel::setWidthOff1    (QString widthoff1 )   		{	this->widthoff1   = widthoff1 ;	}	
void SimulationModel::setHeightOff1   (QString heightoff1)   		{	this->heightoff1  = heightoff1;	}
void SimulationModel::setWidthOff2    (QString widthoff2 )   		{	this->widthoff2   = widthoff2 ;	}
void SimulationModel::setHeightOff2   (QString heightoff2)   		{	this->heightoff2  = heightoff2;	}
                                        		
void SimulationModel::setN1(QString n1)   		                {	this->n1 = n1;			        }
void SimulationModel::setN2(QString n2)   		                {	this->n2 = n2;			        }	
void SimulationModel::setD1(QString d1)   		                {	this->d1 = d1;			        }	
void SimulationModel::setD2(QString d2)   		                {	this->d2 = d2;			        }
void SimulationModel::setCmap(QString cmap)   		            {   this->cmap = cmap;              } 
void SimulationModel::setLegend(QString legend)   		        {   this->legend = legend;          } 
void SimulationModel::setTitulo(QString titulo)   		        {   this->titulo = titulo;          } 
void SimulationModel::setMethod(QString method)	                {	this->method = method; 	        }
void SimulationModel::setModelFile(QString modelFile)	            {	this->modelFile = modelFile; 	}

void SimulationModel::setDistance    (QString distance   )        {   this->distance    =   distance   ; }
void SimulationModel::setDepth       (QString depth      )        {   this->depth       =   depth      ; }
void SimulationModel::setFpeak       (QString fpeak      )        {   this->fpeak       =   fpeak      ; }
void SimulationModel::setFmax        (QString fmax       )        {   this->fmax        =   fmax       ; }
void SimulationModel::setTitle       (QString title      )        {   this->title       =   title      ; }
void SimulationModel::setTMax        (QString tMax       )        {   this->tMax        =   tMax       ; }
void SimulationModel::setLoop        (QString loop       )        {   this->loop        =   loop       ; }
void SimulationModel::setSstrength        ( QString sstrength   )        {   this->sstrength        =   sstrength  ; }
void SimulationModel::setRightBound       ( QString rightBound  )        {   this->rightBound       =   rightBound ; }
void SimulationModel::setLeftBound        ( QString leftBound   )        {   this->leftBound        =   leftBound  ; }
void SimulationModel::setTopBound         ( QString topBound    )        {   this->topBound         =   topBound   ; }
void SimulationModel::setBottomBound      ( QString bottomBound )        {   this->bottomBound      =   bottomBound; }


void SimulationModel::setVelocities(QString velocities)	        {	this->velocities = velocities; 	}
void SimulationModel::setModelsVector(QVector<VelocityModel> models) { this->models = models; }

bool SimulationModel::writeFile(const QString &fileName)
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
	out << qPrintable(QString("%1").arg( title       )) << endl;
	out << qPrintable(QString("%1").arg( tMax        )) << endl;
	out << qPrintable(QString("%1").arg( loop        )) << endl;
	out << qPrintable(QString("%1").arg( sstrength   )) << endl;
	out << qPrintable(QString("%1").arg( velocities  )) << endl;

    out << qPrintable(QString("%1").arg(models.size())) << endl;

    for ( int i = 0; i < models.size(); ++i ) {
        out << models[i].modelName() << " = " 
            << models[i].modelFile() << endl;
    }

    return true;
}

bool SimulationModel::readFile(const QString &fileName)
{
    models.clear();
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
	title               =     in.readLine().simplified();
	tMax                =     in.readLine().simplified();
	loop                =     in.readLine().simplified();
	sstrength           =     in.readLine().simplified();
	velocities			= 	  in.readLine().simplified();

    int num  = in.readLine().simplified().toInt();

    for (int i = 0; i < num ; ++i ) {
        QString item  = in.readLine().simplified();
        QStringList parts = item.split(" = ");
        models.push_back(VelocityModel(parts[0], parts[1]));
    }
	
    return true;
}

void SimulationModel::clear() {
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
   this->titulo        = "Ondas Sismicas P"    ;
   this->method        = "spline"              ;
   this->modelFile     = ""                    ;

   this->distance      = "250"                 ;
   this->depth         = "20"                  ;
   this->fpeak         = "35"                  ;
   this->fmax          = "40"                  ;
   this->title         = "Ondas Sismicas P"    ;
   this->tMax          = "0.40"                ;
   this->loop          = "1"                   ;
   this->sstrength     = "1.0"                 ;
   this->velocities	= ""					   ;

   models.clear();
   models.push_back(VelocityModel("Modelo 1", QDir::currentPath() + "/model.out"));
   models.push_back(VelocityModel("Modelo 2", QDir::currentPath() + "/model2.out"));
   models.push_back(VelocityModel("Modelo 3", QDir::currentPath() + "/model3.out"));
   models.push_back(VelocityModel("Modelo 4", QDir::currentPath() + "/model4.out"));
   models.push_back(VelocityModel("Modelo 5", QDir::currentPath() + "/model5.out"));
}
