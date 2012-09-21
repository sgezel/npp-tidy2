#include "stdafx.h"

#include "NotepadPlugin.h"
#include "PluginDetails.h"
#include "FunctionItem.h"


NotepadPlugin::NotepadPlugin(HMODULE hModule)
	: m_hModule(hModule),
	  m_funcItemArray(NULL),
	  m_functionCount(0)
{
}

NotepadPlugin::~NotepadPlugin()
{
}

const TCHAR* NotepadPlugin::getName()
{
	return _T(PLUGIN_NAME);
}

FuncItem* NotepadPlugin::getMenuItems(int *itemCount)
{
	if (NULL == m_funcItemArray)
	{
		m_funcItemArray = new FuncItem[m_functionList.size()];
		int index = 0;

		for(std::vector<CallableFunction*>::iterator it = m_functionList.begin(); it != m_functionList.end(); ++it, ++index)
		{
			m_funcItemArray[index] = *((*it)->funcItem);
		}
	}

	*itemCount = m_functionList.size();
	return m_funcItemArray;
}

void NotepadPlugin::notify(SCNotification* notification)
{
}

LRESULT NotepadPlugin::windowsMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

void NotepadPlugin::setInfo(const NppData* nppData)
{
	m_nppData = *nppData;
}

	

int NotepadPlugin::addMenuItem(tstring menuText, 
					const bool initToChecked,
					const ModifierKeys modifiers, 
					const UCHAR shortcutKey,
					void* pluginInstance, 
					methodCallTD method)
{
	FunctionItem *functionItem = new FunctionItem();
	functionItem->funcItem = createFuncItem(menuText, initToChecked, modifiers, shortcutKey);

	functionItem->instance = pluginInstance;
	functionItem->method = method;
	m_functionList.push_back(functionItem);

	return m_functionCount++;
}



FuncItem* NotepadPlugin::createFuncItem(tstring menuText, const bool initToChecked, ModifierKeys modifiers, UCHAR shortcutKey)
{
	FuncItem* item = new FuncItem();
	_tcscpy_s<64>(item->_itemName, menuText.c_str());
	item->_pFunc = getStaticFunction(m_functionCount);
	if (0 != shortcutKey)
	{
		ShortcutKey *shortcut = new ShortcutKey();
		shortcut->_key = shortcutKey;
		shortcut->_isCtrl = (modifiers & ModifierCtrl) == ModifierCtrl;
		shortcut->_isAlt = (modifiers & ModifierAlt) == ModifierAlt;
		shortcut->_isShift = (modifiers & ModifierShift) == ModifierShift;;

		item->_pShKey = shortcut;
	}
	else
	{
		item->_pShKey = NULL;
	}

	item->_init2Check = initToChecked;

	return item;
}


void NotepadPlugin::callFunction(int index)
{
	m_functionList[index]->call();
}

HWND NotepadPlugin::getCurrentHScintilla()
{
	int whichScintilla;
	sendMessageNpp(NPPM_GETCURRENTSCINTILLA, 0, reinterpret_cast<LPARAM>(&whichScintilla));
	return whichScintilla ? m_nppData._scintillaSecondHandle : m_nppData._scintillaMainHandle;
}

