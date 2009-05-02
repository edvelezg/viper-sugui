#ifndef LISTDIALOG_H
#define LISTDIALOG_H

#include <QDialog>
#include "ui_listdialog.h"
#include "velocitymodel.h"

class VelocitySetter;

class ListDialog : public QDialog
{
  Q_OBJECT
  
public:
  ListDialog( QWidget *parent = 0 );
  const QString currentLocation() const;

  const QStringList velocities() const;
  const QVector<VelocityModel> getModels() const;

  void setVelocities( const QStringList& );
  void setVelocities( QString );
  void setModels( const QVector<VelocityModel>& );

  
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
