#include "SessionFeedbackPage.h"

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

SessionFeedbackPage::SessionFeedbackPage(QWidget* parent)
	: ICentPage(parent)
	, m_barChart(0)
	, m_scoreLabel(0)
	, m_scoreText()
{
	createLayout();
}

SessionFeedbackPage::~SessionFeedbackPage()
{
}

QString SessionFeedbackPage::headerText()
{
	return tr("Session Feedback");
}

void SessionFeedbackPage::setData(const QList< QPair<QString, double> > &data)
{
	m_barChart->setData(data);
}

void SessionFeedbackPage::setScore(const double score)
{
	m_scoreLabel->setText(m_scoreText + QString::number(score, 'g', 4) + "%");
}

void SessionFeedbackPage::setScoreLabelText(const QString& text)
{
	m_scoreText = text;
}

void SessionFeedbackPage::createLayout()
{
	m_scoreLabel = new QLabel();
	m_scoreLabel->setFont(QFont(m_scoreLabel->font().family(), 18));
	m_barChart = new BarChartWidget();
	m_barChart->setAxisLabel(BarChartWidget::LEFT_AXIS_ID, tr("Success level %"));

	QHBoxLayout* scoreLayout = new QHBoxLayout();
	scoreLayout->addStretch();
	scoreLayout->addWidget(m_scoreLabel);
	scoreLayout->addStretch();

	///// Next Button
	QPushButton* nextButton = new QPushButton(tr("&Next"));
	nextButton->setMaximumWidth(BUTTON_MAX_WIDTH);
	CENT::connect(nextButton, SIGNAL(clicked()), this, SIGNAL(nextClicked()));

	QWidget* buttonWidget = new QWidget;
	QList<QWidget*> list;
	list << nextButton;
	bool b = UiHelpers::createButtonFooter(*buttonWidget, list, UiHelpers::AllRight);
	Q_ASSERT(b); Q_UNUSED(b);

	//main layout
	QVBoxLayout* layout = new QVBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_barChart);
	layout->addLayout(scoreLayout);
	layout->addSpacing(10);
	layout->addWidget(buttonWidget);

	layout->setContentsMargins(0, 0, 0, 0);
	setLayout(layout);
}

