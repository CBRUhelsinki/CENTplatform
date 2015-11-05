#include "videowidgetsurface.h"

#include <QtGui>
#include <qabstractvideosurface.h>
#include <qvideosurfaceformat.h>

VideoWidgetSurface::VideoWidgetSurface(QObject* parent)
	: QAbstractVideoSurface(parent)
	, m_imageFormat(QImage::Format_Invalid)
	, m_blankMode(true)
{
}

VideoWidgetSurface::~VideoWidgetSurface()
{
}

QList<QVideoFrame::PixelFormat> VideoWidgetSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
	if (handleType == QAbstractVideoBuffer::NoHandle)
	{
		return QList<QVideoFrame::PixelFormat>()
				<< QVideoFrame::Format_RGB32
				<< QVideoFrame::Format_ARGB32
				<< QVideoFrame::Format_ARGB32_Premultiplied
				<< QVideoFrame::Format_RGB565
				<< QVideoFrame::Format_RGB555;
	}
	else
	{
		return QList<QVideoFrame::PixelFormat>();
	}
}

bool VideoWidgetSurface::isFormatSupported(const QVideoSurfaceFormat& format, QVideoSurfaceFormat* similar) const
{
	Q_UNUSED(similar);

	const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
	const QSize size = format.frameSize();

	return imageFormat != QImage::Format_Invalid && !size.isEmpty()
			&& format.handleType() == QAbstractVideoBuffer::NoHandle;
}

bool VideoWidgetSurface::start(const QVideoSurfaceFormat& format)
{
	const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
	const QSize size = format.frameSize();

	if (imageFormat == QImage::Format_Invalid || size.isEmpty())
	{
		return false;
	}

	m_imageFormat = imageFormat;
	m_sourceRect = format.viewport();

	QAbstractVideoSurface::start(format);

	updateVideoRect(m_sourceRect);

	return true;
}

void VideoWidgetSurface::stop()
{
	m_currentFrame = QVideoFrame();
	m_targetRect = QRect();

	QAbstractVideoSurface::stop();
}

bool VideoWidgetSurface::present(const QVideoFrame& frame)
{
	if (surfaceFormat().pixelFormat() != frame.pixelFormat() 
		|| surfaceFormat().frameSize() != frame.size())
	{
		return false;
	}

	m_currentFrame = frame;
	return true;
}

void VideoWidgetSurface::updateVideoRect(const QRect& widgetRect)
{
	QSize size = surfaceFormat().sizeHint();
	size.scale(widgetRect.size().boundedTo(size), Qt::KeepAspectRatio);

	m_targetRect = QRect(QPoint(0, 0), size);
	m_targetRect.moveCenter(widgetRect.center());
}

void VideoWidgetSurface::paint(QPainter* painter)
{
	QRect painterRect = painter->viewport();
	updateVideoRect(painterRect);
	if (m_currentFrame.map(QAbstractVideoBuffer::ReadOnly))
	{
		const QTransform oldTransform = painter->transform();

		if (surfaceFormat().scanLineDirection() == QVideoSurfaceFormat::BottomToTop)
		{
			painter->scale(1, -1);
			painter->translate(0, -painterRect.height());
		}

		QImage image(
			m_currentFrame.bits(),
			m_currentFrame.width(),
			m_currentFrame.height(),
			m_currentFrame.bytesPerLine(),
			m_imageFormat);

		painter->drawImage(m_targetRect, image, m_sourceRect);

		painter->setTransform(oldTransform);

		m_currentFrame.unmap();
	}
}

void VideoWidgetSurface::setFallbackPath(const QString& filePath)
{
	m_filePath = filePath;
}

QString VideoWidgetSurface::getFallbackPath()
{
	return m_filePath;
}

bool VideoWidgetSurface::isBlankMode()
{
	return m_blankMode;
}

void VideoWidgetSurface::setBlankMode(bool mode)
{
	m_blankMode = mode;
}
