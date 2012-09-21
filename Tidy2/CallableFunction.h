#ifndef CALLABLEFUNCTION_H_19837191 
#define CALLABLEFUNCTION_H_19837191 

class FuncItem;

class CallableFunction
{
public:
	CallableFunction() { }
	virtual ~CallableFunction() {}

	FuncItem* funcItem;

	virtual void call() = 0;
};

#endif //CALLABLEFUNCTION_H_19837191 