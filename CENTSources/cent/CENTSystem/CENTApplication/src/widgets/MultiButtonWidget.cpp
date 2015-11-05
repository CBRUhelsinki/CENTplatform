#include "MultiButtonWidget.h"

#include "connect.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSignalMapper>

namespace
{
	const int BUTTON_WIDTH = 65;
	const int BUTTON_HEIGHT = 60;
}

MultiButtonWidget::MultiButtonWidget(int buttonCount, QWidget* parent)
	: MultiButtonWidgetBase(buttonCount, parent)
	, m_descriptionLabel(0)
	, m_lowScaleLabel(0)
	, m_highScaleLabel(0)
{
	createLayout();
	createConnections();
}

MultiButtonWidget::~MultiButtonWidget()
{
}

void MultiButtonWidget::reset()
{
	MultiButtonWidgetBase::resetSelection();
}

void MultiButtonWidget::triggerValidityCheck()
{
	emitIsValid();
}

void MultiButtonWidget::setLowScaleText(const QString& text)
{
	m_lowScaleLabel->setText(text);
}

void MultiButtonWidget::setHighScaleText(const QString& text)
{
	m_highScaleLabel->setText(text);
}

void MultiButtonWidget::setDescriptionText(const QString& text)
{
	m_descriptionLabel->setText(text);
}

void MultiButtonWidget::createLayout()
{
	m_descriptionLabel = new QLabel();
	m_lowScaleLabel = new QLabel();
	m_highScaleLabel = new QLabel();

	QHBoxLayout* horizontalLayoutButtons = new QHBoxLayout;
	createButtonsLayout(*horizontalLayoutButtons);

	QHBoxLayout* horizontalLayoutScale = new QHBoxLayout;
	horizontalLayoutScale->addWidget(m_lowScaleLabel);
	horizontalLayoutScale->addStretch();
	horizontalLayoutScale->addWidget(m_highScaleLabel);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(m_descriptionLabel);
	mainLayout->addLayout(horizontalLayoutButtons);
	mainLayout->addLayout(horizontalLayoutScale);

	setLayout(mainLayout);
}

void MultiButtonWidget::createConnections()
{
	MultiButtonWidgetBase::createConnections();
}

void MultiButtonWidget::decorateButton(int buttonNumber, QPushButton& button)
{
	button.setMinimumWidth(BUTTON_WIDTH);
	button.setMinimumHeight(BUTTON_HEIGHT);
	button.setText(QString::number(buttonNumber + 1));
}
