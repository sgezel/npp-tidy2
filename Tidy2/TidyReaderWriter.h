#ifndef TIDYREADERWRITER_H_23409124
#define TIDYREADERWRITER_H_23409124

namespace Tidy2
{
	namespace TidyReaderWriter
	{

		class TidyReadContext {
		public:
			int nextPosition;
			int length;
			HWND hScintilla;
		};

		class TidyWriteContext {
		public:
			HWND hScintilla;
		};



		int TIDY_CALL getByte(void *context) {
			TidyReadContext* tidyContext = reinterpret_cast<TidyReadContext*>(context);

			int returnByte = static_cast<unsigned char>(::SendMessage(tidyContext->hScintilla, SCI_GETCHARAT, tidyContext->nextPosition, 0));
	
			++(tidyContext->nextPosition);

			return returnByte;
		}

		void TIDY_CALL ungetByte(void *context, byte b) {
			TidyReadContext* tidyContext = reinterpret_cast<TidyReadContext*>(context);

			--(tidyContext->nextPosition);

		}

		Bool TIDY_CALL isInputEof(void *context) {
			TidyReadContext* tidyContext = reinterpret_cast<TidyReadContext*>(context);
			if (tidyContext->nextPosition >= tidyContext->length)
			{
				return yes;  // Urgh. Tidy enum.
			}
			else
			{
				return no;
			}
		}

		void TIDY_CALL putByte(void *context, byte b) {
			TidyWriteContext* tidyContext = reinterpret_cast<TidyWriteContext*>(context);
			::SendMessage(tidyContext->hScintilla, SCI_APPENDTEXT, 1, reinterpret_cast<LPARAM>(&b));
		}

}
}
#endif // TIDYREADERWRITER_H_23409124