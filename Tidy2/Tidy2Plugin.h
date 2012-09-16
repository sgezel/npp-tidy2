#ifndef TIDY2PLUGIN_H_3402113123
#define TIDY2PLUGIN_H_3402113123

#include "NotepadPlugin.h"

class Tidy2Plugin : public NotepadPlugin
{
public:
	Tidy2Plugin(HMODULE hModule);
	virtual ~Tidy2Plugin() {}

	
	
	virtual void notify(SCNotification* notification);

private:
	void callTidy(void *context) { doTidy((const char *)context); }
	void doTidy(const char *configName);
	void getConfigPath(char *result, int bufferSize, const char *configName);
	void createDefaultConfig(const char *configPath);
};

#endif // TIDY2PLUGIN_H_3402113123