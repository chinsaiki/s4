#include <QItemDelegate>
#include <QLineEdit>
#include <QIntValidator>

namespace S4{
namespace QT{

class numberValidator: public QIntValidator
{
public:
	explicit numberValidator(QObject * parent = 0)
		:QIntValidator(parent)
	{
 
	}
 
	numberValidator(int bottom, int top, QObject *parent = 0):QIntValidator(bottom, top, parent)
	{
	}
 
	~numberValidator() 
	{
	}
 
	virtual State validate(QString &str, int &i) const
	{
		if (str.isEmpty())
		{
			return QValidator::Intermediate;
		}
		bool cOK = false;
		int val = str.toInt(&cOK);
 
		if (!cOK)
		{
			return QValidator::Invalid;
		}
 
		if (val >= bottom() && val <= top())
		{
			return QValidator::Acceptable;
		}
 
		return QValidator::Invalid;
	}
 
	virtual void fixup(QString &s) const override
	{
		if( s.toInt() < bottom() )
		{
			s = QString::number( bottom() );
		}
		else if ( s.toInt() > top() )
		{
			s = QString::number( top() );
		}
	}
};

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
        QIntValidator *validator = new numberValidator(_bottom, _top, lineEdit);
        lineEdit->setValidator(validator);

        return lineEdit;
    }
private:
    int _bottom;
    int _top;
};


}
}