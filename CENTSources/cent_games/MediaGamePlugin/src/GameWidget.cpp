#include "GameWidget.h"
#include "VideoWidgetSurface.h"
#include "MediaGameSummary.h"
#include "ScoreWidget.h"

//#include <QtGui>
#include <QPainter>
#include <QPaintEvent>
#include <QHBoxLayout>

GameWidget::GameWidget(QWidget* parent)
	: QWidget(parent)
	, m_surface(0)
	, m_mediaGameSummary(0)
	, m_scoreWidget(new ScoreWidget(this))
{
	setAutoFillBackground(false);
	setAttribute(Qt::WA_NoSystemBackground, true);
	setAttribute(Qt::WA_PaintOnScreen, true);

	QPalette palette = this->palette();
	palette.setColor(QPalette::Background, Qt::black);
	setPalette(palette);

	m_pixmap = new QPixmap; 
	m_pixmap->load(":/MediaGamePlugin/background.png");

	QHBoxLayout* layout = new QHBoxLayout;
	layout->addWidget(m_scoreWidget);
	setLayout(layout);

	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

GameWidget::~GameWidget()
{
	delete m_scoreWidget;
}

void GameWidget::setVideoSurface(VideoWidgetSurface* surface)
{
	if(surface)
	{
		m_surface = surface;
	}
}

void GameWidget::setMediaGameSummary(MediaGameSummary* mediaGameSummary)
{
	if(mediaGameSummary)
	{
		m_mediaGameSummary = mediaGameSummary;
		m_scoreWidget->setMediaGameSummary(mediaGameSummary);
	}
}

void GameWidget::paintEvent(QPaintEvent* event)
{
	QPainter mePainter(this);
	updateBackgroundRect(event->rect());
	
	paintVideo(&mePainter);

	// Get clipping frame rect
	int radius_x = m_mediaGameSummary->getClippingFrame() * width() / 100;
	int radius_y = m_mediaGameSummary->getClippingFrame() * height() / 100;
	QRect clippingRect(0,0, radius_x, radius_y);
	clippingRect.moveCenter(m_targetRect.center());

	paintAlpha(&mePainter, clippingRect);
	paintEllipse(&mePainter, clippingRect);
	
	update();
}

void GameWidget::updateBackgroundRect(const QRect& widgetRect)
{
	m_targetRect = widgetRect;
}

void GameWidget::paintVideo(QPainter* painter)
{
	if (m_surface->isActive())
	{
		QBrush brush = palette().background();
		painter->fillRect(rect(), brush);
		m_surface->paint(painter);
	}
	else
	{
		QString fielPath = m_surface->getFallbackPath();
		if(!fielPath.isEmpty() && !m_surface->isBlankMode())
		{
			QImage image(fielPath);
			if(!image.isNull())
			{
				painter->drawImage(m_targetRect,image);
				return;
			}
		}
		painter->fillRect(m_targetRect, palette().background());
	}
}

void GameWidget::paintAlpha(QPainter* painter, const QRect& rect)
{
	painter->setRenderHint(QPainter::Antialiasing, true);

	int alpha = m_mediaGameSummary->getAlphaLevel();
	painter->fillRect(rect, QColor(255, 0, 0, alpha));
}

void GameWidget::paintEllipse(QPainter* painter, const QRect& rect)
{
	QRegion ellipseRegion(rect, QRegion::Ellipse);
	QRegion targectRectRegion(m_targetRect);

	painter->setClipRegion(targectRectRegion.xored(ellipseRegion));
	painter->drawPixmap (m_targetRect, *m_pixmap);
}
