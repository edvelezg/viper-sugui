#ifndef _EDITDIALOG_H_
#define _EDITDIALOG_H_

#include <QDialog>
#include "ui_editdialog.h"

class ClassWizard;

class EditDialog : public QDialog, public Ui::EditDialog
{
	Q_OBJECT

public:
  EditDialog(QWidget *parent=0);
  
  const QString modelName() const;
  void setModelName( const QString& );

  const QString modelLocation() const;
  void setModelLocation( const QString& );

  const QStringList velocities() const;

public slots:
    void on_browseButton_clicked();
    void setVelocities();

private:
    ClassWizard *wizard;
    QStringList vels;
};

#endif // _EDITDIALOG_H_
