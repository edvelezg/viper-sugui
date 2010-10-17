#ifndef COORDINATESETTER_H
#define COORDINATESETTER_H

#include <QDialog>
#include <QList>
#include <QPointF>

class QDialogButtonBox;
class QTableWidget;

class VelocitySetter : public QDialog
{
    Q_OBJECT

public:
    VelocitySetter(QList<double> *coords, QWidget *parent = 0);
    void done(int result);
    const QStringList velocities() const;

  private slots:
      void addRow();

  private:
      QTableWidget *tableWidget;
      QDialogButtonBox *buttonBox;
      QList<double> *coordinates;
    };

#endif
