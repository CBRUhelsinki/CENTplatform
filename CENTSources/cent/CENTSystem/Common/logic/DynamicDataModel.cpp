#include "DynamicDataModel.h"

#include <QDebug>
#include <QMetaProperty>
#include <QWidget>

#include "Connect.h"

namespace
{
	const QString QT_CONTROL_PREFIX = "qt_";
	const int QT_CONTROL_PREFIX_LENGTH = 3;
}

struct DynamicDataObject
{
	DynamicDataObject() {}

	QByteArray m_propertyName;
	QList<QWidget*> m_widgets;
	QVariant m_value;

private:
	Q_DISABLE_COPY(DynamicDataObject);
};

DynamicDataModel::DynamicDataModel(QObject* parent)
	: QObject(parent)
{
}

DynamicDataModel::~DynamicDataModel()
{
	qDeleteAll(m_dataObjects.values());
}

bool DynamicDataModel::addToModel(QWidget * const widget, bool recursive)
{
	if(!widget)
	{
		qWarning() << __FUNCTION__ << "Null pointer!";
		return false;
	}

	QString objectName = widget->objectName();
	if(objectName == widget->metaObject()->className() || objectName.isEmpty() || objectName.left(QT_CONTROL_PREFIX_LENGTH) == QT_CONTROL_PREFIX)
	{
		qDebug() << __FUNCTION__ << "Object's name has not been set or is the same as className or is a qt control. Ignoring this widget: " << objectName;
	}
	else
	{
		DynamicDataObject* dataObject = m_dataObjects.value(objectName, NULL);
		if(dataObject)
		{
			if(dataObject->m_widgets.isEmpty())
			{
				qCritical() << __FUNCTION__ << "m_widgets is empty, this should never happen.";
				return false;
			}

			QWidget* first = dataObject->m_widgets.first();
			if(QString::compare(first->metaObject()->className(), widget->metaObject()->className()) != 0)
			{
				qWarning() << __FUNCTION__ << "You tried to combine objects of different types.";
				return false;
			}

			if(dataObject->m_widgets.contains(widget))
			{
				qWarning() << __FUNCTION__ << "You tried to add an object that is already added.";
				return false;
			}
		}
		else
		{
			const QMetaProperty metaProperty = widget->metaObject()->userProperty();
			const QMetaMethod notifySignal = metaProperty.notifySignal();
			if(!metaProperty.isValid() || notifySignal.methodType() != QMetaMethod::Signal)
			{
				qDebug() << __FUNCTION__ << "Ignoring " << objectName << ", it doesn't have a user Property or a notify signal for it";
			}
			else
			{
				dataObject = new DynamicDataObject;
				dataObject->m_propertyName = metaProperty.name();
				dataObject->m_value = widget->property(dataObject->m_propertyName.data());
				m_dataObjects.insert(objectName, dataObject);
			}
		}

		if(dataObject)
		{
			QString signalSignature(widget->metaObject()->userProperty().notifySignal().signature());
			QString signalName = QString::number(QSIGNAL_CODE) + signalSignature;
			QByteArray byteSignalName = signalName.toAscii();
			CENT::connect(widget, byteSignalName.data(), this, SLOT(onNotify()));
			dataObject->m_widgets.append(widget);
		}
	}

	if(recursive)
	{
		foreach(QObject* child, widget->children())
		{
			if(QWidget* w = qobject_cast<QWidget*>(child))
			{
				addToModel(w);
			}
		}
	}
	return true;
}

void DynamicDataModel::onNotify()
{
	QWidget* sender = qobject_cast<QWidget*>(QObject::sender());
	if(!sender)
	{
		qFatal("Wrong object");
		return;
	}

	QString senderName = sender->objectName();
	DynamicDataObject* dataObject = m_dataObjects.value(senderName, NULL);
	if(!dataObject)
	{
		qCritical() << __FUNCTION__ << "Invalid DynamicDataObject for " << senderName;
		return;
	}

	QVariant newValue = sender->property(dataObject->m_propertyName.data());
	dataObject->m_value = newValue;
	foreach(QWidget* widget, dataObject->m_widgets)
	{
		if(widget == sender)
		{
			continue;
		}

		QVariant current = widget->property(dataObject->m_propertyName.data());
		if(current != newValue)
		{
			widget->setProperty(dataObject->m_propertyName.data(), newValue);
		}
	}
}

bool DynamicDataModel::setValueAndSync(const QString& widgetName, const QVariant& newValue)
{
	DynamicDataObject* dataObject = m_dataObjects.value(widgetName, NULL);
	if(!dataObject)
	{
		qWarning() << __FUNCTION__ << "Widget name doesn't exist in this data model, you tried: " << widgetName;
		return false;
	}

	if(dataObject->m_value.type() != newValue.type())
	{
		qWarning() << __FUNCTION__ << "Incompatible type of new value.";
		return false;
	}

	dataObject->m_value = newValue;
	return synchronize(dataObject);
}

bool DynamicDataModel::synchronizeAll()
{
	bool ret = true;
	foreach(DynamicDataObject* dataObject, m_dataObjects.values())
	{
		ret = ret && synchronize(dataObject);
	}
	return ret;
}

bool DynamicDataModel::synchronizeOne(const QString& widgetName)
{
	DynamicDataObject* dataObject = m_dataObjects.value(widgetName, NULL);
	if(!dataObject)
	{
		qWarning() << __FUNCTION__ << "Widget name doesn't exist in this data model, you tried: " << widgetName;
		return false;
	}

	return synchronize(dataObject);
}

bool DynamicDataModel::synchronize(DynamicDataObject* dataObject)
{
	if(!dataObject)
	{
		qCritical() << __FUNCTION__ << "Null pointer!";
		return false;
	}

	bool ret = true;
	foreach(QWidget* widget, dataObject->m_widgets)
	{
		QVariant current = widget->property(dataObject->m_propertyName.data());
		if(current != dataObject->m_value)
		{
			ret = ret && widget->setProperty(dataObject->m_propertyName.data(), dataObject->m_value);
		}
	}
	return ret;
}
