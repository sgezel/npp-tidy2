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
	std::function<void(PLUGIN_CLASS_NAME*, void*)> statefulMethod;
	std::function<void(PLUGIN_CLASS_NAME*)> statelessMethod;
	FuncItem* funcItem;
	PLUGIN_CLASS_NAME* instance;
	void* state;
	MethodType::MethodType methodType;
};

#endif
