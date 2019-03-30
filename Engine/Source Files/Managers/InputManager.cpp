#include "InputManager.h"

/// <summary>
/// 
/// </summary>
InputManager::InputManager()
{
	mKeyboard = std::make_shared<DirectX::Keyboard>();
	mMouse = std::make_shared<DirectX::Mouse>();
}

/// <summary>
/// 
/// </summary>
InputManager::~InputManager()
{
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
std::shared_ptr<InputManager> InputManager::Instance()
{
	static std::shared_ptr<InputManager> instance{ new InputManager };
	return instance;
}

/// <summary>
/// 
/// </summary>
void InputManager::CenterCursor()
{
	//SetCursorPos(CLIENT_WIDTH / 2, CLIENT_HEIGHT / 2);
}

/// <summary>
/// 
/// </summary>
/// <param name="pVisible"></param>
void InputManager::CursorVisible(bool pVisible)
{
	if (pVisible) { mMouse->SetVisible(true); }
	else { mMouse->SetVisible(true); }
}

/// <summary>
/// 
/// </summary>
/// <param name="pButton"></param>
/// <returns></returns>
bool InputManager::KeyDown(const KEYBOARD_BUTTONS & pButton)
{
	auto key = std::find_if(mKeyboardButtonPresses.begin(), mKeyboardButtonPresses.end(), [&](const std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>& key)
	{
		return key.first == pButton && key.second == KEYBOARD_BUTTON_STATE::KEY_DOWN;
	});
	if (key != mKeyboardButtonPresses.end())
	{
		return true;
	}
	return false;
}

/// <summary>
/// 
/// </summary>
/// <param name="pButton"></param>
/// <returns></returns>
bool InputManager::KeyDown(const MOUSE_BUTTONS & pButton)
{
	auto key = std::find_if(mMouseButtonPresses.begin(), mMouseButtonPresses.end(), [&](const std::pair<MOUSE_BUTTONS, MOUSE_BUTTON_STATE>& key)
	{
		return key.first == pButton && key.second == MOUSE_BUTTON_STATE::MOUSE_DOWN;
	});
	if (key != mMouseButtonPresses.end())
	{
		return true;
	}
	return false;
}

/// <summary>
/// 
/// </summary>
/// <param name="pButton"></param>
/// <returns></returns>
bool InputManager::KeyUp(const KEYBOARD_BUTTONS & pButton)
{
	auto key = std::find_if(mKeyboardButtonPresses.begin(), mKeyboardButtonPresses.end(), [&](const std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>& key)
	{
		return key.first == pButton && key.second == KEYBOARD_BUTTON_STATE::KEY_UP;
	});
	if (key != mKeyboardButtonPresses.end())
	{
		return true;
	}
	return false;
}

/// <summary>
/// 
/// </summary>
/// <param name="pButton"></param>
/// <returns></returns>
bool InputManager::KeyUp(const MOUSE_BUTTONS & pButton)
{
	auto key = std::find_if(mMouseButtonPresses.begin(), mMouseButtonPresses.end(), [&](const std::pair<MOUSE_BUTTONS, MOUSE_BUTTON_STATE>& key)
	{
		return key.first == pButton && key.second == MOUSE_BUTTON_STATE::MOUSE_UP;
	});
	if (key != mMouseButtonPresses.end())
	{
		return true;
	}
	return false;
}

/// <summary>
/// 
/// </summary>
/// <param name="pButton"></param>
/// <returns></returns>
bool InputManager::KeyHeld(const KEYBOARD_BUTTONS & pButton)
{
	auto key = std::find_if(mKeyboardButtonPresses.begin(), mKeyboardButtonPresses.end(), [&](const std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>& key)
	{
		return key.first == pButton && key.second == KEYBOARD_BUTTON_STATE::KEY_HELD;
	});
	if (key != mKeyboardButtonPresses.end())
	{
		return true;
	}
	return false;
}

/// <summary>
/// 
/// </summary>
/// <param name="pButton"></param>
/// <returns></returns>
bool InputManager::KeyHeld(const MOUSE_BUTTONS & pButton)
{
	auto key = std::find_if(mMouseButtonPresses.begin(), mMouseButtonPresses.end(), [&](const std::pair<MOUSE_BUTTONS, MOUSE_BUTTON_STATE>& key)
	{
		return key.first == pButton && key.second == MOUSE_BUTTON_STATE::MOUSE_HELD;
	});
	if (key != mMouseButtonPresses.end())
	{
		return true;
	}
	return false;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
const float & InputManager::ScrollWheel() const
{
	return mMouseWheelValue;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
const float & InputManager::MouseX() const
{
	return mMousePosition.x;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
const float & InputManager::MouseY() const
{
	return mMousePosition.y;
}

/// <summary>
/// 
/// </summary>
/// <param name="pNear"></param>
/// <param name="pFar"></param>
/// <param name="pFOV"></param>
/// <param name="pWindowWidth"></param>
/// <param name="pWindowHeight"></param>
/// <param name="mViewInverse"></param>
void InputManager::RayFromMouse(const float& pNear, const float& pFar, const float& pFOV, const float& pWindowWidth, const float& pWindowHeight, const MathsHelper::Matrix4& pViewInverse,
	MathsHelper::Vector4& pOrigin, MathsHelper::Vector4& pDirection)
{
	float aspectRatio = pWindowWidth / pWindowHeight;
	float widthDiv2 = pWindowWidth * 0.5f;
	float heightDiv2 = pWindowHeight * 0.5f;

	//Normalise and scale mouse co-ords to frustrum
	float x = tanf(pFOV * 0.5f) * (mMousePosition.x / widthDiv2 - 1.0f) / aspectRatio;
	float y = tanf(pFOV * 0.5f) * (1.0f - mMousePosition.y / heightDiv2);

	//Calculate ray
	MathsHelper::Vector4 origin = MathsHelper::Vector4(x * pNear, y * pNear, pNear, 1);
	MathsHelper::Vector4 destination = MathsHelper::Vector4(x * pFar, y * pFar, pFar, 1);

	//MULTIPLY BY INVERSE VIEW MATRIX

	//Return origin and direction of ray
	pOrigin = origin;
	pDirection = MathsHelper::Vector4(destination - origin).Normalise();
}

////--------------------------------------------------------------------------------------
//// Called every time the application receives a message
////--------------------------------------------------------------------------------------

/// <summary>
/// 
/// </summary>
/// <param name="hWnd"></param>
/// <param name="message"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
LRESULT CALLBACK InputManager::WndProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	//mMouse->SetWindow(hWnd);

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
/// 
/// </summary>
void InputManager::Update()
{
	KeyboardInput();
	MouseInput();
}

/// <summary>
/// 
/// </summary>
void InputManager::KeyboardInput()
{
	//Clears key presses from last frame
	mKeyboardButtonPresses.clear();

	//Updates state of keyboard and keyboard tracker
	mKeyboardState = mKeyboard->GetState();
	mKeyboardTracker.Update(mKeyboardState);

	//Methods to gather information from key presses, states and releases
	HeldDownKeys();
	SinglePressKeys();
	ReleasedKeys();
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
void InputManager::MouseInput() 
{
	//Clears mouse presses from last frame
	mMouseButtonPresses.clear();

	mMouseState = mMouse->GetState();
	mMouseTracker.Update(mMouseState);

	//Mouse position
	mMousePosition = DirectX::XMFLOAT2(mMouse->GetState().x, mMouse->GetState().y);

	//Scroll wheel
	mMouseWheelValue = mMouse->GetState().scrollWheelValue;

	//Buttons
	using ButtonState = DirectX::Mouse::ButtonStateTracker::ButtonState;

	//Mouse buttons held
	if (mMouseTracker.leftButton == ButtonState::HELD)
	{ 
		mMouseButtonPresses.push_back(std::pair<MOUSE_BUTTONS, MOUSE_BUTTON_STATE>(MOUSE_BUTTONS::MOUSE_BUTTON_LEFT, MOUSE_BUTTON_STATE::MOUSE_HELD));
	}
	if (mMouseTracker.rightButton == ButtonState::HELD) 
	{ 
		mMouseButtonPresses.push_back(std::pair<MOUSE_BUTTONS, MOUSE_BUTTON_STATE>(MOUSE_BUTTONS::MOUSE_BUTTON_RIGHT, MOUSE_BUTTON_STATE::MOUSE_HELD));
	}
	if (mMouseTracker.middleButton == ButtonState::HELD)
	{ 
		mMouseButtonPresses.push_back(std::pair<MOUSE_BUTTONS, MOUSE_BUTTON_STATE>(MOUSE_BUTTONS::MOUSE_BUTTON_MIDDLE, MOUSE_BUTTON_STATE::MOUSE_HELD));
	}

	//Mouse buttons down
	if (mMouseTracker.leftButton == ButtonState::PRESSED) 
	{
		mMouseButtonPresses.push_back(std::pair<MOUSE_BUTTONS, MOUSE_BUTTON_STATE>(MOUSE_BUTTONS::MOUSE_BUTTON_LEFT, MOUSE_BUTTON_STATE::MOUSE_DOWN));
	}
	if (mMouseTracker.rightButton == ButtonState::PRESSED) 
	{ 
		mMouseButtonPresses.push_back(std::pair<MOUSE_BUTTONS, MOUSE_BUTTON_STATE>(MOUSE_BUTTONS::MOUSE_BUTTON_RIGHT, MOUSE_BUTTON_STATE::MOUSE_DOWN));
	}
	if (mMouseTracker.middleButton == ButtonState::PRESSED) 
	{ 
		mMouseButtonPresses.push_back(std::pair<MOUSE_BUTTONS, MOUSE_BUTTON_STATE>(MOUSE_BUTTONS::MOUSE_BUTTON_MIDDLE, MOUSE_BUTTON_STATE::MOUSE_DOWN));
	}

	//Mouse buttons up
	if (mMouseTracker.leftButton == ButtonState::RELEASED) 
	{
		mMouseButtonPresses.push_back(std::pair<MOUSE_BUTTONS, MOUSE_BUTTON_STATE>(MOUSE_BUTTONS::MOUSE_BUTTON_LEFT, MOUSE_BUTTON_STATE::MOUSE_UP));
	}
	if (mMouseTracker.rightButton == ButtonState::RELEASED) 
	{ 
		mMouseButtonPresses.push_back(std::pair<MOUSE_BUTTONS, MOUSE_BUTTON_STATE>(MOUSE_BUTTONS::MOUSE_BUTTON_RIGHT, MOUSE_BUTTON_STATE::MOUSE_UP));
	}
	if (mMouseTracker.middleButton == ButtonState::RELEASED)
	{ 
		mMouseButtonPresses.push_back(std::pair<MOUSE_BUTTONS, MOUSE_BUTTON_STATE>(MOUSE_BUTTONS::MOUSE_BUTTON_MIDDLE, MOUSE_BUTTON_STATE::MOUSE_UP));
	}
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
void InputManager::ReleasedKeys()
{
	//Letters
	if (mKeyboardTracker.released.A) 
	{ 
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_A, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.B) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_B, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.C) 
	{ 
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_C, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.D)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_D, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.E) 
	{ 
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_E, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F) 
	{ 
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.G) 
	{ 
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_G, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.H) 
	{ 
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_H, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.I) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_I, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.J) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_J, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.K)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_K, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.L)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_L, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.M)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_M, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.N)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_N, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.O)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_O, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.P)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_P, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Q)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_Q, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.R)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_R, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.S)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_S, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.T)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_T, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.U)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_U, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.V)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_V, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.W)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_W, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.X)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_X, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Y)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_Y, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Z)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_Z, KEYBOARD_BUTTON_STATE::KEY_UP));
	}

	//Numpad
	if (mKeyboardTracker.released.NumPad0)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_0, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.NumPad1)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_1, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.NumPad2)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_2, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.NumPad3)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_3, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.NumPad4)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_4, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.NumPad5)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_5, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.NumPad6)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_6, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.NumPad7)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_7, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.NumPad8)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_8, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.NumPad9)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_9, KEYBOARD_BUTTON_STATE::KEY_UP));
	}

	//Numbers
	if (mKeyboardTracker.released.D0)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_0, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.D1)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_1, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.D2)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_2, KEYBOARD_BUTTON_STATE::KEY_UP));
	}																										
	if (mKeyboardTracker.released.D3)																		
	{																										
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_3, KEYBOARD_BUTTON_STATE::KEY_UP));
	}																									
	if (mKeyboardTracker.released.D4)																	
	{																									
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_4, KEYBOARD_BUTTON_STATE::KEY_UP));
	}																									
	if (mKeyboardTracker.released.D5)																	
	{																									
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_5, KEYBOARD_BUTTON_STATE::KEY_UP));
	}																									
	if (mKeyboardTracker.released.D6)																	
	{																									
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_6, KEYBOARD_BUTTON_STATE::KEY_UP));
	}																								
	if (mKeyboardTracker.released.D7)																
	{																								
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_7, KEYBOARD_BUTTON_STATE::KEY_UP));
	}																								
	if (mKeyboardTracker.released.D8)																
	{																								
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_8, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.D9)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_9, KEYBOARD_BUTTON_STATE::KEY_UP));
	}

	//Maths symbols
	if (mKeyboardTracker.released.Add)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_ADD, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Subtract)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_SUBTRACT, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Multiply)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_MULTIPLY, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Divide) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_DIVIDE, KEYBOARD_BUTTON_STATE::KEY_UP));
	}

	//Arrow keys
	if (mKeyboardTracker.released.Up) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_UP_ARROW, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Left) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_LEFT_ARROW, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Right) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_RIGHT_ARROW, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Down) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_DOWN_ARROW, KEYBOARD_BUTTON_STATE::KEY_UP));
	}

	//F keys
	if (mKeyboardTracker.released.F1) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F1, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F2) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F2, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F3) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F3, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F4) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F4, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F5) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F5, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F6) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F6, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F7) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F7, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F8) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F8, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F9) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F9, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F10)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F10, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F11)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F11, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.F12)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F12, KEYBOARD_BUTTON_STATE::KEY_UP));
	}

	//Other keys
	if (mKeyboardTracker.released.Back)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_BACKSPACE, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.CapsLock)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_CAPS_LOCK, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Delete)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_DELETE, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Enter) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_ENTER, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Escape)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_ESC, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.LeftAlt)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_LEFT_ALT, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.LeftControl)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_LEFT_CTRL, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.LeftShift) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_LEFT_SHIFT, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.LeftWindows)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_LEFT_WINDOWS, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.RightAlt) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_RIGHT_ALT, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.RightControl) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_RIGHT_CTRL, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.RightShift)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_RIGHT_SHIFT, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.RightWindows) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_RIGHT_WINDOWS, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Space) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_SPACE, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.Tab) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_TAB, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.OemPeriod)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_PERIOD, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
	if (mKeyboardTracker.released.OemComma)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_COMMA, KEYBOARD_BUTTON_STATE::KEY_UP));
	}
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
void InputManager::SinglePressKeys()
{
	//Letters
	if (mKeyboardTracker.pressed.A)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_A, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.B)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_B, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.C)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_C, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.D)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_D, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.E)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_E, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.G)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_G, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.H)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_H, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.I)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_I, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.J)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_J, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.K)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_K, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.L)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_L, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.M)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_M, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.N)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_N, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.O)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_O, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.P)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_P, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Q)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_Q, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.R)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_R, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.S)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_S, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.T)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_T, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.U)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_U, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.V)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_V, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.W)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_W, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.X)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_X, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Y)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_Y, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Z)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_Z, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}

	//Numpad
	if (mKeyboardTracker.pressed.NumPad0)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_0, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.NumPad1)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_1, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.NumPad2)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_2, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.NumPad3)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_3, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.NumPad4)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_4, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.NumPad5)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_5, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.NumPad6)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_6, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.NumPad7)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_7, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.NumPad8)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_8, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.NumPad9)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_9, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}

	//Numbers
	if (mKeyboardTracker.pressed.D0)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_0, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.D1)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_1, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.D2)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_2, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.D3)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_3, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.D4)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_4, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.D5)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_5, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.D6)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_6, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.D7)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_7, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.D8)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_8, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.D9)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_9, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}

	//Maths symbols
	if (mKeyboardTracker.pressed.Add)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_ADD, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Subtract)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_SUBTRACT, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Multiply)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_MULTIPLY, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Divide)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_DIVIDE, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}

	//Arrow keys
	if (mKeyboardTracker.pressed.Up)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_UP_ARROW, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Left)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_LEFT_ARROW, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Right)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_RIGHT_ARROW, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Down)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_DOWN_ARROW, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}

	//F keys
	if (mKeyboardTracker.pressed.F1)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F1, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F2)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F2, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F3)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F3, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F4)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F4, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F5)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F5, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F6)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F6, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F7)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F7, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F8)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F8, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F9)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F9, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F10)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F10, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F11)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F11, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.F12)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F12, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}

	//Other keys
	if (mKeyboardTracker.pressed.Back)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_BACKSPACE, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.CapsLock)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_CAPS_LOCK, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Delete)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_DELETE, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Enter)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_ENTER, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Escape)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_ESC, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.LeftAlt)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_LEFT_ALT, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.LeftControl)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_LEFT_CTRL, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.LeftShift)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_LEFT_SHIFT, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.LeftWindows)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_LEFT_WINDOWS, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.RightAlt)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_RIGHT_ALT, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.RightControl)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_RIGHT_CTRL, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.RightShift)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_RIGHT_SHIFT, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.RightWindows)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_RIGHT_WINDOWS, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Space)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_SPACE, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.pressed.Tab)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_TAB, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.released.OemPeriod)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_PERIOD, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
	if (mKeyboardTracker.released.OemComma)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_COMMA, KEYBOARD_BUTTON_STATE::KEY_DOWN));
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="state"></param>
/// <returns></returns>
void InputManager::HeldDownKeys()
{
	//Letters
	if (mKeyboardState.A) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_A, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.B) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_B, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.C) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_C, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.D) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_D, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.E) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_E, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.F) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.G) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_G, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.H) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_H, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.I) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_I, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.J) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_J, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.K) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_K, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.L) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_L, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.M) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_M, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.N) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_N, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.O) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_O, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.P) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_P, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.Q) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_Q, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.R) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_R, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.S) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_S, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.T) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_T, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.U) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_U, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.V) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_V, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.W) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_W, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.X) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_X, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.Y) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_Y, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.Z) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_Z, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}

	//Numpad
	if (mKeyboardState.NumPad0)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_0, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.NumPad1)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_1, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.NumPad2)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_2, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.NumPad3)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_3, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.NumPad4)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_4, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.NumPad5)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_5, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.NumPad6)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_6, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.NumPad7)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_7, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.NumPad8)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_8, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.NumPad9)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::NUM_PAD_KEY_9, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}

	//Numbers
	if (mKeyboardState.D0)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_0, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.D1)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_1, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.D2)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_2, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.D3)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_3, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.D4)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_4, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.D5)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_5, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.D6)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_6, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.D7)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_7, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.D8)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_8, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.D9)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_9, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}

	//Arrow keys
	if (mKeyboardState.Up) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_UP_ARROW, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.Left) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_LEFT_ARROW, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.Right)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_RIGHT_ARROW, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.Down) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_DOWN_ARROW, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}

	//Maths symbols
	if (mKeyboardState.Multiply)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_MULTIPLY, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.Subtract) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_SUBTRACT, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.Add)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_ADD, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.Divide)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_DIVIDE, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}

	//F keys
	if (mKeyboardState.F1) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F1, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.F2) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F2, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.F3) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F3, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.F4) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F4, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.F5) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F5, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.F6) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F6, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.F7) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F7, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.F8) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F8, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.F9) 
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F9, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.F10)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F10, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.F11)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F11, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.F12)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_F12, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}

	//Other keys
	if (mKeyboardState.Back)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_BACKSPACE, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.CapsLock)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_CAPS_LOCK, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.Delete)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_DELETE, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.Enter)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_ENTER, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.Escape)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_ESC, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.LeftAlt)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_LEFT_ALT, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.LeftControl)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_LEFT_CTRL, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.LeftShift)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_LEFT_SHIFT, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.LeftWindows)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_LEFT_WINDOWS, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.RightAlt)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_RIGHT_ALT, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.RightControl)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_RIGHT_CTRL, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.RightShift)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_RIGHT_SHIFT, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.RightWindows)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_RIGHT_WINDOWS, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.Space)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_SPACE, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.Tab)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_TAB, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.OemPeriod)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_PERIOD, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
	if (mKeyboardState.OemComma)
	{
		mKeyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(KEYBOARD_BUTTONS::KEY_COMMA, KEYBOARD_BUTTON_STATE::KEY_HELD));
	}
}