#include "ComboBox.h"

#include <QDebug>

ComboBox::ComboBox(QWidget* parent)
	: QComboBox(parent)
{
}

ComboBox::~ComboBox()
{
}

void ComboBox::setCurrentItemById(const int id)
{
	const int size = count();

	for (int i = 0; i < size; ++i)
	{
		if (id == itemData(i).toInt())
		{
			setCurrentIndex(i);
			return;
		}
	}

	qDebug() << __FUNCTION__ << "Cannot find id: " << id;
}

int ComboBox::currentId()
{
	return itemData(currentIndex()).toInt();
}

