class GameManager
{
public:
	GameManager(ID3D10Device* m_GraphicsDevice);

	virtual bool KeyPress();
	virtual bool MousePress();
	virtual bool MouseMove();
	virtual bool MouseWheel(WPARAM wParam);
	virtual void CheckInput(float dt);	

	virtual void Init();
	virtual void Update(float dt);
	virtual void Render();
};