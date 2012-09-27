#include "Common.h"

Window* Window::m_Window = 0;

LRESULT Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
case WM_KEYDOWN:
		//if (!(wParam & 1<<30)) // If this is not a repeated key
		InputManager::GetInstance()->set_KeyPress(wParam);

		if(InputManager::GetInstance()->get_Key(VK_ESCAPE))
			m_Quit= true;

		m_GameManager->KeyPress();
		break;
		
	case WM_MOUSEWHEEL:

		m_GameManager->MouseWheel(wParam);
		break;

	case WM_KEYUP:
		InputManager::GetInstance()->set_KeyRelease(wParam);
		m_GameManager->KeyPress();
		break;

	case WM_MOUSEMOVE:
		InputManager::GetInstance()->set_MouseMove(lParam);
		m_GameManager->MouseMove();
		break;

	case WM_LBUTTONDOWN:
		InputManager::GetInstance()->set_MousePress(LEFTBUTTON);
		m_GameManager->MousePress();
		break;

	case WM_LBUTTONUP:
		InputManager::GetInstance()->set_MouseRelease(LEFTBUTTON);
		m_GameManager->MousePress();
		break;

	case WM_RBUTTONDOWN:
		InputManager::GetInstance()->set_MousePress(RIGHTBUTTON);
		m_GameManager->MousePress();
		break;

	case WM_RBUTTONUP:
		InputManager::GetInstance()->set_MouseRelease(RIGHTBUTTON);
		m_GameManager->MousePress();
		break;

	case WM_MBUTTONDOWN:
		InputManager::GetInstance()->set_MousePress(CENTERBUTTON);
		m_GameManager->MousePress();
		break;

	case WM_MBUTTONUP:
		InputManager::GetInstance()->set_MouseRelease(CENTERBUTTON);
		m_GameManager->MousePress();
		break;

	case WM_SIZE:
		m_ScreenSize.X  = LOWORD(lParam);
		m_ScreenSize.Y = HIWORD(lParam);
		DevConsole::GetInstance()->realignConsole(m_ScreenSize,m_ScreenPosition);

		GraphicsDevice::GetInstance()->OnResize(m_ScreenSize.X, m_ScreenSize.Y);
		break;

	case WM_MOVE:
		m_ScreenPosition.X  = LOWORD(lParam);
		m_ScreenPosition.Y = HIWORD(lParam);
		DevConsole::GetInstance()->realignConsole(m_ScreenSize,m_ScreenPosition);
		break;

	case WM_CREATE:
		break;

	case WM_CLOSE:
		m_Quit = true;
		PostQuitMessage(0);
		break;

	case WM_QUIT:
		m_Quit = true;
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);

}

Window::Window()
{
	memset(this, 0, sizeof(Window));

}

LRESULT CALLBACK Window::StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{   
	if (Window::m_Window)
	{
		return Window::m_Window->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

Window* Window::GetInstance(const HINSTANCE &Instance,const int &ShowCmd,const short &X,const short &Y,const int &Width,const int &Height,const bool &FullScreen)
{
	if(m_Window == 0)
	{
		m_Window = new Window();
	}

	LogMessage ("Making Window");
	
	m_Window->m_Instance = Instance;
	m_Window->m_ScreenSize.X  = Height;
	m_Window->m_ScreenSize.Y  = Width;
	m_Window->m_ScreenPosition.X  = X;
	m_Window->m_ScreenPosition.Y  = Y;

	DWORD Style= WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME;//gives the style of window we will use
	DWORD ExtendedStyle= WS_EX_APPWINDOW;

	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = RGB(1.0,1.0,1.0);
	HBRUSH CustomBrush = CreateBrushIndirect(&logBrush);

	WNDCLASSEX wndClass;
	memset(&wndClass, 0, sizeof(WNDCLASSEX));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.hInstance = Instance;
	wndClass.lpfnWndProc = Window::StaticWindowProc;
	wndClass.style =  CS_HREDRAW | CS_VREDRAW;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); 
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); 
	wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.lpszClassName = L"MyWindowClass";
	wndClass.hbrBackground = CustomBrush;
	RegisterClassEx(&wndClass);

	RECT RectWindow = {0,0,Width,Height};
	AdjustWindowRectEx(&RectWindow,Style,false,ExtendedStyle);
	
	m_Window ->m_HandeToWindow = CreateWindowEx(ExtendedStyle, L"MyWindowClass", L"Group 1 PinBall ", Style, X, Y, RectWindow.right - RectWindow.left, RectWindow.bottom - RectWindow.top, 0, 0, Instance, 0);


	if (!m_Window->m_HandeToWindow)
	{
		LogError("Create Window Failed")

	}

	ShowWindow(m_Window-> m_HandeToWindow, ShowCmd);

	GraphicsDevice::GetInstance()->Init(m_Window->m_HandeToWindow, Width, Height);

	m_Window->m_GameManager = new GameManager(GraphicsDevice::GetInstance()->m_Device);

	return m_Window;
}

bool Window::Destroy()
{   
	if(Window::m_Window)
	{
		LogMessage("Deleting window...");
		delete Window::m_Window;
		Window::m_Window = 0;
		delete DevConsole::GetInstance();
		return true;
	}
	return false;
}

Window::~Window(){}


int Window::MessageLoop()
{
	MSG message;

	memset(&message,0,sizeof(MSG));

	LogMessage("Message loop Entered");
	
	m_GameManager->Init();

	while (!m_Quit) 
	{
		while(PeekMessage(&message,m_HandeToWindow,0,0,PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		GraphicsDevice::GetInstance()->Clear();

		m_GameManager->Update(getElapsedSeconds());
		m_GameManager->Render();

		GraphicsDevice::GetInstance()->Present();
	}
	LogMessage("Exited Message Loop");

	Destroy();
	return message.wParam;

}

float Window::getElapsedSeconds()
{
	float currentTime = float(GetTickCount()) / 1000.0f;
	float seconds = float(currentTime - m_lastTime);
	m_lastTime = currentTime;
	return seconds;
}
