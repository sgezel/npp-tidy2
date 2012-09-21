#ifndef FUNCTIONITEM_H_148198471
#define FUNCTIONITEM_H_148198471

#include "CallableFunction.h"

class NotepadPlugin;


class FunctionItem : public CallableFunction
{
public:
	std::function<void(NotepadPlugin*)> method;
	
	void* instance;

	virtual void call();
};


void FunctionItem::call()
{
	method(static_cast<NotepadPlugin*>(instance));
}

#endif // FUNCTIONITEM_H_148198471
