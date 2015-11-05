#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <QObject>
#include <QString>

class QWidget;
class QMessageBox;

class ErrorHandler : public QObject
{
	Q_OBJECT
public:
	explicit ErrorHandler(QWidget& messageBoxParent, QObject* parent = 0);
	virtual ~ErrorHandler();
	
public slots:
	void onError(const QString& message, Qt::WindowModality modality = Qt::WindowModal);

private:
	QWidget& m_messageBoxParent;
};

#endif //ERROR_HANDLER_h