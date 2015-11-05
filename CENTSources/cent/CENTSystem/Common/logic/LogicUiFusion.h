#ifndef LOGIC_UI_FUSION_H
#define LOGIC_UI_FUSION_H

#include <QMap>

#include "CentMainWindowControllerBase.h"
#include "BaseCentPageController.h"

template <class T, class V>
class LogicUiFusion
{
public:
	explicit LogicUiFusion(CentMainWindowControllerBase& mainWindowController);
	inline ~LogicUiFusion() {}

public:
	void registerPageController(T state, BaseCentPageController& pageController);
	void onStateChanged(T state);

	void setStateToUiMap(const QMap<T, V>& stateToUi);

private:
	QMap<T, BaseCentPageController*> m_stateToController;
	QMap<T, V>                       m_stateToUi;

	CentMainWindowControllerBase&    m_mainWindowController;
	BaseCentPageController*          m_currentPageController;
};



template <class T, class V>
LogicUiFusion<T,V>::LogicUiFusion(CentMainWindowControllerBase& mainWindowController)
	: m_mainWindowController(mainWindowController)
	, m_currentPageController(0)
{

}

template <class T, class V>
void LogicUiFusion<T,V>::registerPageController(T state, BaseCentPageController& pageController)
{
	pageController.registerPage();
	m_stateToController.insert(state, &pageController);
}

template <class T, class V>
void LogicUiFusion<T,V>::onStateChanged(T state)
{
	BaseCentPageController* newPageController = m_stateToController.value(state);
	if (! newPageController)
	{
		return;
	}

	m_mainWindowController.onUiStateChanged(m_stateToUi.value(state));

	if (m_currentPageController)
	{
		m_currentPageController->deactivatePage();
	}

	newPageController->activatePage();
	m_currentPageController = newPageController;
}

template <class T, class V>
void LogicUiFusion<T,V>::setStateToUiMap(const QMap<T, V>& stateToUi)
{
	m_stateToUi.clear();
	m_stateToUi = stateToUi;
}

#endif // LOGIC_UI_FUSION_H

