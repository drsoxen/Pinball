class InputManager
{
public:
	static InputManager* GetInstance();
	static void Destroy();

	void set_MousePress(int MouseButton);
	void set_MouseRelease(int MouseButton);
	void set_KeyPress(WPARAM wParam);
	void set_KeyRelease(WPARAM wParam);
	void set_MouseMove(LPARAM lParam);
	
	bool get_Key(char key);
	bool get_Mouse(int MouseButton);
	
	COORD get_MouseMove();
private:
	static InputManager* m_InputManager;
	COORD MousePos;

	bool isMousePressed;
	bool isMouseReleased;
	bool Keys[256];
	bool MouseButtons[3];

	InputManager();
	~InputManager();
};

