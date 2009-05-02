#ifndef TRACKDELEGATE_H
#define TRACKDELEGATE_H

#include <QItemDelegate>

class VelocityDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    VelocityDelegate(int durationColumn, QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

private slots:
    void commitAndCloseEditor();

private:
    int durationColumn;
};

#endif

