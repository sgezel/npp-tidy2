#ifndef FUNCTIONITEM_H_32402394
#define FUNCTIONITEM_H_32402394

#include "PluginDetails.h"

class NotepadPlugin;
class PLUGIN_CLASS_NAME;

namespace MethodType
{
enum MethodType
{
	Stateless,
	Stateful
};
}


class FunctionItem
{
public:
	std::function<void(NotepadPlugin*, void*)> statefulMethod;
	std::function<void(NotepadPlugin*)> statelessMethod;
	FuncItem* funcItem;
	void* instance;
	void* state;
	MethodType::MethodType methodType;
};

#endif
