#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "KodeboldsMath.h"

//Keys enum
enum class KEYS
{
	//LETTERS
	KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M,
	KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,

	//F KEYS
	KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10,
	KEY_F11, KEY_F12,

	//NUMBERS
	KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
	NUM_PAD_KEY_0, NUM_PAD_KEY_1, NUM_PAD_KEY_2, NUM_PAD_KEY_3, NUM_PAD_KEY_4, NUM_PAD_KEY_5, NUM_PAD_KEY_6, NUM_PAD_KEY_7, NUM_PAD_KEY_8, NUM_PAD_KEY_9,

	//ARROWS
	KEY_DOWN_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_UP_ARROW,

	//MATHS SYMBOLS
	KEY_ADD, KEY_SUBTRACT, KEY_MULTIPLY, KEY_DIVIDE,

	//OTHER
	KEY_LEFT_SHIFT, KEY_RIGHT_SHIFT, KEY_SPACE, KEY_BACKSPACE, KEY_LEFT_CTRL, KEY_RIGHT_CTRL, KEY_LEFT_ALT, KEY_RIGHT_ALT, KEY_ENTER,
	KEY_ESC, KEY_LEFT_WINDOWS, KEY_RIGHT_WINDOWS, KEY_CAPS_LOCK, KEY_DELETE, KEY_TAB, KEY_PERIOD, KEY_COMMA,

	//MOUSE BUTTONS
	MOUSE_BUTTON_LEFT, MOUSE_BUTTON_RIGHT, MOUSE_BUTTON_MIDDLE
};

//Key states enum
enum class KEY_STATE
{
	KEY_DOWN, KEY_UP, KEY_HELD
};

class InputManager
{
protected:
	//Mouse and key states
	std::vector<std::pair<KEYS, KEY_STATE>> mKeyStates;
	KodeboldsMath::Vector2 mMousePosition;
	int mMouseWheelValue;

	virtual void KeyboardInput() = 0;
	virtual void SinglePressKeys() = 0;
	virtual void ReleasedKeys() = 0;
	virtual void HeldDownKeys() = 0;
	virtual void MouseInput() = 0;

	//Private constructor for singleton pattern
	InputManager();

public:
	virtual ~InputManager();

	void Update();

	//Singleton pattern
	//Deleted copy constructor and assignment operator so no copies of the singleton instance can be made
	InputManager(InputManager const&) = delete;
	InputManager& operator=(InputManager const&) = delete;

	virtual void CenterCursor() = 0;
	virtual void CursorVisible(bool pVisible) = 0;

	//Get key presses this frame
	bool KeyDown(const KEYS& pButton);

	//Get key releases this frame
	bool KeyUp(const KEYS& pButton);

	//Get keys held this frame
    bool KeyHeld(const KEYS& pButton);

	//Mouse
	const int ScrollWheel() const;
	const KodeboldsMath::Vector2& MousePos() const;
	const KodeboldsMath::Vector4 RayFromMouse(const KodeboldsMath::Matrix4& pViewInverse, const KodeboldsMath::Matrix4& pProjInverse, const float& pWidth, const float& pHeight);
};