#include "UiHelpers.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVariant>
#include <QVBoxLayout>
#include <QWidget>

namespace
{
	const char* FOOTER_PROPERTY = "FooterButtonWidget";

	const int BOUNDING_WIDGET_STRETCH_FACTOR = 10;
}

bool UiHelpers::createButtonFooter(QWidget& footer, QList<QWidget*> list, ButtonLayout f)
{
	Q_ASSERT(list.count() != 0);
	if(list.count() == 0)
	{
		qCritical() << __FUNCTION__ << "Invalid arguments";
		return false;
	}

	QHBoxLayout* hLayout = new QHBoxLayout;
	if(f == AllRight || f == CenterWidgets)
	{
		hLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding));
		for(int i = 0; i < list.count(); i++)
		{
			Q_ASSERT(list[i]);
			hLayout->addWidget(list[i]);
			list[i]->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
		}
		if(f == CenterWidgets)
		{
			hLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding));
		}
	}
	else if (f == LastRightAndRestCenter)
	{
		QWidget* w = new QWidget;
		w->setMinimumWidth(list.last()->width());
		hLayout->addWidget(w);
		hLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding));
		for(int i = 0; i < list.count() - 1; i++)
		{
			Q_ASSERT(list[i]);
			hLayout->addWidget(list[i]);
		}
		Q_ASSERT(list.last());
		hLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding));
		hLayout->addWidget(list.last());
	}
	else if(f == FirstLeftAndRestRight)
	{
		Q_ASSERT(list.first());
		hLayout->addWidget(list.first());
		hLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding));
		for(int i = 1; i < list.count(); i++)
		{
			Q_ASSERT(list[i]);
			hLayout->addWidget(list[i]);
			list[i]->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
		}
	}
	else
	{
		delete hLayout;
		return false;
	}

	QVBoxLayout* vLayout = new QVBoxLayout;
	vLayout->addStretch();
	vLayout->addLayout(hLayout);
	vLayout->addStretch();

	vLayout->setContentsMargins(8, 8, 8, 0);
	footer.setLayout(vLayout);
	footer.setProperty(FOOTER_PROPERTY, true);
	return true;
}
