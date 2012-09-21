#ifndef FUNCTIONITEM_H_32402394
#define FUNCTIONITEM_H_32402394

#include "CallableFunction.h"

class NotepadPlugin;


template <typename ContextT>
class ContextualFunctionItem : public CallableFunction
{
public:
	std::function<void(NotepadPlugin*, ContextT)> method;
	
	
	void* instance;
	ContextT state;


	virtual void call();
};


template <typename ContextT>
void ContextualFunctionItem<ContextT>::call()
{
	method(static_cast<NotepadPlugin*>(instance), this->state);
}

#endif
