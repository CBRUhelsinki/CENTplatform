#include "MultiButtonWidgetBase.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QVariant>
#include <QSignalMapper>

#include "connect.h"

namespace
{
	const char* BUTTON_GROUP_NAME = "ButtonsGroupName";
	const char* BUTTON_GROUP_MEM  = "isInButtonsGroup";
}

const int MultiButtonWidgetBase::NOT_SELECTED = -1;

MultiButtonWidgetBase::MultiButtonWidgetBase(int buttonCount, QWidget* parent)
	: QWidget(parent)
	, m_buttonCount(buttonCount)
	, m_signalMap(new QSignalMapper(this))
{
	m_selection = NOT_SELECTED;
}

MultiButtonWidgetBase::~MultiButtonWidgetBase()
{
}

void MultiButtonWidgetBase::setSelection(int sel)
{
	if ((sel < 0 || sel > m_buttons.size() || sel == m_selection) && sel != NOT_SELECTED)
	{
		return;
	}

	if(m_selection != NOT_SELECTED)
	{
		m_buttons[m_selection]->setChecked(false);
	}

	m_selection = sel;

	if(m_selection != NOT_SELECTED)
	{
		m_buttons[m_selection]->setChecked(true);
		emit isValid(true);
	}
	else
	{
		emit isValid(false);
	}

	emit selectionChanged(m_selection);
}

int MultiButtonWidgetBase::selection()
{
	return m_selection;
}

void MultiButtonWidgetBase::resetSelection()
{
	if(m_selection != NOT_SELECTED && m_selection < m_buttons.size())
	{
		m_buttons[m_selection]->setChecked(false);
	}
	m_selection = NOT_SELECTED;
	emit isValid(false);
	emit selectionChanged(m_selection);
}

void MultiButtonWidgetBase::emitIsValid()
{
	if(m_selection != NOT_SELECTED)
	{
		emit isValid(true);
	}
	else
	{
		emit isValid(false);
	}
}

void MultiButtonWidgetBase::onButtonClicked(int num)
{
	if (num < 0 || num > m_buttons.size())
	{
		return;
	}

	if(m_buttons[num]->isChecked())
	{
		if(m_selection != NOT_SELECTED)
		{
			m_buttons[m_selection]->setChecked(false);
		}

		m_selection = num;
		emit isValid(true);
	}
	else
	{
		m_selection = NOT_SELECTED;
		emit isValid(false);
	}

	emit buttonClicked();
	emit selectionChanged(m_selection);
}

void MultiButtonWidgetBase::createConnections()
{
	for (int i = 0; i < m_buttons.size(); i++)
	{
		m_signalMap->setMapping(m_buttons[i], i);
		CENT::connect(m_buttons[i], SIGNAL(clicked()), m_signalMap, SLOT(map()));
	}
	CENT::connect(m_signalMap, SIGNAL(mapped(int)), this, SLOT(onButtonClicked(int)));
}

void MultiButtonWidgetBase::createButtonsLayout(QHBoxLayout& buttonsLayout)
{
	QPushButton* button;
	for (int i=0; i < m_buttonCount; i++)
	{
		button = new QPushButton;
		button->setCheckable(true);
		button->setProperty(BUTTON_GROUP_MEM, true);

		decorateButton(i, *button);

		buttonsLayout.addWidget(button);
		m_buttons.append(button);
	}
}

void MultiButtonWidgetBase::setButtonNames(const QStringList& names)
{
	if(names.count() != m_buttonCount && m_buttonCount != m_buttons.count())
	{
		return;
	}
	for(int i = 0; i < names.count(); i++)
	{
		m_buttons[i]->setObjectName(names[i]);
	}
}

void MultiButtonWidgetBase::setButtonsGroupProperty(const QString& name)
{
	foreach(QPushButton* button, m_buttons)
	{
		button->setProperty(BUTTON_GROUP_NAME, name);
	}
}

void MultiButtonWidgetBase::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);

	for(int i = 0; i < m_buttons.count(); i++)
	{
		if(m_buttons[i]->width() != m_buttons[i]->height())
		{
			m_buttons[i]->setMinimumHeight(m_buttons[i]->width());
			m_buttons[i]->setMaximumHeight(m_buttons[i]->width());
		}
	}
}

