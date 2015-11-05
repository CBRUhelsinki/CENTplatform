#ifndef MULTI_BUTTON_WIDGET_BASE_H
#define MULTI_BUTTON_WIDGET_BASE_H

#include <QWidget>

class QHBoxLayout;
class QPushButton;
class QSignalMapper;

/*** This is not ideal
* It should be just a class for multi button selection widget, 
* instead it is used as a template for a wizard page that has multiple buttons as a mean of selection
* not enough time to do it right, there is only enough to eliminate the nasty copy-paste that was made in classes that now derive from this
***/
class MultiButtonWidgetBase : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(int selectedButton READ selection WRITE setSelection RESET resetSelection NOTIFY selectionChanged USER true)
public:
	explicit MultiButtonWidgetBase(int buttonCount, QWidget* parent = 0);
	virtual ~MultiButtonWidgetBase();

	int selection();

	virtual void decorateButton(int buttonNumber, QPushButton& button) = 0;
	void setButtonNames(const QStringList& names);
	void setButtonsGroupProperty(const QString& name);

public: // constant data
	static const int NOT_SELECTED;

signals:
	void selectionChanged(int sel);
	void isValid(bool valid);
	void buttonClicked();

public slots:
	void setSelection(int sel);
	void resetSelection();
	void emitIsValid();

private slots:
	void onButtonClicked(int num);

protected:
	void createButtonsLayout(QHBoxLayout& buttonsLayout);
	virtual void createConnections();

	void resizeEvent(QResizeEvent* event);

private:
	int m_buttonCount;
	int m_selection;

	QList<QPushButton*> m_buttons;
	QSignalMapper*      m_signalMap;
};

#endif // MULTI_BUTTON_WIDGET_BASE_H
