#include "InputManager_GL.h"

/// <summary>
/// Constructor
/// Sets pointers to DirectX mouse and keyboard
/// </summary>
InputManager_GL::InputManager_GL() : InputManager()
{
}

/// <summary>
/// Default destructor
/// </summary>
InputManager_GL::~InputManager_GL()
{
}

/// <summary>
/// Creates a singleton instance of Input Manager if one hasn't been created before
/// Returns pointer to the instance of Input Manager
/// </summary>
/// <returns>Shared pointer to the Input Manager instance</returns>
std::shared_ptr<InputManager_GL> InputManager_GL::Instance()
{
	static std::shared_ptr<InputManager_GL> instance{ new InputManager_GL };
	return instance;
}

/// <summary>
/// Centers the cursor the the middle of the screen
/// </summary>
void InputManager_GL::CenterCursor()
{
	//NOT IMPLEMENTED
}

/// <summary>
/// Sets the visibility of the cursor
/// </summary>
/// <param name="pVisible">State to set the cursor visibility to</param>
void InputManager_GL::CursorVisible(const bool pVisible)
{
}


////--------------------------------------------------------------------------------------
//// Called every time the application receives a message
////--------------------------------------------------------------------------------------
LRESULT CALLBACK InputManager_GL::WndProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	//Tells anttweak to handle events relevant to anttweak
	if (TwEventWin(hWnd, message, wParam, lParam))
	{
		return 0;
	}

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_ACTIVATEAPP:
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
		break;

		// KEYBOARD
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/// <summary>
/// Updates the state of the keyboard and calls the three methods for tracking keyboard inputs
/// </summary>
void InputManager_GL::KeyboardInput()
{
	//Methods to gather information from key presses, states and releases
	HeldDownKeys();
	SinglePressKeys();
	ReleasedKeys();
}

/// <summary>
/// Updates all the information about the mouse input based on information reveived from the DirectXTK
/// </summary>
void InputManager_GL::MouseInput()
{
}

/// <summary>
/// Updates all the information about released keys this frame based on information reveived from the DirectXTK
/// </summary>
void InputManager_GL::ReleasedKeys()
{
}

/// <summary>
/// Updates all the information about pressed keys this frame based on information reveived from the DirectXTK
/// </summary>
void InputManager_GL::SinglePressKeys()
{
}

/// <summary>
/// Updates all the information about held down keys this frame based on information reveived from the DirectXTK
/// </summary>
void InputManager_GL::HeldDownKeys()
{
}