#include "AboutWidget.h"

#include <QLabel>
#include <QStyleOption>
#include <QStylePainter>
#include <QVBoxLayout>

AboutWidget::AboutWidget(const QString& content, QWidget* parent)
	: QWidget(parent)
{
	QVBoxLayout* layout = new QVBoxLayout;
	QLabel* label = new QLabel(content);
	label->setWordWrap(true);
	label->setTextFormat(Qt::RichText);
	label->setOpenExternalLinks(true);
	layout->addWidget(label);
	setLayout(layout);
}

AboutWidget::~AboutWidget()
{
}

void AboutWidget::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;
	opt.init(this);
	QStylePainter p(this);
	p.drawPrimitive(QStyle::PE_Widget, opt);
	QWidget::paintEvent(event);
}
