#include <qwt_plot.h>
#include <QtDebug>
#include <QVector>
#include <QPair>


class ColorBar;
class QwtWheel;

class Plot: public QwtPlot
{
    Q_OBJECT
public:
	Plot (QWidget *parent = NULL);
    Plot(int, QVector<int> &numPoints, QWidget *parent = NULL);
    // virtual bool eventFilter(QObject *, QEvent *);

public slots:
    void setCanvasColor(const QColor &);
    void insertCurve(int axis, double base);
	void insertCurve(const QColor &, QVector<QPair<double, double> >);


private slots:
    // void scrollLeftAxis(double);

private:
    void insertCurve(Qt::Orientation, const QColor &, double base, int);
    // 
    // ColorBar *d_colorBar;
    // QwtWheel *d_wheel;
};
