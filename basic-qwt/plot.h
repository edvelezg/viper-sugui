#include <qwt_plot.h>

class ColorBar;
class QwtWheel;

class Plot: public QwtPlot
{
    Q_OBJECT
public:
    Plot(int, int, QWidget *parent = NULL);
    // virtual bool eventFilter(QObject *, QEvent *);

public slots:
    void setCanvasColor(const QColor &);
    void insertCurve(int axis, double base);

private slots:
    // void scrollLeftAxis(double);

private:
    void insertCurve(Qt::Orientation, const QColor &, double base, int);
    // 
    // ColorBar *d_colorBar;
    // QwtWheel *d_wheel;
};