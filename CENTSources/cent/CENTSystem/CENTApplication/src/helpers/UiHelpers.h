#ifndef UI_HELPERS_H
#define UI_HELPERS_H

#include <QList>

class QPushButton;
class QWidget;

class UiHelpers
{
public:
	enum ButtonLayout
	{
		AllRight,
		LastRightAndRestCenter,
		CenterWidgets,
		FirstLeftAndRestRight
	};
	static bool createButtonFooter(QWidget& footer, QList<QWidget*> list, ButtonLayout f);

};

#endif // UI_HELPERS_H