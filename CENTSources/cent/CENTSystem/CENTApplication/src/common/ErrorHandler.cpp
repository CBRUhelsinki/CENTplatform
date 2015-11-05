#include "ErrorHandler.h"

#include <QWidget>
#include <QMessageBox>

namespace
{
	const QString CENT_ERROR_WINDOW_TITLE("CENT System error");
}

ErrorHandler::ErrorHandler(QWidget& messageBoxParent, QObject* parent)
	: QObject(parent)
	, m_messageBoxParent(messageBoxParent)
{

}

ErrorHandler::~ErrorHandler()
{
}

void ErrorHandler::onError(const QString& message, Qt::WindowModality modality)
{
	QMessageBox* messageBox = new QMessageBox(QMessageBox::Critical, CENT_ERROR_WINDOW_TITLE, message, QMessageBox::Ok, &m_messageBoxParent);

	messageBox->setAttribute(Qt::WA_DeleteOnClose, true);
	messageBox->setWindowModality(modality);
	messageBox->setVisible(true);
}