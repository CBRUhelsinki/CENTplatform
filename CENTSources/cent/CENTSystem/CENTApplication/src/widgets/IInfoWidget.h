#ifndef I_INFO_WIDGET_H
#define I_INFO_WIDGET_H

namespace CentPatient
{
	struct Data;
}

class IInfoWidget
{
public:
	virtual void getQuestionaryData(CentPatient::Data& data) = 0;
	virtual void reset() = 0;
	virtual void triggerValidityCheck() = 0;	//this should emit isValid() in current "design"
	virtual bool checkValid() = 0;
};

#endif //I_INFO_WIDGET_H