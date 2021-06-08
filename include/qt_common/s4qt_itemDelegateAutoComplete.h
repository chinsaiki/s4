#include <QStyledItemDelegate>
#include <QStringListModel>
#include <QLineEdit>
#include <QCompleter>
#include <QLineEdit>

namespace S4{
namespace QT{


enum CustomRoles{
    itemDelegateAutoCompleterRole = Qt::UserRole + 1001
};

class itemDelegateAutoComplete: public QStyledItemDelegate{
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{
        QWidget* editor = QStyledItemDelegate::createEditor(parent, option, index);
        if(QLineEdit *le = qobject_cast<QLineEdit *>(editor)){
            QStringListModel *model = new QStringListModel(le);
            QCompleter *completer = new QCompleter(le);
            completer->setModel(model);
            le->setCompleter(completer);
        }
        return editor;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const{
        QStyledItemDelegate::setEditorData(editor, index);
        if(QLineEdit *le = qobject_cast<QLineEdit *>(editor)){
            if(QCompleter *completer = le->completer()){
                if(QStringListModel *model = qobject_cast<QStringListModel *>(completer->model())){
                    QVariant v = index.data(CustomRoles::itemDelegateAutoCompleterRole);
                    if (v.canConvert<QStringList>()){
                        QStringList options = v.value<QStringList>();
                        model->setStringList(options);
                    }
                }
            }
        }
    }
};



}
}