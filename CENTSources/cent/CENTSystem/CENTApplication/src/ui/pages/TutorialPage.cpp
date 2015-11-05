#include "TutorialPage.h"

#include <QComboBox>
#include <QDir>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QtWebKit>
#include <QVBoxLayout>

#include "Connect.h"
#include "UiHelpers.h"

namespace
{
	const QString TUTORIAL_DIR_FORMATTER = "plugins/Tutorials/%1_tutorial";
	const QString TUTORIAL_INDEX = "index.html";
	const QString FILE_URL_FORMATTER = "file:///%1";

	const char* TUTORIAL_UNAVAILABLE_TEXT = "Tutorial unavailable";
	const QString TUTORIAL_UNAVAILABLE_HTML_FORMATTER = "<html><head></head><body><b>%1</b></body></html>";

	const int BUTTON_MAX_WIDTH = 100;
}

TutorialPage::TutorialPage(QWidget* parent)
	: ICentPage(parent)
	, m_gameComboBox(0)
	, m_tutorialWebView(0)
{
	createLayout();
}

TutorialPage::~TutorialPage()
{
}

void TutorialPage::setTutorial(const QString& fileName)
{
	QString name = fileName;
#ifndef NDEBUG
	if(fileName.count() && fileName.right(1) == "d")
	{
		name = fileName.left(fileName.count() - 1);
	}
#endif
	QDir tutorialDir(QCoreApplication::applicationDirPath() + QDir::separator() + TUTORIAL_DIR_FORMATTER.arg(name));

	QString tutorialPath = tutorialDir.absoluteFilePath(TUTORIAL_INDEX);

	if(tutorialDir.exists(tutorialPath))
	{
		m_tutorialWebView->load(QUrl(FILE_URL_FORMATTER.arg(tutorialPath), QUrl::TolerantMode));
	}
	else
	{
		m_tutorialWebView->setHtml(TUTORIAL_UNAVAILABLE_HTML_FORMATTER.arg(tr(TUTORIAL_UNAVAILABLE_TEXT)));
	}
}

void TutorialPage::createLayout()
{
	m_tutorialWebView = new QWebView(this);
	m_tutorialWebView->setObjectName("TutorialPageQWebView");

	m_tutorialWebView->page()->history()->setMaximumItemCount(0); // we don't want back/forwoard functionality
	m_tutorialWebView->setContextMenuPolicy(Qt::NoContextMenu);
	m_tutorialWebView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->addStretch();
	hLayout->addWidget(m_tutorialWebView);
	hLayout->addStretch();

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addStretch();
	mainLayout->addLayout(hLayout);

	QPushButton* backButton = new QPushButton(tr("&Back"));
	backButton->setMaximumWidth(BUTTON_MAX_WIDTH);
	CENT::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backClicked()));

	QPushButton* nextButton = new QPushButton(tr("&Play"));
	nextButton->setMaximumWidth(BUTTON_MAX_WIDTH);
	CENT::connect(nextButton, SIGNAL(clicked()), this, SIGNAL(nextClicked()));

	// Buttons
	QWidget* buttonWidget = new QWidget();

	QList<QWidget*> buttons;
	buttons << backButton << nextButton;

	bool b = UiHelpers::createButtonFooter(*buttonWidget, buttons, UiHelpers::AllRight);
	Q_ASSERT(b); Q_UNUSED(b);

	mainLayout->addStretch();
	mainLayout->addWidget(buttonWidget);

	setLayout(mainLayout);
}

