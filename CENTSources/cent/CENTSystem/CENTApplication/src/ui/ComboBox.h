#ifndef COMBO_BOX_H
#define COMBO_BOX_H

#include <QComboBox>

// Todo:: it might be needed to add signals like
// currentIdChanged()

class ComboBox: public QComboBox
{
public:
	explicit ComboBox(QWidget* parent = 0);
	virtual ~ComboBox();

public:
	void setCurrentItemById(const int id);
	int currentId();
};

#endif // COMBO_BOX_H

