#ifndef SCOREWIDGET_H
#define SCOREWIDGET_H

#include <QWidget>

class MediaGameSummary;

class ScoreWidget : public QWidget
{
public:
	ScoreWidget(QWidget* parent = 0);
	~ScoreWidget();

	void setMediaGameSummary(MediaGameSummary* mediaGameSummary);

protected:
	void paintEvent(QPaintEvent* event);

private:
	MediaGameSummary*   m_mediaGameSummary;
};
#endif //SCOREWIDGET_H
