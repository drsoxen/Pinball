class DevConsole
{
public:
	static DevConsole* GetInstance();

	void logFormattedMessage(char *fmt, ...);
	void logMessage(const char* message,unsigned short Colour = FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	void logError(const char* error,unsigned short Colour = FOREGROUND_RED | FOREGROUND_INTENSITY);	
	void SystemCommand(const char* Command);
	COORD realignConsole(COORD Size,COORD Position);

	~DevConsole();

private:
	static DevConsole* m_DevConsole;

	DevConsole();

	void gotoxy( short x, short y );	

	HANDLE mOutHandle;
	HANDLE mInHandle;

	COORD ConsolePosition;

	int ConsoleHandle;
	HWND TheCoolerConsoleHandle;
	RECT WinRect;

	CONSOLE_CURSOR_INFO cursorInfo;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;

	FILE *fp;
};