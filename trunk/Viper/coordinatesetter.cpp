#include <QtGui>

#include "coordinatesetter.h"
#include "velocitydelegate.h"

VelocitySetter::VelocitySetter(QList<double> *coords,
                                   QWidget *parent)
    : QDialog(parent)
{
    coordinates = coords;

    tableWidget = new QTableWidget(coords->count(), 1);
	tableWidget->setItemDelegate(new VelocityDelegate(0));
    tableWidget->setHorizontalHeaderLabels(
            QStringList() << tr("Velocidades") );

    for (int row = 0; row < coordinates->count(); ++row) {
        double point = coordinates->at(row);
        // addRow();
		
		QTableWidgetItem *item1  = new QTableWidgetItem(QString::number(point));
		item1->setTextAlignment(Qt::AlignRight);
		tableWidget->setItem(row, 0, item1);
    }

	// tableWidget->resizeColumnToContents(0);

    buttonBox = new QDialogButtonBox(Qt::Horizontal);
 	// QPushButton *addRowButton = buttonBox->addButton(tr("&Add Row"),
 	//          QDialogButtonBox::ActionRole);
    buttonBox->addButton(QDialogButtonBox::Ok);
    buttonBox->addButton(QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tableWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Editor de Velocidades"));
}

void VelocitySetter::done(int result)
{
    if (result == QDialog::Accepted) {
		qDebug() << coordinates->size();
        coordinates->clear();
        for (int row = 0; row < tableWidget->rowCount(); ++row) {
            double x = tableWidget->item(row, 0)->text().toDouble();
            coordinates->append(x);
        }
    }
    QDialog::done(result);
}

void VelocitySetter::addRow()
{
    int row = tableWidget->rowCount();

    tableWidget->insertRow(row);

    QTableWidgetItem *item0 = new QTableWidgetItem;
    item0->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    tableWidget->setItem(row, 0, item0);

    tableWidget->setCurrentItem(item0);
}

const QStringList VelocitySetter::velocities() const {

	QStringList vels;
	for (int i = 0; i < coordinates->size(); ++i) {
		vels << QString::number(coordinates->at(i));
		qDebug() << QString::number(coordinates->at(i));
	 }
	return vels;
}
