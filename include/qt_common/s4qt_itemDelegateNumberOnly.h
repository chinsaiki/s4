#include <QItemDelegate>
#include <QLineEdit>

namespace S4{
namespace QT{

class itemDelegateNumberOnly : public QItemDelegate
{
public:
    itemDelegateNumberOnly(int bottom, int top, QWidget *parent):
        QItemDelegate(parent),
        _bottom(bottom),
        _top(top)
    {}
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem & option,
                      const QModelIndex & index) const
    {
        QLineEdit *lineEdit = new QLineEdit(parent);

        // Set your validator, such as 'only number between 0 ~ 9 )
        QIntValidator *validator = new QIntValidator(_bottom, _top, lineEdit);
        lineEdit->setValidator(validator);

        return lineEdit;
    }
private:
    int _bottom;
    int _top;
};


}
}