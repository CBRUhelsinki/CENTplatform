#include "SummaryPage.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "BarChartWidget.h"
#include "Connect.h"
#include "UiHelpers.h"

namespace
{
	const int BUTTON_MAX_WIDTH = 100;
}

SummaryPage::SummaryPage(Mode mode, QWidget* parent)
	: ICentPage(parent)
	, m_mode(mode)
	, m_barChart(0)
	, m_scoreLabel(0)
	, m_scoreText()
{
	createLayout();
}

SummaryPage::~SummaryPage()
{
}

QString SummaryPage::headerText()
{
	return tr("Games results");
}

void SummaryPage::setData(const QList< QPair<QString, double> > &data)
{
	m_barChart->setData(data);
}

void SummaryPage::setScore(const double score)
{
	m_scoreLabel->setText(m_scoreText + QString::number(score, 'g', 4) + "%");
}

void SummaryPage::setScoreLabelText(const QString& text)
{
	m_scoreText = text;
}

void SummaryPage::createLayout()
{
	m_scoreLabel = new QLabel();
	m_scoreLabel->setFont(QFont(m_scoreLabel->font().family(), 18));
	m_barChart = new BarChartWidget();
	m_barChart->setAxisLabel(BarChartWidget::LEFT_AXIS_ID, tr("Success level %"));

	QHBoxLayout* scoreLayout = new QHBoxLayout();
	scoreLayout->addStretch();
	scoreLayout->addWidget(m_scoreLabel);
	scoreLayout->addStretch();

	//Replay Button
	QPushButton* replayButton = new QPushButton(this);
	replayButton->setText(tr("Play again"));
	CENT::connect(replayButton, SIGNAL(clicked()), this, SIGNAL(playAgain()));
	
	//Next Button
	QPushButton* nextButton = new QPushButton(this);
	nextButton->setText(tr("Session summary"));
	CENT::connect(nextButton, SIGNAL(clicked()), this, SIGNAL(nextClicked()));

	//Change params Button
	QPushButton* paramsButton = new QPushButton(this);
	paramsButton->setText(tr("Change params"));
	CENT::connect(paramsButton, SIGNAL(clicked()), this, SIGNAL(changeParams()));

	//Change game
	QPushButton* gameButton = new QPushButton(this);
	gameButton->setText(tr("Change game"));
	CENT::connect(gameButton, SIGNAL(clicked()), this, SIGNAL(changeGame()));

	paramsButton->setVisible(m_mode == TherapistMode);
	nextButton->setVisible(m_mode == TherapistMode);

	QList<QWidget*> buttons;
	buttons << paramsButton << gameButton << replayButton << nextButton;

	QWidget* buttonsWidget = new QWidget;
	UiHelpers::ButtonLayout uiMode = m_mode == TherapistMode ? UiHelpers::LastRightAndRestCenter : UiHelpers::CenterWidgets;

	bool b = UiHelpers::createButtonFooter(*buttonsWidget, buttons, uiMode);
	Q_ASSERT(b); Q_UNUSED(b);


	//main layout
	QVBoxLayout* layout = new QVBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_barChart);
	layout->addLayout(scoreLayout);
	layout->addSpacing(10);
	layout->addWidget(buttonsWidget);

	layout->setContentsMargins(0, 0, 0, 0);
	setLayout(layout);
}
