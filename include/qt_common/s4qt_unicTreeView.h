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

    int findChild(const QString&);
public slots:
    void onSetTextFormater(textFormater_t*);
    void onItemChanged(QWidget* editor, int hint);

	void onDelkey();
	void onMouseClick(const QModelIndex& index);
    void onBlankDoubleClick(); 
    void onSetCurrentRoot(QStandardItem* );

    void onDelItem(const QString&);
signals:
    void signal_selectItem(const QString&);
	void signal_blankDoubleClick();
    void signal_newItem(const QString&);
    void signal_delItem(const QString&);
protected:
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
	QStandardItem* _current_root;
    textFormater_t* _textFormater = nullptr;
};




} // namespace QT
} // namespace S4
