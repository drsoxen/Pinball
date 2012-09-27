#include "Common.h"

GraphicsDevice* GraphicsDevice::m_GraphicsDevice = 0;

GraphicsDevice::GraphicsDevice()
{
	memset(this, 0, sizeof(GraphicsDevice));
}

GraphicsDevice::~GraphicsDevice()
{
	if(m_SwapChain)ReleaseCOM(m_SwapChain);
	if(m_Device)ReleaseCOM(m_Device);
	if(m_RenderTargetView)ReleaseCOM(m_RenderTargetView);
	if(m_DepthStencilBuffer)ReleaseCOM(m_DepthStencilBuffer);
	if(m_DepthStencilView)ReleaseCOM(m_DepthStencilView);
}

GraphicsDevice* GraphicsDevice::GetInstance()
{
	if(m_GraphicsDevice == 0)
		m_GraphicsDevice = new GraphicsDevice();
	return m_GraphicsDevice;
}

void GraphicsDevice::Destroy()
{
	if (m_GraphicsDevice) 
		delete m_GraphicsDevice;
	m_GraphicsDevice = 0;
}

bool GraphicsDevice::Init(HWND hwnd, int Width, int Height)
{
	LogMessage("Initializing DirectX");

	m_WindowHeight = Height;
	m_WindowWidth = Width;

	CreateSwapChain(hwnd);
	CreateRenderTargetView();
	CreateDepthBufferAndStencilBuffer();
	m_Device->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
	SetViewport();

	return true;
}
void GraphicsDevice::OnResize(int Width, int Height)
{
	if (!m_Device)
	{
		LogError("Resize message received, but DirectX not initialized yet");
		return;
	}

	LogMessage("Window has been resized");

	m_WindowHeight = Width;
	m_WindowWidth = Height;
	ReleaseCOM(m_RenderTargetView);
	ReleaseCOM(m_DepthStencilView);
	ReleaseCOM(m_DepthStencilBuffer);
	LogMessage("Resizing Buffers");

	m_SwapChain->ResizeBuffers(1, Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	CreateRenderTargetView();
	CreateDepthBufferAndStencilBuffer();

	m_Device->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	SetViewport();
}

void GraphicsDevice::CreateSwapChain(HWND hwnd)
{
	LogMessage("Creating my Swap Chain");

	DXGI_SWAP_CHAIN_DESC SwapChain;
	memset(&SwapChain,0,sizeof(DXGI_SWAP_CHAIN_DESC));

	SwapChain.BufferDesc.Height = m_WindowHeight;
	SwapChain.BufferDesc.Width = m_WindowWidth;
	SwapChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChain.BufferDesc.RefreshRate.Denominator = 1;
	SwapChain.BufferDesc.RefreshRate.Numerator = 60;
	SwapChain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; 

	//No Multisampling
	SwapChain.SampleDesc.Count = 1;//number of samples taken per pixel
	SwapChain.SampleDesc.Quality = 0;//quality level of the multisampleing

	SwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//rendering to the back buffer
	SwapChain.BufferCount = 1;//how many back buffers

	SwapChain.OutputWindow = hwnd;//handle to the output window

	SwapChain.Windowed = true;//is going to be run in a window (might cause problems?)

	SwapChain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;//lets the display driver choose the best presentation methood

	SwapChain.Flags = 0;//chooses to use the desktop display mode for fullscreen

	//Make the Device
#ifdef _DEBUG
	D3D10CreateDeviceAndSwapChain(NULL,//uses the primary adapter
		D3D10_DRIVER_TYPE_HARDWARE,//use the computers hardware accelerated graphics 
		NULL,//uses the hardware rasterizer
		D3D10_CREATE_DEVICE_DEBUG,//outputs problems to the output window
		D3D10_SDK_VERSION,								  
		&SwapChain,//our swapchain
		&m_SwapChain,//stores out swapchain
		&m_Device//stores the device
		);
#endif

#ifdef _WINDOWS
	D3D10CreateDeviceAndSwapChain(NULL,//uses the primary adapter
		D3D10_DRIVER_TYPE_HARDWARE,//use the computers hardware accelerated graphics 
		NULL,//uses the hardware rasterizer
		NULL,//outputs problems to the output window
		D3D10_SDK_VERSION,								  
		&SwapChain,//our swapchain
		&m_SwapChain,//stores out swapchain
		&m_Device//stores the device
		);
#endif

}

void GraphicsDevice::CreateRenderTargetView()
{
	LogMessage("Creating Render Target View");

	ID3D10Texture2D* BackBuffer;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&BackBuffer));
	m_Device->CreateRenderTargetView(BackBuffer, 0, &m_RenderTargetView);
	ReleaseCOM(BackBuffer);
}

void GraphicsDevice::CreateDepthBufferAndStencilBuffer()
{
	LogMessage("Creating The Depth Buffer and Stencil Buffer");

	D3D10_TEXTURE2D_DESC Texture2D;
	memset(&Texture2D,0,sizeof(D3D10_TEXTURE2D_DESC));

	Texture2D.Height = m_WindowHeight;
	Texture2D.Width = m_WindowWidth;

	Texture2D.MipLevels = 1;//number of mipmap levels
	Texture2D.ArraySize = 1;//number of textures in the texture array
	Texture2D.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//format of the texels

	//No Multisampling
	Texture2D.SampleDesc.Count = 1;//number of samples taken per pixel
	Texture2D.SampleDesc.Quality = 0;//quality level of the multisampleing

	Texture2D.Usage = D3D10_USAGE_DEFAULT;//says only the GPU can edit the texture not the CPU
	Texture2D.BindFlags = D3D10_BIND_DEPTH_STENCIL;//says we are using depth and stencil


	m_Device->CreateTexture2D(&Texture2D,NULL,&m_DepthStencilBuffer);
	m_Device->CreateDepthStencilView(m_DepthStencilBuffer,NULL,&m_DepthStencilView);
}

void GraphicsDevice::SetViewport()
{
	LogMessage("Creating The ViewPort");

	D3D10_VIEWPORT ViewPort;
	memset(&ViewPort,0,sizeof(D3D10_VIEWPORT));

	ViewPort.Height = m_WindowHeight;
	ViewPort.Width = m_WindowWidth;
	ViewPort.MaxDepth = 1;
	ViewPort.MinDepth = 0;
	ViewPort.TopLeftX = 0;
	ViewPort.TopLeftY = 0;

	m_Device->RSSetViewports(1, &ViewPort);
}

void GraphicsDevice::CreateFont()
{
	D3DX10_FONT_DESC FontDesc;
	memset(&FontDesc,0,sizeof(D3DX10_FONT_DESC));

	FontDesc.Height = 24;
	FontDesc.Width = 0;
	FontDesc.Weight = 0;
	FontDesc.MipLevels = 1;
	FontDesc.Italic = false;
	FontDesc.CharSet = DEFAULT_CHARSET;
	FontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	FontDesc.Quality = DEFAULT_QUALITY;
	FontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	wcscpy(FontDesc.FaceName, L"Times New Roman");

	D3DX10CreateFontIndirect(m_Device, &FontDesc, &m_Font);	
}

void GraphicsDevice::DrawText(int Score, int Balls)
{
	//const D3DXCOLOR DarkGrey(0.3f,0.3f,0.3f,1.0f);//Setting Color
	//RECT ScoreSpace = {5,5,0,0};//Setting a place for the Score to Draw
	//m_Font->DrawTextW(0, L"Score :  " + Score, -1 , &ScoreSpace, DT_NOCLIP, DarkGrey);//Drawing
	//RECT LivesSpace = {5,5,0,10};//Setting a space for the balls
	//m_Font->DrawTextW(0, L"Balls :  " + Balls, -1 , &ScoreSpace, DT_NOCLIP, DarkGrey);//Drawing.
}

void GraphicsDevice::Clear()
{
	float clearColor[] =  { 0.392f, 0.584f, 0.929f, 1.0f };
	//float clearColor[] =  { 0.0f, 0.0, 0.0f, 1.0f };
	m_Device->ClearRenderTargetView(m_RenderTargetView, clearColor);
	m_Device->ClearDepthStencilView(m_DepthStencilView, D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0f, 0);
}

void GraphicsDevice::Present()
{
	m_SwapChain->Present(0,0);
}