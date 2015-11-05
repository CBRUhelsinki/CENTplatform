#ifndef IACTION_INVOKER_H
#define IACTION_INVOKER_H

class IResultReceiver
{
public:
	virtual void result(int error) = 0;
};
#endif // IACTION_INVOKER_H