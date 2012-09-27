#include "Common.h"


DevConsole* DevConsole::m_DevConsole = 0;

DevConsole::DevConsole()
{
	AllocConsole();
	SetConsoleTitle(L"Dev Console ZDirectX");

	cursorInfo.bVisible = false;
	cursorInfo.dwSize = 10;
	SetConsoleCursorInfo(mOutHandle,&cursorInfo);

	mOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleHandle = _open_osfhandle((long)mOutHandle, _O_TEXT);
	fp = _fdopen( ConsoleHandle, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );

	mOutHandle = GetStdHandle(STD_ERROR_HANDLE);
	ConsoleHandle = _open_osfhandle((long)mOutHandle, _O_TEXT);
	fp = _fdopen( ConsoleHandle, "w" );
	*stderr = *fp;
	setvbuf( stderr, NULL, _IONBF, 0 );

	mInHandle = GetStdHandle(STD_INPUT_HANDLE);
	ConsoleHandle = _open_osfhandle((long)mOutHandle, _O_TEXT);
	fp = _fdopen( ConsoleHandle, "r" );
	*stdin = *fp;
	setvbuf( stdin, NULL, _IONBF, 0 );

	TheCoolerConsoleHandle = GetConsoleWindow();

	ConsolePosition.X = 640;
	ConsolePosition.Y = 0;
}

DevConsole::~DevConsole()
{
	FreeConsole();
}

DevConsole* DevConsole::GetInstance()
{
	if(m_DevConsole == 0)
		m_DevConsole = new DevConsole();
	return m_DevConsole;
}

COORD DevConsole::realignConsole(COORD Size,COORD Position)
{
	ConsolePosition.X = Position.X + Size.X;
	ConsolePosition.Y = Position.Y;

	GetConsoleScreenBufferInfo(TheCoolerConsoleHandle, &coninfo);

	GetWindowRect((HWND)TheCoolerConsoleHandle, &WinRect);
	MoveWindow((HWND)TheCoolerConsoleHandle, ConsolePosition.X+5, ConsolePosition.Y-30, WinRect.right - WinRect.left, WinRect.bottom - WinRect.top, TRUE);
	
	return ConsolePosition;
}

void DevConsole::logMessage(const char* message, unsigned short Colour)
{
	SetConsoleTextAttribute(mOutHandle, Colour);
	cout << message;
	cout << "\n";
}

void DevConsole::logFormattedMessage(char *message, ...)
{
	SetConsoleTextAttribute(mOutHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	//http://vijayinterviewquestions.blogspot.com/2007/07/write-your-own-printf-function-in-c.html
	
	char *p;
	int i;
	char * s;
	char c;
	double d;
	char fmtbuf[256];

	va_list args;
	va_start(args, message);

	for(p = message;*p!='\0';p++)
	{
		if(*p != '%')
			putchar(*p);
		else
		{
			p++;
			switch(*p)
			{
			case 'd':
				d = va_arg(args, double);
				cout<<d;
				break;

			case 'f':
				d = va_arg(args, double);
				cout<<d;
				break;

			case 's':
				s = va_arg(args, char *);
				cout<<s;
				break;

			case 'c':
				c = va_arg(args, char);
				cout<<c;
				break;

			case 'i':				
				i = va_arg(args, int);
				cout<<i;
				break;

			case '%':
				putchar('%');
				break;
			}
		}
	}

	va_end(args);

}

void DevConsole::logError(const char* error, unsigned short Colour)
{
	SetConsoleTextAttribute(mOutHandle, Colour);
	cerr << error;
	cerr << "\n";
}

void DevConsole::SystemCommand(const char* Command)
{
	SetConsoleTextAttribute(mOutHandle, 0x0007 /*| FOREGROUND_INTENSITY*/);
	system(Command);
}

void DevConsole::gotoxy( short x, short y )
{ 
	COORD position = { x, y };
	SetConsoleCursorPosition( mOutHandle, position );
} 