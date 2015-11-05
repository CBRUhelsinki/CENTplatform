#include "SimplePage.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QLabel>
#include <QPushButton>
#include <QSignalMapper>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

#include "Connect.h"
#include "UiHelpers.h"

namespace
{
	const int BUTTON_COUNT = 4;
}

SimplePage::SimplePage(const QString& title, QWidget* parent)
	: ICentPage(parent)
	, m_title(title)
	, m_buttonsWidget(0)
	, m_contentBrowser(0)
	, m_topWidget(0)
	, m_bottomWidget(0)
{
	createLayout();
}

SimplePage::~SimplePage()
{
}

void SimplePage::setContent(const QString& content, Qt::Alignment alignment)
{
	m_topWidget->setVisible(alignment != Qt::AlignTop && alignment != Qt::AlignJustify);
	m_bottomWidget->setVisible(alignment != Qt::AlignBottom && alignment != Qt::AlignJustify);

	m_contentBrowser->setHtml(content);

	// Size hint bug fix.
	if(alignment != Qt::AlignJustify)
	{
		int height = countContentHeitht();
		m_contentBrowser->setMaximumHeight(height);
		m_contentBrowser->setMinimumHeight(height);
	}
	else
	{
		m_contentBrowser->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding)); //should be always full screen
	}
}

int SimplePage::countContentHeitht()
{
	QTextDocument* doc = m_contentBrowser->document();
	doc->setTextWidth(m_contentBrowser->width());
	return doc->size().toSize().height();
}

QString SimplePage::content()
{
	return m_contentBrowser->toHtml();
}

void SimplePage::setBtnVisible(int index, bool visible)
{
	if(index >= BUTTON_COUNT || index < 0)
	{
		return;
	}

	m_buttons[index]->setVisible(visible);

	bool b = false;
	for(int i = 0; i < m_buttons.count(); i++)
	{
		if(i == index)
		{
			b = b || visible;
		}
		else
		{
			b = b || m_buttons[i]->isVisible();
		}
	}
	m_buttonsWidget->setVisible(b);
}

void SimplePage::setBtnText(int index, const QString& text)
{
	if(index >= BUTTON_COUNT || index < 0)
	{
		return;
	}

	m_buttons[index]->setText(text);
}

bool SimplePage::btnVisible(int index)
{
	if(index >= BUTTON_COUNT || index < 0)
	{
		return false;
	}

	return m_buttons[index]->isVisible();
}

QString SimplePage::btnText(int index)
{
	if(index >= BUTTON_COUNT || index < 0)
	{
		return "";
	}

	return m_buttons[index]->text();
}

QString SimplePage::headerText()
{
	return m_title;
}

void SimplePage::setFooterText(const QString& text)
{
	m_footerLabel->setText(text);
}

void SimplePage::createLayout()
{
	// Content QTextBrowser
	m_contentBrowser = new QTextBrowser();

	m_contentBrowser->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	m_contentBrowser->setMinimumHeight(0);
	m_contentBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_contentBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_contentBrowser->setFrameStyle(QFrame::NoFrame);
	m_contentBrowser->setContextMenuPolicy(Qt::NoContextMenu);

	// Set background color same as window color
	QPalette pal = m_contentBrowser->palette();
	pal.setColor(QPalette::Base, pal.color(QPalette::Window));
	m_contentBrowser->setPalette(pal);

	// Spacer Widgets
	m_topWidget = new QWidget();
	m_bottomWidget = new QWidget();

	m_topWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_bottomWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	// Main layout
	QVBoxLayout* layout = new QVBoxLayout();

	layout->addWidget(m_topWidget);
	layout->addWidget(m_contentBrowser);
	layout->addWidget(m_bottomWidget);

	// Footer Label
	m_footerLabel = new QLabel;
	m_footerLabel->setWordWrap(true);

	// Bottom buttons
	m_buttonsWidget = new QWidget();
	QPushButton* button;
	QSignalMapper* mapButtons = new QSignalMapper(this);

	QList<QWidget*> tempList;
	tempList << m_footerLabel;
	for(int i =0; i < BUTTON_COUNT; i++)
	{
		button = new QPushButton();
		CENT::connect(button, SIGNAL(clicked()), mapButtons, SLOT(map()));

		button->setVisible(false);
		button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
		m_buttons.append(button);
		tempList.append(button);

		mapButtons->setMapping(button, i);
	}

	// Map buttons to int
	CENT::connect(mapButtons, SIGNAL(mapped(int)), this, SIGNAL(buttonClicked(int)));
	bool b = UiHelpers::createButtonFooter(*m_buttonsWidget, tempList, UiHelpers::FirstLeftAndRestRight);
	Q_ASSERT(b); Q_UNUSED(b);

	m_buttonsWidget->setVisible(false);

	layout->addWidget(m_buttonsWidget);
	layout->setContentsMargins(0, 0, 0, 0);

	setLayout(layout);
}
