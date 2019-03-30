#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "MathsHelper.h"

//Mouse buttons enum
enum class MOUSE_BUTTONS
{
	MOUSE_BUTTON_LEFT, MOUSE_BUTTON_RIGHT, MOUSE_BUTTON_MIDDLE, NO_MOUSE_BUTTON,
};

//Keyboard buttons enum
enum class KEYBOARD_BUTTONS
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

	NO_KEYBOARD_BUTTON
};

//Mouse button states enum
enum class MOUSE_BUTTON_STATE
{
	MOUSE_DOWN, MOUSE_UP, MOUSE_HELD
};

//Keyboard button states enum
enum class KEYBOARD_BUTTON_STATE
{
	KEY_DOWN, KEY_UP, KEY_HELD
};

class InputManager
{
protected:
	//Keyboard
	std::vector<std::pair<MOUSE_BUTTONS, MOUSE_BUTTON_STATE>> mMouseButtonPresses;

	//Mouse
	std::vector<std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>> mKeyboardButtonPresses;
	MathsHelper::Vector2 mMousePosition; //Make vector 2 struct for this
	float mMouseWheelValue;

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
	bool KeyDown(const KEYBOARD_BUTTONS& pButton);
	bool KeyDown(const MOUSE_BUTTONS& pButton);

	//Get key releases this frame
	bool KeyUp(const KEYBOARD_BUTTONS& pButton);
	bool KeyUp(const MOUSE_BUTTONS& pButton);

	//Get keys held this frame
    bool KeyHeld(const KEYBOARD_BUTTONS& pButton);
	bool KeyHeld(const MOUSE_BUTTONS& pButton);

	//Mouse
	const float& ScrollWheel() const;
	const MathsHelper::Vector2& MousePos() const;
	void RayFromMouse(const float& pNear, const float& pFar, const float& pFOV, const float& pWindowWidth, const float& pWindowHeight, const MathsHelper::Matrix4& pViewInverse,
		MathsHelper::Vector4& pOrigin, MathsHelper::Vector4& pDirection);
};