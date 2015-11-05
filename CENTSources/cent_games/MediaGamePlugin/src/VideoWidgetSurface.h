#ifndef VIDEOWIDGETSURFACE_H
#define VIDEOWIDGETSURFACE_H

#include <QtCore/QRect>
#include <QtGui/QImage>
#include <qabstractvideosurface.h>
#include <qvideoframe.h>


class VideoWidgetSurface : public QAbstractVideoSurface
{
	Q_OBJECT
public:
	VideoWidgetSurface(QObject* parent = 0);
	virtual ~VideoWidgetSurface();

	QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;
	bool isFormatSupported(const QVideoSurfaceFormat& format, QVideoSurfaceFormat* similar) const;

	bool start(const QVideoSurfaceFormat& format);
	void stop();

	bool present(const QVideoFrame& frame);

	QRect videoRect() const { return m_targetRect; }
	void updateVideoRect(const QRect& m_widgetRect);

	void paint(QPainter* painter);

	void setFallbackPath(const QString& filePath);
	QString getFallbackPath();

	bool isBlankMode();
	void setBlankMode(bool mode);

private:
	QImage::Format m_imageFormat;
	QRect          m_targetRect;
	QRect          m_sourceRect;
	QVideoFrame    m_currentFrame;
	QString        m_filePath;
	bool           m_blankMode;
};
#endif
