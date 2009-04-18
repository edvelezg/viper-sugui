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

#include <QtDebug>

Plot::Plot( int numCurves, int numPoints, QWidget *parent ):
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

	double inc = 800.0 / numCurves;
	
	for (int i = (numCurves - 1); i >= 0; --i) {
		insertCurve(Qt::Horizontal, Qt::blue, (i+1)*inc, numPoints);
	}
	    
// 
//     insertCurve(Qt::Horizontal, Qt::blue, 750.0);
// //     // insertCurve(Qt::Vertical, Qt::magenta, 70.0);
//     insertCurve(Qt::Horizontal, Qt::yellow, 500.0);
//     insertCurve(Qt::Horizontal, Qt::white, 70.0);
// 
    replot();
// 
//     // ------------------------------------
//     // We add a color bar to the left axis
//     // ------------------------------------
// 
//     QwtScaleWidget *scaleWidget = (QwtScaleWidget *)axisWidget(yLeft);
//     scaleWidget->setMargin(10); // area for the color bar
//     d_colorBar = new ColorBar(Qt::Vertical, scaleWidget);
//     d_colorBar->setRange(Qt::red, Qt::darkBlue);
// #if QT_VERSION >= 0x040000
//     d_colorBar->setFocusPolicy( Qt::TabFocus  );
// #else
//     d_colorBar->setFocusPolicy( QWidget::TabFocus  );
// #endif
// 
//     connect(d_colorBar, SIGNAL(selected(const QColor &)),
//         SLOT(setCanvasColor(const QColor &)));
// 
//     // we need the resize events, to lay out the color bar
//     scaleWidget->installEventFilter(this); 
// 
//     // ------------------------------------
//     // We add a wheel to the canvas
//     // ------------------------------------
// 
//     d_wheel = new QwtWheel(canvas());
//     d_wheel->setOrientation(Qt::Vertical);
//     d_wheel->setRange(-100, 100);
//     d_wheel->setValue(0.0);
//     d_wheel->setMass(0.2);
//     d_wheel->setTotalAngle(4 * 360.0);
// 
//     connect(d_wheel, SIGNAL(valueChanged(double)),
//         SLOT(scrollLeftAxis(double)));
// 
//     // we need the resize events, to lay out the wheel
//     canvas()->installEventFilter(this);
// 
// #if QT_VERSION < 0x040000
//     QWhatsThis::add(d_colorBar, 
//         "Selecting a color will change the background of the plot.");
//     QWhatsThis::add(scaleWidget, 
//         "Selecting a value at the scale will insert a new curve.");
//     QWhatsThis::add(d_wheel, 
//         "With the wheel you can move the visible area.");
//     QWhatsThis::add(axisWidget(xBottom), 
//         "Selecting a value at the scale will insert a new curve.");
// #else
//     d_colorBar->setWhatsThis(
//         "Selecting a color will change the background of the plot.");
//     scaleWidget->setWhatsThis(
//         "Selecting a value at the scale will insert a new curve.");
//     d_wheel->setWhatsThis(
//         "With the wheel you can move the visible area.");
//     axisWidget(xBottom)->setWhatsThis(
//         "Selecting a value at the scale will insert a new curve.");
// #endif

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
