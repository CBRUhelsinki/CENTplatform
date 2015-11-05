#include "CentMainWindow.h"
#include "Connect.h"

#include <QFrame>
#include <QMenu>
#include <QMenuBar>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QStatusBar>
#include <QVBoxLayout>

namespace
{
    const QSize INITIAL_SIZE(960, 600);
}
	
CentMainWindow::CentMainWindow(const QString& title, QWidget* parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
	, m_title(title)
	, m_headerLabel(0)
	, m_stackedWidget(new QStackedWidget(this))
{
	resize(INITIAL_SIZE);
	setMinimumSize(INITIAL_SIZE);

	createMenu();
	createLayout();
}

CentMainWindow::~CentMainWindow()
{
}

bool CentMainWindow::addPage(QWidget* page)
{
	if (page)
	{
		m_stackedWidget->addWidget(page);
		return true;
	}

	return false;
}

bool CentMainWindow::activatePage(QWidget* page)
{
	if (page)
	{
		m_stackedWidget->setCurrentWidget(page);
		return true;
	}
	
	return false;
}

QString CentMainWindow::headerText()
{
	return m_headerLabel->text();
}

void CentMainWindow::setHeaderText(const QString& text)
{
	m_headerLabel->setText(text);
}

bool CentMainWindow::headerVisible()
{
	return m_headerLabel->isVisible();
}

void CentMainWindow::setHeaderVisible(bool visible)
{
	m_headerLabel->setVisible(visible);
}

void CentMainWindow::createLayout()
{
	QWidget* centralWidget = new QWidget(this);
	centralWidget->setObjectName("centralWidget");
	QVBoxLayout* centralWidgetLayout = new QVBoxLayout();
	centralWidgetLayout->setContentsMargins(0, 0, 0, 0);

	m_headerLabel = new QLabel();
	m_headerLabel->setObjectName("headerLabel");
	m_headerLabel->setVisible(false);

	centralWidgetLayout->addWidget(m_headerLabel);
	centralWidgetLayout->addWidget(m_stackedWidget);

	centralWidget->setLayout(centralWidgetLayout);
	setCentralWidget(centralWidget);

	setWindowTitle(m_title);
}

void CentMainWindow::createMenu()
{
	QMenuBar* menu = menuBar();

	QMenu* fileMenu = menu->addMenu(tr("File"));
	QAction* closeAction = fileMenu->addAction(tr("Close"));
	CENT::connect(closeAction, SIGNAL(triggered()), this, SIGNAL(closeClicked()));

	setMenuBar(menu);
}

void CentMainWindow::closeEvent(QCloseEvent* closeEvent)
{
	QMainWindow::closeEvent(closeEvent);
	emit closeClicked();
}

