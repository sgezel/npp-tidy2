#ifndef NOTEPADPLUGIN_H_3114142994
#define NOTEPADPLUGIN_H_3114142994

#include "PluginDetails.h"
#include "ModifierKeys.h"

struct FuncItem;
class FunctionItem;
class PLUGIN_CLASS_NAME;

class NotepadPlugin {
public:
	NotepadPlugin(HMODULE hModule);
	virtual ~NotepadPlugin();

	const TCHAR* getName();

	void beNotified(SCNotification*	notification) { notify(notification); }
	
	LRESULT messageProc(UINT message, WPARAM wParam, LPARAM lParam)
				{ return windowsMessage(message, wParam, lParam); }
	
	void setInfo(const NppData *nppData);

	FuncItem* getMenuItems(int *itemCount);

	void callFunction(int index);

	typedef void(NotepadPlugin::*methodCall)();

protected:
	///<summary>
	/// Adds a menu item. Can only be called inside a derived class' constructor. 
	/// Calling after getMenuItems() has been called results in an error (false return).
	/// </summary>
	int addMenuItem(tstring menuText, 
					const bool initToChecked,
					const ModifierKeys modifiers, 
					const UCHAR shortcutKey,
					PLUGIN_CLASS_NAME* pluginInstance, 
					std::function<void(PLUGIN_CLASS_NAME*)> method);

	///<summary>
	/// Adds a menu item. Can only be called inside a derived class' constructor. 
	/// Calling after getMenuItems() has been called results in an error (false return).
	/// </summary>
	int addMenuItem(tstring menuText, 
					const bool initToChecked,
					const ModifierKeys modifiers, 
					const UCHAR shortcutKey,
					PLUGIN_CLASS_NAME* pluginInstance, 
					std::function<void(PLUGIN_CLASS_NAME*, void*)> method,
					void* context);

	/** Receive a notification from Scintilla or Notepad++ */
	virtual void notify(SCNotification *notification);

	/** Receive a windows message */
	virtual LRESULT windowsMessage(UINT Message, WPARAM wParam, LPARAM lParam);


	LRESULT sendMessageNpp(UINT message, WPARAM wParam = 0, LPARAM lParam = 0) 
		{ return ::SendMessage(m_nppData._nppHandle, message, wParam, lParam); }

	HWND getNppHandle() { return m_nppData._nppHandle; }

	HWND getCurrentHScintilla();

	HWND getScintilla1() { return m_nppData._scintillaMainHandle; }
	HWND getScintilla2() { return m_nppData._scintillaSecondHandle; }

private:
	std::vector<FunctionItem*> m_functionList;
	int m_functionCount;
	HMODULE m_hModule;
	FuncItem* m_funcItemArray;
	NppData m_nppData;

	PFUNCPLUGINCMD getStaticFunction(int index);
	FuncItem* createFuncItem(tstring menuText, const bool initToChecked, ModifierKeys modifiers, UCHAR shortcutKey);
};

#endif // NOTEPADPLUGIN_H_3114142994