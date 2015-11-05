#ifndef DYNAMIC_DATA_MODEL_H
#define DYNAMIC_DATA_MODEL_H

#include <QObject>
#include <QVariant>

class QWidget;

struct DynamicDataObject;

/***
* This class is not perfect. There might happen synchronization issues. Needs more work for more complex usage.
*/

class DynamicDataModel : public QObject
{
	Q_OBJECT
public:
	explicit DynamicDataModel(QObject* parent = 0);
	virtual ~DynamicDataModel();

public:
	bool addToModel(QWidget * const widget, bool recursive = true);

public slots:
	void onNotify();
	bool setValueAndSync(const QString& widgetName, const QVariant& newValue);
	bool synchronizeAll();
	bool synchronizeOne(const QString& widgetName);

private:
	bool synchronize(DynamicDataObject* dataObject);

private:
	QMap<QString, DynamicDataObject*> m_dataObjects;
};

#endif // DYNAMIC_DATA_MODEL_H
