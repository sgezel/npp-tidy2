// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "PluginMain.h"
#include "Tidy2Plugin.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	g_hModule = hModule;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_plugin = new PLUGIN_CLASS_NAME(hModule);
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		delete g_plugin;
		g_plugin = NULL;
		break;
	}
	return TRUE;
}


