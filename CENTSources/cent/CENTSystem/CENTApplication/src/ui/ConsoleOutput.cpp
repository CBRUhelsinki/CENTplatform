#include "ConsoleOutput.h"

#include <QTabWidget>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QPushButton>

#include "Connect.h"

namespace
{
	const static QMargins DEFAULT_MARGINS(4, 4, 4, 4);
	const static int DEFAULT_SPACING = 4;
}

ConsoleOutput::ConsoleOutput(QWidget* parent)
	: QWidget(0/* cannot have parent because it will becode topmost*/, Qt::Dialog)
	, m_parent(parent)
{
	setWindowTitle(tr("Console Output"));

	m_tabs = new QTabWidget(this);
	QLayout* layout = new QVBoxLayout();
	layout->addWidget(m_tabs);
	layout->setContentsMargins(DEFAULT_MARGINS);
	layout->setSpacing(DEFAULT_SPACING);
	setLayout(layout);

	if (m_parent)
	{
		move(m_parent->x(), m_parent->frameGeometry().top());
	}
}

void ConsoleOutput::print(QString msg, QString programTitle)
{
	QTextBrowser* browser = tab(programTitle);
	browser->append(msg);
}

QTextBrowser* ConsoleOutput::tab(const QString& title)
{
	QTextBrowser* browser = m_browsers.value(title);
	if (! browser)
	{
		// Create browser
		browser = new QTextBrowser();
		m_browsers.insert(title, browser);

		// Create clear button
		QPushButton* clear = new QPushButton(tr("Clear"));
		CENT::connect(clear, SIGNAL(clicked()), browser, SLOT(clear()));

		// Configure layout and add new tab
		QVBoxLayout* layout= new QVBoxLayout;
		layout->setContentsMargins(DEFAULT_MARGINS);
		layout->setSpacing(DEFAULT_SPACING);
		layout->addWidget(browser);
		layout->addWidget(clear, 0, Qt::AlignRight);
		QWidget* tabWidget = new QWidget;
		tabWidget->setLayout(layout);
		m_tabs->addTab(tabWidget, title);
	}
	return browser;
}

QSize ConsoleOutput::sizeHint() const
{
	QSize s = QWidget::sizeHint();
	if (m_parent)
	{
		s.setWidth(m_parent->width());
	}
	return s;
}
