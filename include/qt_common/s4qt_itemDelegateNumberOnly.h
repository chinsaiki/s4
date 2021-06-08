#include <QItemDelegate>
#include <QLineEdit>

namespace S4{
namespace QT{

class itemDelegateNumberOnly : public QItemDelegate
{
public:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem & option,
                      const QModelIndex & index) const
    {
        QLineEdit *lineEdit = new QLineEdit(parent);

        // Set your validator, such as 'only number between 0 ~ 9 )
        QIntValidator *validator = new QIntValidator(0, 999999, lineEdit);
        lineEdit->setValidator(validator);

        return lineEdit;
    }
};


}
}