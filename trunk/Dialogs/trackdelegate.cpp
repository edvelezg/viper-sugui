#include <QtGui>

#include "trackdelegate.h"

TrackDelegate::TrackDelegate(int durationColumn, QObject *parent)
    : QItemDelegate(parent)
{
    this->durationColumn = durationColumn;
}

void TrackDelegate::paint(QPainter *painter,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    if (index.column() == durationColumn) {
        double secs = index.model()->data(index, Qt::DisplayRole).toDouble();
        QString text = QString("%1")
                       .arg(secs);

        QStyleOptionViewItem myOption = option;
        myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;

        drawDisplay(painter, myOption, myOption.rect, text);
        drawFocus(painter, myOption, myOption.rect);
    } else{
        QItemDelegate::paint(painter, option, index);
    }
}

QWidget *TrackDelegate::createEditor(QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    if (index.column() == durationColumn) {
        QDoubleSpinBox *timeEdit = new QDoubleSpinBox(parent);
        timeEdit->setMinimum(1500.00);
        timeEdit->setMaximum(10000.00);
        connect(timeEdit, SIGNAL(editingFinished()),
                this, SLOT(commitAndCloseEditor()));
        return timeEdit;
    } else {
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void TrackDelegate::setEditorData(QWidget *editor,
                                  const QModelIndex &index) const
{
    if (index.column() == durationColumn) {
        int secs = index.model()->data(index, Qt::DisplayRole).toInt();
        QDoubleSpinBox *timeEdit = qobject_cast<QDoubleSpinBox *>(editor);
        timeEdit->setValue(secs);
    } else {
        QItemDelegate::setEditorData(editor, index);
    }
}

void TrackDelegate::setModelData(QWidget *editor,
                                 QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    if (index.column() == durationColumn) {
        QDoubleSpinBox *timeEdit = qobject_cast<QDoubleSpinBox *>(editor);
//      QTime time = timeEdit->time();
//      int secs = (time.minute() * 60) + time.second();
        double secs = timeEdit->value();
        model->setData(index, secs);
    } else {
        QItemDelegate::setModelData(editor, model, index);
    }
}

void TrackDelegate::commitAndCloseEditor()
{
    QSpinBox *editor = qobject_cast<QSpinBox *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
