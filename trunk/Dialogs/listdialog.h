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
  
private slots:
  void addItem();
  void editItem();
  void deleteItem();
  void enableButton();

private:
  Ui::ListDialog ui;
  QString modelFile;
  QStringList vels;
};

#endif // LISTDIALOG_H
