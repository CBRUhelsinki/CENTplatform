#include "ScoreWidget.h"
#include "MediaGameSummary.h"

#include <QPainter>
#include <QPaintEvent>
#include <QApplication>

namespace
{
	const int SCORE_FONT_SIZE               = 30;
	const int BONUS_SCORE_FONT_SIZE         = 20;
	const double SCORE_PEN_WIGHT_SIZE       = 1;
	const double BONUS_SCORE_PEN_WIGHT_SIZE = 0.75;

	const QString FONT_NAME("Chicago");
}

ScoreWidget::ScoreWidget(QWidget* parent)
	: QWidget(parent)
	, m_mediaGameSummary(0)
{
	setAutoFillBackground(false);
}

ScoreWidget::~ScoreWidget()
{
}

void ScoreWidget::setMediaGameSummary(MediaGameSummary* mediaGameSummary)
{
	if(mediaGameSummary)
	{
		m_mediaGameSummary = mediaGameSummary;
	}
}

void ScoreWidget::paintEvent(QPaintEvent* event)
{
	QPainter mePainter(this);

	mePainter.setFont(QFont(FONT_NAME, SCORE_FONT_SIZE));
	mePainter.setPen(QPen(Qt::black, SCORE_PEN_WIGHT_SIZE));

	QRect boundingRect;
	mePainter.drawText(event->rect(), Qt::AlignLeft | Qt::AlignTop, QString::number(m_mediaGameSummary->getScore()), &boundingRect);
	int bonusScore =m_mediaGameSummary->getBonusscore();
	if(bonusScore > 0)
	{
		QRect bonusRect(boundingRect.bottomLeft(), QSize(120,100));
		QString str = QString("+%1").arg(bonusScore);
		mePainter.setFont(QFont(FONT_NAME, BONUS_SCORE_FONT_SIZE));
		mePainter.setPen(QPen(Qt::black, BONUS_SCORE_PEN_WIGHT_SIZE));
		mePainter.drawText(bonusRect, str);
	}
	update();
}
