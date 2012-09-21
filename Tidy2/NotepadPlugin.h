#ifndef NOTEPADPLUGIN_H_3114142994
#define NOTEPADPLUGIN_H_3114142994

#define ADDMENUITEM(menuText,initToChecked,modifiers,shortcutKey,method) \
		addMenuItem(menuText, initToChecked, modifiers, shortcutKey, this, static_cast<void (__thiscall NotepadPlugin::* )()>(method))

#define ADDMENUITEM_WITHCONTEXT(menuText,initToChecked,modifiers,shortcutKey,method,contextType,context) \
		addMenuItem<contextType>(menuText, initToChecked, modifiers, shortcutKey, this, static_cast<void (__thiscall NotepadPlugin::* )(contextType)>(method), context)

#include "ModifierKeys.h"
#include "ContextualFunctionItem.h"

struct FuncItem;
class CallableFunction;


class NotepadPlugin {
public:
	NotepadPlugin(HMODULE hModule);
	virtual ~NotepadPlugin();

	/** Returns the name of the plugin (presented in the menu)
	 *  Edit the name in PluginDetails.h to change this 
	 */
	const TCHAR* getName();

	/** Handles Notepad++ notifications. 
	 *  Override notify(SCNotification*) to receive notifications
	 */
	void beNotified(SCNotification*	notification) { notify(notification); }
	
	/** Handles windows messages forwarded from Notepad++. 
	 *  Override windowsMessage(message, wParam, lParam) in order to receive windows messages.
	 */
	LRESULT messageProc(UINT message, WPARAM wParam, LPARAM lParam)
				{ return windowsMessage(message, wParam, lParam); }
	
	/** Sets the NppData
	 *  Users of the NotepadPlugin class do not need to use this method
	 */
	void setInfo(const NppData *nppData);

	/** Gets the FuncItem array for the Notepad++ call.
	 *  Users of the NotepadPlugin class do not need to use this method
	 */
	FuncItem* getMenuItems(int *itemCount);

	/** Calls the registered function with the specified index.
	 *  Users of the NotepadPlugin class do not need to use this method
	 */
	void callFunction(int index);


protected:
	
	typedef void (__thiscall NotepadPlugin::*methodCallTD )(void) ;

	/** 
	 * Adds a menu item. Can only be called inside a derived class' constructor. 
	 * Calling after getMenuItems() has been called results in an error (false return).
	 */
	int addMenuItem(tstring menuText, 
					const bool initToChecked,
					const ModifierKeys modifiers, 
					const UCHAR shortcutKey,
					void* pluginInstance, 
					methodCallTD method);


	

	/**
	* Adds a menu item. Can only be called inside a derived class' constructor. 
	* Calling after getMenuItems() has been called results in an error (returns -1).
	* @returns index of the menu item.
	*/
	template <typename ContextT>
	int addMenuItem(tstring menuText, 
					const bool initToChecked,
					const ModifierKeys modifiers, 
					const UCHAR shortcutKey,
					void* pluginInstance, 
					typedef void (__thiscall NotepadPlugin::*method )(ContextT),
					ContextT context);

	/** Receive a notification from Scintilla or Notepad++ 
	 *  Override this method to receive notifications from Notepad++
	 */
	virtual void notify(SCNotification *notification);

	/** Receive a windows message 
	 *  Override this method to receive windows messages from Notepad++
	 *  Note that only select messages are forwarded.
	 */
	virtual LRESULT windowsMessage(UINT Message, WPARAM wParam, LPARAM lParam);

	/** Sends a windows message to Notepad++
	 */
	LRESULT sendMessageNpp(UINT message, WPARAM wParam = 0, LPARAM lParam = 0) 
		{ return ::SendMessage(m_nppData._nppHandle, message, wParam, lParam); }

	/** Gets the HWND handle for Notepad++
	 */
	HWND getNppHandle() { return m_nppData._nppHandle; }


	/** Gets the HWND handle for the active Scintilla 
	 *  If you need to access this a lot, it's more efficient to cache the result of this, 
	 *  and refresh your cached copy if you receive a NPPN_BUFFERACTIVATED or NPPN_DOCORDERCHANGED 
	 *  to your notify() method
	 */
	HWND getCurrentHScintilla();

	/** Gets the first (main) Scintilla handle
	 */
	HWND getScintilla1() { return m_nppData._scintillaMainHandle; }

	/** Gets the second Scintilla handle (from the "right hand" view)
	 */
	HWND getScintilla2() { return m_nppData._scintillaSecondHandle; }

private:
	std::vector<CallableFunction*> m_functionList;
	int m_functionCount;
	HMODULE m_hModule;
	FuncItem* m_funcItemArray;
	NppData m_nppData;

	PFUNCPLUGINCMD getStaticFunction(int index);
	FuncItem* createFuncItem(tstring menuText, const bool initToChecked, ModifierKeys modifiers, UCHAR shortcutKey);
};


template <typename ContextT>
int NotepadPlugin::addMenuItem(tstring menuText, 
					const bool initToChecked,
					const ModifierKeys modifiers, 
					const UCHAR shortcutKey,
					void* pluginInstance, 
					typedef void (__thiscall NotepadPlugin::*method )(ContextT),
					ContextT context)
{
	ContextualFunctionItem<ContextT> *functionItem = new ContextualFunctionItem<ContextT>();
	functionItem->funcItem = createFuncItem(menuText, initToChecked, modifiers, shortcutKey);

	functionItem->instance = pluginInstance;
	functionItem->method = method;
	
	// TODO: Replace with templated FunctionItem
	functionItem->state = context;
	m_functionList.push_back(functionItem);

	return m_functionCount++;
}

#endif // NOTEPADPLUGIN_H_3114142994