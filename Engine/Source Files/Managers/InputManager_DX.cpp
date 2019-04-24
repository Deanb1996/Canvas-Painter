#include "InputManager_DX.h"

/// <summary>
/// Constructor
/// Sets pointers to DirectX mouse and keyboard
/// </summary>
InputManager_DX::InputManager_DX() : InputManager()
{
	mKeyboard = std::make_shared<DirectX::Keyboard>();
	mMouse = std::make_shared<DirectX::Mouse>();
}

/// <summary>
/// Default destructor
/// </summary>
InputManager_DX::~InputManager_DX()
{
}

/// <summary>
/// Creates a singleton instance of Input Manager if one hasn't been created before
/// Returns pointer to the instance of Input Manager
/// </summary>
/// <returns>Shared pointer to the Input Manager instance</returns>
std::shared_ptr<InputManager_DX> InputManager_DX::Instance()
{
	static std::shared_ptr<InputManager_DX> instance{ new InputManager_DX };
	return instance;
}

/// <summary>
/// Centers the cursor the the middle of the screen
/// </summary>
void InputManager_DX::CenterCursor()
{
	//NOT IMPLEMENTED
}

/// <summary>
/// Sets the visibility of the cursor
/// </summary>
/// <param name="pVisible">State to set the cursor visibility to</param>
void InputManager_DX::CursorVisible(const bool pVisible)
{
	mMouse->SetVisible(pVisible);
}


////--------------------------------------------------------------------------------------
//// Called every time the application receives a message
////--------------------------------------------------------------------------------------
LRESULT CALLBACK InputManager_DX::WndProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	//Tells anttweak to handle events relevant to anttweak
	//if (TwEventWin(hWnd, message, wParam, lParam))
	//{
	//	return 0;
	//}

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_ACTIVATEAPP:
		DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);
		break;

		// MOUSE
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);
		break;

		// KEYBOARD
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/// <summary>
/// Updates the state of the keyboard and calls the three methods for tracking keyboard inputs
/// </summary>
void InputManager_DX::KeyboardInput()
{
	//Updates state of keyboard and keyboard tracker
	mKeyboardState = mKeyboard->GetState();
	mKeyboardTracker.Update(mKeyboardState);

	//Methods to gather information from key presses, states and releases
	HeldDownKeys();
	SinglePressKeys();
	ReleasedKeys();
}

/// <summary>
/// Updates all the information about the mouse input based on information reveived from the DirectXTK
/// </summary>
void InputManager_DX::MouseInput()
{
	mMouseState = mMouse->GetState();
	mMouseTracker.Update(mMouseState);

	//Mouse position
	mMousePosition = KodeboldsMath::Vector2(static_cast<float>(mMouse->GetState().x), static_cast<float>(mMouse->GetState().y));

	//Scroll wheel
	mMouseWheelValue = mMouse->GetState().scrollWheelValue;

	//Buttons
	using ButtonState = DirectX::Mouse::ButtonStateTracker::ButtonState;

	//Mouse buttons held
	if (mMouseTracker.leftButton == ButtonState::HELD)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::MOUSE_BUTTON_LEFT, KEY_STATE::KEY_HELD));
	}
	if (mMouseTracker.rightButton == ButtonState::HELD)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::MOUSE_BUTTON_RIGHT, KEY_STATE::KEY_HELD));
	}
	if (mMouseTracker.middleButton == ButtonState::HELD)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::MOUSE_BUTTON_MIDDLE, KEY_STATE::KEY_HELD));
	}

	//Mouse buttons down
	if (mMouseTracker.leftButton == ButtonState::PRESSED)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::MOUSE_BUTTON_LEFT, KEY_STATE::KEY_DOWN));
	}
	if (mMouseTracker.rightButton == ButtonState::PRESSED)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::MOUSE_BUTTON_RIGHT, KEY_STATE::KEY_DOWN));
	}
	if (mMouseTracker.middleButton == ButtonState::PRESSED)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::MOUSE_BUTTON_MIDDLE, KEY_STATE::KEY_DOWN));
	}

	//Mouse buttons up
	if (mMouseTracker.leftButton == ButtonState::RELEASED)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::MOUSE_BUTTON_LEFT, KEY_STATE::KEY_UP));
	}
	if (mMouseTracker.rightButton == ButtonState::RELEASED)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::MOUSE_BUTTON_RIGHT, KEY_STATE::KEY_UP));
	}
	if (mMouseTracker.middleButton == ButtonState::RELEASED)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::MOUSE_BUTTON_MIDDLE, KEY_STATE::KEY_UP));
	}
}

/// <summary>
/// Updates all the information about released keys this frame based on information reveived from the DirectXTK
/// </summary>
void InputManager_DX::ReleasedKeys()
{
	//Letters
	if (mKeyboardTracker.released.A)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_A, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.B)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_B, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.C)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_C, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.D)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_D, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.E)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_E, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.G)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_G, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.H)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_H, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.I)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_I, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.J)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_J, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.K)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_K, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.L)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_L, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.M)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_M, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.N)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_N, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.O)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_O, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.P)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_P, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Q)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_Q, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.R)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_R, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.S)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_S, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.T)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_T, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.U)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_U, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.V)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_V, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.W)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_W, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.X)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_X, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Y)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_Y, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Z)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_Z, KEY_STATE::KEY_UP));
	}

	//Numpad
	if (mKeyboardTracker.released.NumPad0)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_0, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.NumPad1)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_1, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.NumPad2)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_2, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.NumPad3)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_3, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.NumPad4)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_4, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.NumPad5)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_5, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.NumPad6)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_6, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.NumPad7)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_7, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.NumPad8)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_8, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.NumPad9)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_9, KEY_STATE::KEY_UP));
	}

	//Numbers
	if (mKeyboardTracker.released.D0)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_0, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.D1)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_1, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.D2)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_2, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.D3)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_3, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.D4)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_4, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.D5)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_5, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.D6)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_6, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.D7)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_7, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.D8)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_8, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.D9)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_9, KEY_STATE::KEY_UP));
	}

	//Maths symbols
	if (mKeyboardTracker.released.Add)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_ADD, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Subtract)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_SUBTRACT, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Multiply)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_MULTIPLY, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Divide)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_DIVIDE, KEY_STATE::KEY_UP));
	}

	//Arrow keys
	if (mKeyboardTracker.released.Up)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_UP_ARROW, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Left)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_LEFT_ARROW, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Right)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_RIGHT_ARROW, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Down)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_DOWN_ARROW, KEY_STATE::KEY_UP));
	}

	//F keys
	if (mKeyboardTracker.released.F1)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F1, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F2)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F2, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F3)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F3, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F4)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F4, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F5)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F5, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F6)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F6, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F7)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F7, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F8)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F8, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F9)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F9, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F10)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F10, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F11)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F11, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F12)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F12, KEY_STATE::KEY_UP));
	}

	//Other keys
	if (mKeyboardTracker.released.Back)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_BACKSPACE, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.CapsLock)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_CAPS_LOCK, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Delete)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_DELETE, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Enter)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_ENTER, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Escape)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_ESC, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.LeftAlt)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_LEFT_ALT, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.LeftControl)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_LEFT_CTRL, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.LeftShift)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_LEFT_SHIFT, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.LeftWindows)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_LEFT_WINDOWS, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.RightAlt)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_RIGHT_ALT, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.RightControl)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_RIGHT_CTRL, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.RightShift)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_RIGHT_SHIFT, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.RightWindows)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_RIGHT_WINDOWS, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Space)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_SPACE, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Tab)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_TAB, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.OemPeriod)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_PERIOD, KEY_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.OemComma)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_COMMA, KEY_STATE::KEY_UP));
	}
}

/// <summary>
/// Updates all the information about pressed keys this frame based on information reveived from the DirectXTK
/// </summary>
void InputManager_DX::SinglePressKeys()
{
	//Letters
	if (mKeyboardTracker.pressed.A)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_A, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.B)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_B, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.C)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_C, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.D)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_D, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.E)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_E, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.G)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_G, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.H)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_H, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.I)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_I, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.J)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_J, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.K)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_K, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.L)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_L, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.M)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_M, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.N)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_N, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.O)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_O, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.P)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_P, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Q)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_Q, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.R)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_R, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.S)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_S, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.T)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_T, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.U)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_U, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.V)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_V, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.W)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_W, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.X)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_X, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Y)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_Y, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Z)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_Z, KEY_STATE::KEY_DOWN));
	}

	//Numpad
	if (mKeyboardTracker.pressed.NumPad0)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_0, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.NumPad1)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_1, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.NumPad2)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_2, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.NumPad3)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_3, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.NumPad4)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_4, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.NumPad5)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_5, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.NumPad6)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_6, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.NumPad7)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_7, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.NumPad8)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_8, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.NumPad9)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_9, KEY_STATE::KEY_DOWN));
	}

	//Numbers
	if (mKeyboardTracker.pressed.D0)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_0, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.D1)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_1, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.D2)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_2, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.D3)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_3, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.D4)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_4, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.D5)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_5, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.D6)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_6, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.D7)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_7, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.D8)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_8, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.D9)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_9, KEY_STATE::KEY_DOWN));
	}

	//Maths symbols
	if (mKeyboardTracker.pressed.Add)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_ADD, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Subtract)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_SUBTRACT, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Multiply)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_MULTIPLY, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Divide)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_DIVIDE, KEY_STATE::KEY_DOWN));
	}

	//Arrow keys
	if (mKeyboardTracker.pressed.Up)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_UP_ARROW, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Left)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_LEFT_ARROW, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Right)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_RIGHT_ARROW, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Down)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_DOWN_ARROW, KEY_STATE::KEY_DOWN));
	}

	//F keys
	if (mKeyboardTracker.pressed.F1)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F1, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F2)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F2, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F3)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F3, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F4)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F4, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F5)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F5, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F6)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F6, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F7)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F7, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F8)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F8, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F9)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F9, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F10)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F10, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F11)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F11, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F12)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F12, KEY_STATE::KEY_DOWN));
	}

	//Other keys
	if (mKeyboardTracker.pressed.Back)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_BACKSPACE, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.CapsLock)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_CAPS_LOCK, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Delete)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_DELETE, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Enter)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_ENTER, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Escape)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_ESC, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.LeftAlt)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_LEFT_ALT, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.LeftControl)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_LEFT_CTRL, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.LeftShift)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_LEFT_SHIFT, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.LeftWindows)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_LEFT_WINDOWS, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.RightAlt)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_RIGHT_ALT, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.RightControl)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_RIGHT_CTRL, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.RightShift)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_RIGHT_SHIFT, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.RightWindows)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_RIGHT_WINDOWS, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Space)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_SPACE, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Tab)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_TAB, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.released.OemPeriod)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_PERIOD, KEY_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.released.OemComma)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_COMMA, KEY_STATE::KEY_DOWN));
	}
}

/// <summary>
/// Updates all the information about held down keys this frame based on information reveived from the DirectXTK
/// </summary>
void InputManager_DX::HeldDownKeys()
{
	//Letters
	if (mKeyboardState.A)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_A, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.B)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_B, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.C)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_C, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.D)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_D, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.E)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_E, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.F)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.G)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_G, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.H)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_H, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.I)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_I, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.J)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_J, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.K)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_K, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.L)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_L, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.M)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_M, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.N)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_N, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.O)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_O, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.P)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_P, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.Q)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_Q, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.R)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_R, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.S)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_S, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.T)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_T, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.U)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_U, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.V)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_V, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.W)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_W, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.X)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_X, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.Y)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_Y, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.Z)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_Z, KEY_STATE::KEY_HELD));
	}

	//Numpad
	if (mKeyboardState.NumPad0)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_0, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.NumPad1)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_1, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.NumPad2)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_2, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.NumPad3)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_3, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.NumPad4)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_4, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.NumPad5)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_5, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.NumPad6)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_6, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.NumPad7)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_7, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.NumPad8)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_8, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.NumPad9)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::NUM_PAD_KEY_9, KEY_STATE::KEY_HELD));
	}

	//Numbers
	if (mKeyboardState.D0)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_0, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.D1)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_1, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.D2)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_2, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.D3)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_3, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.D4)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_4, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.D5)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_5, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.D6)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_6, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.D7)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_7, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.D8)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_8, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.D9)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_9, KEY_STATE::KEY_HELD));
	}

	//Arrow keys
	if (mKeyboardState.Up)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_UP_ARROW, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.Left)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_LEFT_ARROW, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.Right)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_RIGHT_ARROW, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.Down)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_DOWN_ARROW, KEY_STATE::KEY_HELD));
	}

	//Maths symbols
	if (mKeyboardState.Multiply)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_MULTIPLY, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.Subtract)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_SUBTRACT, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.Add)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_ADD, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.Divide)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_DIVIDE, KEY_STATE::KEY_HELD));
	}

	//F keys
	if (mKeyboardState.F1)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F1, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.F2)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F2, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.F3)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F3, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.F4)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F4, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.F5)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F5, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.F6)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F6, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.F7)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F7, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.F8)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F8, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.F9)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F9, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.F10)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F10, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.F11)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F11, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.F12)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_F12, KEY_STATE::KEY_HELD));
	}

	//Other keys
	if (mKeyboardState.Back)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_BACKSPACE, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.CapsLock)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_CAPS_LOCK, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.Delete)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_DELETE, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.Enter)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_ENTER, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.Escape)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_ESC, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.LeftAlt)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_LEFT_ALT, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.LeftControl)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_LEFT_CTRL, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.LeftShift)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_LEFT_SHIFT, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.LeftWindows)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_LEFT_WINDOWS, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.RightAlt)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_RIGHT_ALT, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.RightControl)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_RIGHT_CTRL, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.RightShift)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_RIGHT_SHIFT, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.RightWindows)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_RIGHT_WINDOWS, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.Space)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_SPACE, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.Tab)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_TAB, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.OemPeriod)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_PERIOD, KEY_STATE::KEY_HELD));
	}
	if (mKeyboardState.OemComma)
	{
		mKeyStates.emplace_back(std::pair<KEYS, KEY_STATE>(KEYS::KEY_COMMA, KEY_STATE::KEY_HELD));
	}
}