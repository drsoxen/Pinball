class Window
{
public:
	static Window* m_Window;

	static Window* GetInstance(const HINSTANCE &Instance,const int &ShowCmd,const short &X,const short &Y,const int &Width,const int &Height,const bool &FullScreen);
	static bool Destroy();
	static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	int MessageLoop();
	float getElapsedSeconds();


protected:

	Window();
	~Window();

	GameManager * m_GameManager;

	HINSTANCE m_Instance;
	COORD m_ScreenSize;
	COORD m_ScreenPosition;
	bool m_Quit;
	HWND m_HandeToWindow;
	float m_lastTime;

	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


};