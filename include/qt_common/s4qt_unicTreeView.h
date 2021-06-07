#pragma once

#include <QTreeView>

namespace S4
{
namespace QT
{
    
class unicTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit unicTreeView(QWidget *parent = nullptr);

public slots:

    void onTreeDoubleClick(const QModelIndex& index);

signals:
    void signal_mouseDoubleClick();
    void signal_treeDoubleClick(const QModelIndex& index);
    void signal_newTree(const QString&);
    void signal_delTree(const QString&);
protected:
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
	
};




} // namespace QT
} // namespace S4
