#pragma once

#include <QTreeView>
#include <QStandardItem>

namespace S4
{
namespace QT
{
    
class unicTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit unicTreeView(QWidget *parent = nullptr);

    typedef bool (textFormater_t)(const QString& raw_code, QString& mktCode);

public slots:
    void onSetTextFormater(textFormater_t*);
    void onItemChanged(QStandardItem *item);

    void onTreeDoubleClick(const QModelIndex& index);
    void onBlankDoubleClick();
    void onSetCurrentRoot(QStandardItem* );
signals:
    void signal_mouseDoubleClick();
    void signal_treeDoubleClick(const QModelIndex& index);
    void signal_newItem(const QString&);
    void signal_delItem(const QString&);
protected:
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
	QStandardItem* _current_root;
    textFormater_t* _textFormater = nullptr;
};




} // namespace QT
} // namespace S4
