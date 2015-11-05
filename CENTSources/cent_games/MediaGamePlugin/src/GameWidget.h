#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QPixmap>

class VideoWidgetSurface;
class MediaGameSummary;
class ScoreWidget;

class GameWidget : public QWidget
{
public:
	GameWidget(QWidget* parent = 0);
	~GameWidget();

	void setVideoSurface(VideoWidgetSurface* surface);
	void setMediaGameSummary(MediaGameSummary* mediaGameSummary);

protected:
	void paintEvent(QPaintEvent* event);

private:
	void paintVideo(QPainter* painter);
	void paintAlpha(QPainter* painter, const QRect& rect);
	void paintEllipse(QPainter* painter, const QRect& rect);
	void updateBackgroundRect(const QRect& widgetRect);

private:
	VideoWidgetSurface* m_surface;
	MediaGameSummary*   m_mediaGameSummary;
	QPixmap*            m_pixmap;
	QRect               m_targetRect;
	ScoreWidget*        m_scoreWidget;
};
#endif
