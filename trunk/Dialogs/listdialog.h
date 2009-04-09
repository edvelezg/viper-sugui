#ifndef LISTDIALOG_H
#define LISTDIALOG_H

#include <QDialog>
#include <QStringList>
#include "ui_listdialog.h"

class ListDialog : public QDialog
{
  Q_OBJECT
  
public:
  ListDialog( QWidget *parent = 0 );
  const QString currentLocation() const;
  const QStringList velocities() const;
  void setVelocities(QString vels);

  
private slots:
  void addItem();
  void editItem();
  void deleteItem();
  void modelChanged();

private:
  Ui::ListDialog ui;
  QString modelFile;
  QStringList vels;
};

#endif // LISTDIALOG_H
