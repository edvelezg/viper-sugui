#include "plot.h"
#include <qevent.h>
#if QT_VERSION < 0x040000
#include <qwhatsthis.h>
#endif
#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_scale_widget.h>
#include <qwt_wheel.h>
#include <stdlib.h>

Plot::Plot( int numCurves, QVector<int> &numPoints, QWidget *parent ):
    QwtPlot(parent)
{
    setTitle("Modelo de Velocidad");

    setCanvasColor(Qt::gray);

    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->setMajPen(QPen(Qt::white, 0, Qt::DotLine));
    grid->attach(this);

    // axes

    setAxisScale(QwtPlot::xBottom, 0.0, 1000.0);
    setAxisScale(QwtPlot::yLeft, 0.0, 1000.0);

    // Avoid jumping when label with 3 digits
//     // appear/disappear when scrolling vertically
// 
    QwtScaleDraw *sd = axisScaleDraw(QwtPlot::yLeft);
    sd->setMinimumExtent( sd->extent(QPen(), 
        axisWidget(QwtPlot::yLeft)->font()));

    plotLayout()->setAlignCanvasToScales(true);

	double inc = 1000.0 / numCurves;
	
	for (int i = (numCurves - 1); i >= 0; --i) {
		double v = (2*i*inc + inc)/ 2.0;
		insertCurve(Qt::Horizontal, Qt::blue, v, numPoints[i]);
	}
	    
    replot();
}

Plot::Plot( QWidget *parent ):
    QwtPlot(parent)
{
    setTitle("Modelo de Velocidad");

    setCanvasColor(Qt::gray);

    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->setMajPen(QPen(Qt::white, 0, Qt::DotLine));
    grid->attach(this);

    // axes

    setAxisScale(QwtPlot::xBottom, 0.0, 1000.0);
    setAxisScale(QwtPlot::yLeft, 0.0, 1000.0);

    // Avoid jumping when label with 3 digits
//     // appear/disappear when scrolling vertically
// 
    QwtScaleDraw *sd = axisScaleDraw(QwtPlot::yLeft);
    sd->setMinimumExtent( sd->extent(QPen(), 
        axisWidget(QwtPlot::yLeft)->font()));

    plotLayout()->setAlignCanvasToScales(true);

    replot();
}

void Plot::setCanvasColor(const QColor &c)
{
    setCanvasBackground(c);
    replot();
}

void Plot::insertCurve(int axis, double base)
{
    Qt::Orientation o;
    if ( axis == yLeft || axis == yRight )
        o = Qt::Horizontal;
    else
        o = Qt::Vertical;
    
    QRgb rgb = (uint)rand();
    insertCurve(o, QColor(rgb), base, 10);
    replot();
}

void Plot::insertCurve(Qt::Orientation o,
    const QColor &c, double base, int numPoints)
{
    QwtPlotCurve *curve = new QwtPlotCurve();

    curve->setPen(c);
	
	// Forma de las bolitas
    curve->setSymbol(QwtSymbol(QwtSymbol::Ellipse,
        Qt::gray, c, QSize(5, 5)));

    double x[numPoints];
    double y[sizeof(x) / sizeof(x[0])];

	double incr = 1000.0/numPoints;

    for ( uint i = 0; i < sizeof(x) / sizeof(x[0]); i++ )
    {
        double v = (2*i*incr + incr)/ 2.0;
        if ( o == Qt::Horizontal )
        {
            x[i] = v;
            y[i] = base;
        }
        else
        {
            x[i] = base;
            y[i] = v;
        }
    }
        
    curve->setData(x, y, sizeof(x) / sizeof(x[0]));
    curve->attach(this);
}

void Plot::insertCurve(const QColor &c, QVector<QPair<double, double> > myPoints)
{
	    QwtPlotCurve *curve = new QwtPlotCurve();
	
	    curve->setPen(c);
	
	// Forma de las bolitas
	    curve->setSymbol(QwtSymbol(QwtSymbol::Ellipse,
	        Qt::gray, c, QSize(5, 5)));
	
		double x[myPoints.size()];
	    double y[sizeof(x) / sizeof(x[0])];

	    for ( uint i = 0; i < sizeof(x) / sizeof(x[0]); i++ )
	    {
			x[i] = myPoints[i].first;
			// qDebug() << myPoints[i].first << " , " << myPoints[i].second;
	        y[i] = 1000 - myPoints[i].second;
	    }
	        
	    curve->setData(x, y, sizeof(x) / sizeof(x[0]));
	    curve->attach(this);
}
