#include "stdafx.h"

#include "Tidy2Plugin.h"
#include "../tidy-html5/include/tidy.h"
#include "TidyReaderWriter.h"

Tidy2Plugin::Tidy2Plugin(HMODULE hModule)
	: NotepadPlugin(hModule)
{
	ADDMENUITEM(_T("Tidy 1"), false, ModifierKeys::None, 0, &Tidy2Plugin::showMessage);
	ADDMENUITEM_WITHCONTEXT(_T("Tidy 2"), false, ModifierKeys::None, 0, &Tidy2Plugin::showMessageParam, const TCHAR *, _T("Test macro!"));
}




void Tidy2Plugin::getConfigPath(char *result, int bufferSize, const char *configName)
{
#ifdef UNICODE
	TCHAR wideConfigPath[MAX_PATH];
	sendMessageNpp(NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, reinterpret_cast<LPARAM>(&wideConfigPath));
	
	WideCharToMultiByte(CP_UTF8, 0, wideConfigPath, -1, result, bufferSize, NULL, NULL);
	PathCombineA(result, result, configName);

#else
	
	sendMessageNpp(NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, reinterpret_cast<LPARAM>(&result));
	PathCombineA(result, result, configName);
#endif
}


void Tidy2Plugin::doTidy(const char *configName)
{
	HWND currentScintilla = getCurrentHScintilla();
	int originalLength = static_cast<int>(::SendMessage(currentScintilla, SCI_GETLENGTH, 0, 0));
	LRESULT currentBufferID = sendMessageNpp(NPPM_GETCURRENTBUFFERID);
	int encoding = static_cast<int>(sendMessageNpp(NPPM_GETBUFFERENCODING, currentBufferID));
	TidyDoc tidyDoc = tidyCreate();

	char configPath[MAX_PATH];
	getConfigPath(configPath, MAX_PATH, configName);

	if (!tidyFileExists(tidyDoc, configPath))
	{
		createDefaultConfig(configPath);
	}

	tidyLoadConfigEnc(tidyDoc, configPath, "utf8");


	Tidy2::TidyReaderWriter::TidyReadContext context;
	context.nextPosition = 0;
	context.hScintilla = currentScintilla;
	context.length = originalLength;
	TidyInputSource inputSource;
	inputSource.getByte = Tidy2::TidyReaderWriter::getByte;
	inputSource.ungetByte = Tidy2::TidyReaderWriter::ungetByte;
	inputSource.sourceData = &context;
	inputSource.eof = Tidy2::TidyReaderWriter::isInputEof;
	char encodingString[30];
	
	switch(encoding)
	{
		case 1:  // UTF8
		case 2:  // UCS-2 BE, but stored in scintilla as UTF8
		case 3:  // UCS-2 LE, but stored in scintilla as UTF8
		case 6:  // UCS-2 BE No BOM - probably doesn't even exist
		case 7:  // UCS-2 LE No BOM - probably doesn't even exist
			strcpy(encodingString, "utf8");
			break;

		case 0:  // Ansi
		case 5:  // 7bit
			strcpy(encodingString, "raw");
			break;
		default:
			strcpy(encodingString, "raw");  // Assume it's some form of ASCII/Latin text
			break;

	}

	tidySetCharEncoding(tidyDoc, encodingString);
	tidyParseSource(tidyDoc, &inputSource);
	
	TidyOutputSink outputSink;
	Tidy2::TidyReaderWriter::TidyWriteContext writeContext;
	
	
	writeContext.hScintilla = reinterpret_cast<HWND>(sendMessageNpp(NPPM_CREATESCINTILLAHANDLE));
	if (NULL == writeContext.hScintilla) 
	{
		::MessageBox(getNppHandle(), _T("Could not create new Scintilla handle for tidied output (You're probably low on memory!)"), _T("Tidy Errors"), MB_ICONERROR | MB_OK);
	}
	else
	{
		::SendMessage(writeContext.hScintilla, SCI_ALLOCATE, originalLength, 0);

		outputSink.putByte = Tidy2::TidyReaderWriter::putByte;
		outputSink.sinkData = &writeContext;
		
		
		tidyCleanAndRepair(tidyDoc);

		tidySaveSink(tidyDoc, &outputSink);
		int errorCount = tidyErrorCount(tidyDoc);

		if (errorCount > 0)
		{
			::MessageBox(getNppHandle(), _T("Errors were encountered tidying the document"), _T("Tidy Errors"), MB_ICONERROR | MB_OK);
		}
		else
		{
			char *text = reinterpret_cast<char*>(::SendMessage(writeContext.hScintilla, SCI_GETCHARACTERPOINTER, 0, 0));
			int length = static_cast<int>(::SendMessage(writeContext.hScintilla, SCI_GETLENGTH, 0, 0));
			::SendMessage(currentScintilla, SCI_BEGINUNDOACTION, 0, 0);
			::SendMessage(currentScintilla, SCI_CLEARALL, 0, 0);
	
			::SendMessage(currentScintilla, SCI_APPENDTEXT, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text));
			::SendMessage(currentScintilla, SCI_ENDUNDOACTION, 0, 0);
		}
	
		sendMessageNpp(NPPM_DESTROYSCINTILLAHANDLE, 0, reinterpret_cast<LPARAM>(&writeContext.hScintilla));
	}

	tidyRelease(tidyDoc);
}




void Tidy2Plugin::createDefaultConfig(const char *configPath)
{
	FILE *defaultConfig = fopen(configPath, "wb");
	if (NULL != defaultConfig)
	{
		fprintf(defaultConfig, "%s", 
			"indent: auto\r\n"
			"indent-spaces: 2\r\n"
			"wrap: 132\r\n"
			"markup: yes\r\n"
			"output-xml: yes\r\n"
			"input-xml: no\r\n"
			"numeric-entities: yes\r\n"
			"quote-marks: yes\r\n"
			"quote-nbsp: yes\r\n"
			"quote-ampersand: no\r\n"
			"break-before-br: no\r\n"
			"uppercase-tags: no\r\n"
			"uppercase-attributes: no\r\n"
			"new-inline-tags: cfif, cfelse, math, mroot, \r\n"
			"  mrow, mi, mn, mo, msqrt, mfrac, msubsup, munderover,\r\n"
			"  munder, mover, mmultiscripts, msup, msub, mtext,\r\n"
			"  mprescripts, mtable, mtr, mtd, mth\r\n"
			"new-blocklevel-tags: cfoutput, cfquery\r\n"
			"new-empty-tags: cfelse\r\n");
		fclose(defaultConfig);
	}
}
