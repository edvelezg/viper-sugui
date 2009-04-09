#ifndef LISTDIALOG_H
#define LISTDIALOG_H

#include <QDialog>
#include "ui_listdialog.h"
#include "velocitymodel.h"

class CoordinateSetter;

class ListDialog : public QDialog
{
  Q_OBJECT
  
public:
  ListDialog( QWidget *parent = 0 );
  const QString currentLocation() const;

  const QStringList velocities() const;
  void setVelocities( const QStringList& );
  void setVelocities( QString );

  
private slots:
  void addItem();
  void editItem();
  void deleteItem();
  void modelChanged();
  void setVelocities();

private:
  Ui::ListDialog ui;
  QMap<QString, VelocityModel> vmMap;
  QStringList mVels;
  QString modelFile;
};

#endif // LISTDIALOG_H
