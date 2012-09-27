class GraphicsDevice
{
public:
	static GraphicsDevice* GetInstance();
	static void Destroy();

	bool Init(HWND hwnd, int Width, int Height);
	void OnResize(int Width, int Height);

	void Clear();
	void Present();

	void DrawText();

	void DrawText(int Score, int Balls);

	ID3D10Device* m_Device;

	ID3D10Device* GetGraphicsDevice() { return m_Device; }

	int GetWindowHeight() { return m_WindowHeight; }
	int GetWindowWidth() { return m_WindowWidth; }

protected:
	GraphicsDevice();
	~GraphicsDevice();
	static GraphicsDevice* m_GraphicsDevice;

	int m_WindowHeight;
	int m_WindowWidth;

	IDXGISwapChain* m_SwapChain;
	ID3D10RenderTargetView* m_RenderTargetView;
	ID3D10DepthStencilView* m_DepthStencilView;
	ID3D10Texture2D* m_DepthStencilBuffer;
	ID3DX10Font * m_Font;

	void CreateSwapChain(HWND hwnd);
	void CreateRenderTargetView();
	void CreateDepthBufferAndStencilBuffer();
	void SetViewport();
	void CreateFont();

};