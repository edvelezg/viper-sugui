#include <qobject.h>

class QPoint;
class QCustomEvent;
class QwtPlot;
class QwtPlotCurve;

class CanvasPicker: public QObject
{
    Q_OBJECT

public slots:

public:
    CanvasPicker(QwtPlot *plot);
    virtual bool eventFilter(QObject *, QEvent *);
    bool writeFile(const QString &fileName);
    virtual bool event(QEvent *);

signals:
	void modified();

private:
    void select(const QPoint &);
    void move(const QPoint &);
    void moveBy(int dx, int dy);
    void setCurrentFile(const QString &fileName);

    
    // void release();
    
    void showCursor(bool enable);
    void shiftPointCursor(bool up);
    void shiftCurveCursor(bool up);

    QwtPlot *plot() { return (QwtPlot *)parent(); }
    const QwtPlot *plot() const { return (QwtPlot *)parent(); }
    
    QwtPlotCurve *d_selectedCurve;
    int d_selectedPoint;
	QString curFile;
};
