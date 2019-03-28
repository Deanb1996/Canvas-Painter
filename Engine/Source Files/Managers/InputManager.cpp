#include "InputManager.h"

InputManager::InputManager()
{
	mKeyboard = std::make_shared<DirectX::Keyboard>();
	mMouse = std::make_shared<DirectX::Mouse>();
}

InputManager::~InputManager()
{
}

std::shared_ptr<InputManager> InputManager::Instance()
{
	static std::shared_ptr<InputManager> instance{ new InputManager };
	return instance;
}

const std::vector<std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>> InputManager::KeyPresses() const
{
	/*return std::vector<std::pair<KEYBOARD_BUTTONS, bool>>();*/
	return keyboardButtonPresses;
}

const std::vector<std::pair<MOUSE_BUTTONS, MOUSE_BUTTON_STATE>> InputManager::MousePresses() const
{
	//return std::vector<std::pair<MOUSE_BUTTONS, bool>>();
	return mouseButtonPresses;
}

void InputManager::CenterCursor()
{
	//SetCursorPos(CLIENT_WIDTH / 2, CLIENT_HEIGHT / 2);
}

void InputManager::CursorVisible(bool pVisible)
{
	if (pVisible) { mMouse->SetVisible(true); }
	else { mMouse->SetVisible(true); }
}

////--------------------------------------------------------------------------------------
//// Called every time the application receives a message
////--------------------------------------------------------------------------------------
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

void InputManager::Update()
{
	KeyboardInput();
	MouseInput();
}

void InputManager::KeyboardInput()
{
	keyboardButtonPresses.clear();

	auto state = mKeyboard->GetState();
	mKeyboardTracker.Update(state);


	//HeldDownKeys(state); // Registers every frame that a key is pressed
	SinglePressKeys(); // Registers once
	ReleasedKeys();
}

const void InputManager::MouseInput() {

	mouseButtonPresses.clear();

	auto state = mMouse->GetState();
	mMouseTracker.Update(state);

	mMousePosition = DirectX::XMFLOAT2(mMouse->GetState().x, mMouse->GetState().y);

	// Scroll wheel
	mMouseWheelValue = mMouse->GetState().scrollWheelValue;

	// Buttons
	using ButtonState = DirectX::Mouse::ButtonStateTracker::ButtonState;

	// Allows mouse buttons to be held down
	if (mMouseTracker.leftButton == ButtonState::HELD) { pressedMouseButton = MOUSE_BUTTONS::MOUSE_BUTTON_LEFT; mouseButtonState = MOUSE_BUTTON_STATE::MOUSE_HELD; }
	if (mMouseTracker.rightButton == ButtonState::HELD) { pressedMouseButton = MOUSE_BUTTONS::MOUSE_BUTTON_RIGHT; mouseButtonState = MOUSE_BUTTON_STATE::MOUSE_HELD; }
	if (mMouseTracker.middleButton == ButtonState::HELD) { pressedMouseButton = MOUSE_BUTTONS::MOUSE_BUTTON_MIDDLE; mouseButtonState = MOUSE_BUTTON_STATE::MOUSE_HELD; }

	// single mouse button presses
	if (mMouseTracker.leftButton == ButtonState::PRESSED) {
		pressedMouseButton = MOUSE_BUTTONS::MOUSE_BUTTON_LEFT;
		mouseButtonState = MOUSE_BUTTON_STATE::MOUSE_DOWN;
		mouseButtonPresses.push_back(std::pair<MOUSE_BUTTONS, MOUSE_BUTTON_STATE>(pressedMouseButton, mouseButtonState));
	}

	if (mMouseTracker.rightButton == ButtonState::PRESSED) { pressedMouseButton = MOUSE_BUTTONS::MOUSE_BUTTON_RIGHT; mouseButtonState = MOUSE_BUTTON_STATE::MOUSE_DOWN; }
	if (mMouseTracker.middleButton == ButtonState::PRESSED) { pressedMouseButton = MOUSE_BUTTONS::MOUSE_BUTTON_MIDDLE; mouseButtonState = MOUSE_BUTTON_STATE::MOUSE_DOWN; }

	// single mouse button releases
	if (mMouseTracker.leftButton == ButtonState::RELEASED) { pressedMouseButton = MOUSE_BUTTONS::MOUSE_BUTTON_LEFT; mouseButtonState = MOUSE_BUTTON_STATE::MOUSE_UP; }
	if (mMouseTracker.rightButton == ButtonState::RELEASED) { pressedMouseButton = MOUSE_BUTTONS::MOUSE_BUTTON_RIGHT; mouseButtonState = MOUSE_BUTTON_STATE::MOUSE_UP; }
	if (mMouseTracker.middleButton == ButtonState::RELEASED) { pressedMouseButton = MOUSE_BUTTONS::MOUSE_BUTTON_MIDDLE; mouseButtonState = MOUSE_BUTTON_STATE::MOUSE_UP; }
}

const void InputManager::ReleasedKeys()
{
	if (mKeyboardTracker.released.A) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_A; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.B) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_B; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.C) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_C; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.D) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_D; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.E) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_E; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.F) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.G) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_G; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.H) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_H; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.I) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_I; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.J) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_J; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.K) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_K; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.L) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_L; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.M) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_M; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.N) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_N; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.O) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_O; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.P) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_P; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.Q) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_Q; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.R) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_R; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.S) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_S; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.T) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_T; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.U) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_U; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.V) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_V; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.W) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_W; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.X) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_X; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.Y) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_Y; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.Z) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_Z; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }

	if (mKeyboardTracker.released.NumPad0) { pressedKeyboardButton = KEYBOARD_BUTTONS::NUM_PAD_KEY_0; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.NumPad1) { pressedKeyboardButton = KEYBOARD_BUTTONS::NUM_PAD_KEY_1; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.NumPad2) { pressedKeyboardButton = KEYBOARD_BUTTONS::NUM_PAD_KEY_2; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.NumPad3) { pressedKeyboardButton = KEYBOARD_BUTTONS::NUM_PAD_KEY_3; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.NumPad4) { pressedKeyboardButton = KEYBOARD_BUTTONS::NUM_PAD_KEY_4; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.NumPad5) { pressedKeyboardButton = KEYBOARD_BUTTONS::NUM_PAD_KEY_5; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.NumPad6) { pressedKeyboardButton = KEYBOARD_BUTTONS::NUM_PAD_KEY_6; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.NumPad7) { pressedKeyboardButton = KEYBOARD_BUTTONS::NUM_PAD_KEY_7; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.NumPad8) { pressedKeyboardButton = KEYBOARD_BUTTONS::NUM_PAD_KEY_8; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.NumPad9) { pressedKeyboardButton = KEYBOARD_BUTTONS::NUM_PAD_KEY_9; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }

	if (mKeyboardTracker.released.D0) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_0; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.D1) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_1; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.D2) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_2; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.D3) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_3; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.D4) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_4; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.D5) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_5; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.D6) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_6; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.D7) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_7; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.D8) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_8; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.D9) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_9; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }

	if (mKeyboardTracker.released.Add) {}
	if (mKeyboardTracker.released.Apps) {}
	if (mKeyboardTracker.released.Attn) {}
	if (mKeyboardTracker.released.Back) {}
	if (mKeyboardTracker.released.BrowserBack) {}
	if (mKeyboardTracker.released.BrowserFavorites) {}
	if (mKeyboardTracker.released.BrowserForward) {}
	if (mKeyboardTracker.released.BrowserHome) {}
	if (mKeyboardTracker.released.BrowserRefresh) {}
	if (mKeyboardTracker.released.BrowserSearch) {}
	if (mKeyboardTracker.released.BrowserStop) {}
	if (mKeyboardTracker.released.CapsLock) {}
	if (mKeyboardTracker.released.Crsel) {}

	if (mKeyboardTracker.released.Decimal) {}
	if (mKeyboardTracker.released.Delete) {}
	if (mKeyboardTracker.released.Divide) {}
	if (mKeyboardTracker.released.Down) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_DOWN_ARROW; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.End) {}
	if (mKeyboardTracker.released.Enter) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_ENTER; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.EraseEof) {}
	if (mKeyboardTracker.released.Escape) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_ESC; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.Execute) {}
	if (mKeyboardTracker.released.Exsel) {}

	if (mKeyboardTracker.released.F1) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F1; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.F2) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F2; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.F3) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F3; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.F4) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F4; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.F5) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F5; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.F6) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F6; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.F7) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F7; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.F8) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F8; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.F9) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F9; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.F10) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F10; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.F11) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F11; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.F12) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F12; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.F13) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F13; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.F14) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F14; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.F15) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F15; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.F16) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F16; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.F17) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F17; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.F18) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F18; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.F19) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F19; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.F20) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F20; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }

	if (mKeyboardTracker.released.Help) {}
	if (mKeyboardTracker.released.Home) {}
	if (mKeyboardTracker.released.ImeConvert) {}
	if (mKeyboardTracker.released.ImeNoConvert) {}
	if (mKeyboardTracker.released.Insert) {}
	if (mKeyboardTracker.released.Kana) {}
	if (mKeyboardTracker.released.Kanji) {}
	if (mKeyboardTracker.released.LaunchApplication1) {}
	if (mKeyboardTracker.released.LaunchApplication2) {}
	if (mKeyboardTracker.released.LaunchMail) {}
	if (mKeyboardTracker.released.Left) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_LEFT_ARROW; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.LeftAlt) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_LEFT_ALT; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.LeftControl) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_LEFT_CTRL; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.LeftShift) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_LEFT_SHIFT; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.LeftWindows) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_LEFT_WINDOWS; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.MediaNextTrack) {}
	if (mKeyboardTracker.released.MediaPlayPause) {}
	if (mKeyboardTracker.released.MediaPreviousTrack) {}
	if (mKeyboardTracker.released.MediaStop) {}
	if (mKeyboardTracker.released.Multiply) {}
	if (mKeyboardTracker.released.Oem8) {}
	if (mKeyboardTracker.released.OemAuto) {}
	if (mKeyboardTracker.released.OemBackslash) {}
	if (mKeyboardTracker.released.OemClear) {}
	if (mKeyboardTracker.released.OemCloseBrackets) {}
	if (mKeyboardTracker.released.OemComma) {}
	if (mKeyboardTracker.released.OemCopy) {}
	if (mKeyboardTracker.released.OemEnlW) {}
	if (mKeyboardTracker.released.OemMinus) {}
	if (mKeyboardTracker.released.OemOpenBrackets) {}
	if (mKeyboardTracker.released.OemPeriod) {}
	if (mKeyboardTracker.released.OemPipe) {}
	if (mKeyboardTracker.released.OemPlus) {}
	if (mKeyboardTracker.released.OemQuestion) {}
	if (mKeyboardTracker.released.OemQuotes) {}
	if (mKeyboardTracker.released.OemSemicolon) {}
	if (mKeyboardTracker.released.OemTilde) {}
	if (mKeyboardTracker.released.PageDown) {}
	if (mKeyboardTracker.released.Pa1) {}
	if (mKeyboardTracker.released.PageUp) {}
	if (mKeyboardTracker.released.Pause) {}
	if (mKeyboardTracker.released.Play) {}
	if (mKeyboardTracker.released.Print) {}
	if (mKeyboardTracker.released.PrintScreen) {}
	if (mKeyboardTracker.released.ProcessKey) {}
	if (mKeyboardTracker.released.Right) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_RIGHT_ARROW; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.RightAlt) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_RIGHT_ALT; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.RightControl) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_RIGHT_CTRL; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.RightShift) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_RIGHT_SHIFT; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.RightWindows) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_RIGHT_WINDOWS; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.Scroll) {}
	if (mKeyboardTracker.released.Select) {}
	if (mKeyboardTracker.released.SelectMedia) {}
	if (mKeyboardTracker.released.Separator) {}
	if (mKeyboardTracker.released.Sleep) {}
	if (mKeyboardTracker.released.Space) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_SPACE; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.Subtract) {}
	if (mKeyboardTracker.released.Tab) {}
	if (mKeyboardTracker.released.Up) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_UP_ARROW; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_UP; }
	if (mKeyboardTracker.released.VolumeDown) {}
	if (mKeyboardTracker.released.VolumeUp) {}
	if (mKeyboardTracker.released.VolumeMute) {}
	if (mKeyboardTracker.released.Zoom) {}
}

const void InputManager::SinglePressKeys()
{
	if (mKeyboardTracker.pressed.A) {
		pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_A;
		keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN;
		keyboardButtonPresses.push_back(std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>(pressedKeyboardButton, keyboardButtonState));
	}

	if (mKeyboardTracker.pressed.B) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_B; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.C) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_C; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.D) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_D; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.E) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_E; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.F) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.G) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_G; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.H) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_H; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.I) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_I; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.J) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_J; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.K) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_K; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.L) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_L; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.M) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_M; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.N) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_N; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.O) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_O; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.P) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_P; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.Q) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_Q; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.R) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_R; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.S) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_S; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.T) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_T; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.U) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_U; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.V) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_V; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.W) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_W; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.X) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_X; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.Y) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_Y; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.Z) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_Z; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }

	if (mKeyboardTracker.pressed.NumPad0) { pressedKeyboardButton = KEYBOARD_BUTTONS::NUM_PAD_KEY_0; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.NumPad1) { pressedKeyboardButton = KEYBOARD_BUTTONS::NUM_PAD_KEY_1; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.NumPad2) { pressedKeyboardButton = KEYBOARD_BUTTONS::NUM_PAD_KEY_2; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.NumPad3) { pressedKeyboardButton = KEYBOARD_BUTTONS::NUM_PAD_KEY_3; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.NumPad4) { pressedKeyboardButton = KEYBOARD_BUTTONS::NUM_PAD_KEY_4; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.NumPad5) { pressedKeyboardButton = KEYBOARD_BUTTONS::NUM_PAD_KEY_5; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.NumPad6) { pressedKeyboardButton = KEYBOARD_BUTTONS::NUM_PAD_KEY_6; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.NumPad7) { pressedKeyboardButton = KEYBOARD_BUTTONS::NUM_PAD_KEY_7; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.NumPad8) { pressedKeyboardButton = KEYBOARD_BUTTONS::NUM_PAD_KEY_8; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.NumPad9) { pressedKeyboardButton = KEYBOARD_BUTTONS::NUM_PAD_KEY_9; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }

	if (mKeyboardTracker.pressed.D0) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_0; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.D1) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_1; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.D2) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_2; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.D3) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_3; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.D4) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_4; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.D5) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_5; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.D6) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_6; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.D7) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_7; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.D8) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_8; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.D9) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_9; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }

	if (mKeyboardTracker.pressed.Add) {}
	if (mKeyboardTracker.pressed.Apps) {}
	if (mKeyboardTracker.pressed.Attn) {}
	if (mKeyboardTracker.pressed.Back) {}
	if (mKeyboardTracker.pressed.BrowserBack) {}
	if (mKeyboardTracker.pressed.BrowserFavorites) {}
	if (mKeyboardTracker.pressed.BrowserForward) {}
	if (mKeyboardTracker.pressed.BrowserHome) {}
	if (mKeyboardTracker.pressed.BrowserRefresh) {}
	if (mKeyboardTracker.pressed.BrowserSearch) {}
	if (mKeyboardTracker.pressed.BrowserStop) {}
	if (mKeyboardTracker.pressed.CapsLock) {}
	if (mKeyboardTracker.pressed.Crsel) {}

	if (mKeyboardTracker.pressed.Decimal) {}
	if (mKeyboardTracker.pressed.Delete) {}
	if (mKeyboardTracker.pressed.Divide) {}
	if (mKeyboardTracker.pressed.Down) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_DOWN_ARROW; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.End) {}
	if (mKeyboardTracker.pressed.Enter) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_ENTER; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.EraseEof) {}
	if (mKeyboardTracker.pressed.Escape) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_ESC; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.Execute) {}
	if (mKeyboardTracker.pressed.Exsel) {}

	if (mKeyboardTracker.pressed.F1) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F1; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.F2) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F2; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.F3) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F3; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.F4) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F4; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.F5) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F5; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.F6) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F6; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.F7) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F7; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.F8) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F8; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.F9) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F9; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.F10) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F10; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.F11) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F11; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.F12) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F12; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.F13) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F13; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.F14) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F14; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.F15) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F15; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.F16) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F16; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.F17) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F17; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.F18) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F18; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.F19) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F19; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.F20) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_F20; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }

	if (mKeyboardTracker.pressed.Help) {}
	if (mKeyboardTracker.pressed.Home) {}
	if (mKeyboardTracker.pressed.ImeConvert) {}
	if (mKeyboardTracker.pressed.ImeNoConvert) {}
	if (mKeyboardTracker.pressed.Insert) {}
	if (mKeyboardTracker.pressed.Kana) {}
	if (mKeyboardTracker.pressed.Kanji) {}
	if (mKeyboardTracker.pressed.LaunchApplication1) {}
	if (mKeyboardTracker.pressed.LaunchApplication2) {}
	if (mKeyboardTracker.pressed.LaunchMail) {}
	if (mKeyboardTracker.pressed.Left) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_LEFT_ARROW; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.LeftAlt) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_LEFT_ALT; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.LeftControl) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_LEFT_CTRL; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.LeftShift) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_LEFT_SHIFT; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.LeftWindows) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_LEFT_WINDOWS; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.MediaNextTrack) {}
	if (mKeyboardTracker.pressed.MediaPlayPause) {}
	if (mKeyboardTracker.pressed.MediaPreviousTrack) {}
	if (mKeyboardTracker.pressed.MediaStop) {}
	if (mKeyboardTracker.pressed.Multiply) {}
	if (mKeyboardTracker.pressed.Oem8) {}
	if (mKeyboardTracker.pressed.OemAuto) {}
	if (mKeyboardTracker.pressed.OemBackslash) {}
	if (mKeyboardTracker.pressed.OemClear) {}
	if (mKeyboardTracker.pressed.OemCloseBrackets) {}
	if (mKeyboardTracker.pressed.OemComma) {}
	if (mKeyboardTracker.pressed.OemCopy) {}
	if (mKeyboardTracker.pressed.OemEnlW) {}
	if (mKeyboardTracker.pressed.OemMinus) {}
	if (mKeyboardTracker.pressed.OemOpenBrackets) {}
	if (mKeyboardTracker.pressed.OemPeriod) {}
	if (mKeyboardTracker.pressed.OemPipe) {}
	if (mKeyboardTracker.pressed.OemPlus) {}
	if (mKeyboardTracker.pressed.OemQuestion) {}
	if (mKeyboardTracker.pressed.OemQuotes) {}
	if (mKeyboardTracker.pressed.OemSemicolon) {}
	if (mKeyboardTracker.pressed.OemTilde) {}
	if (mKeyboardTracker.pressed.PageDown) {}
	if (mKeyboardTracker.pressed.Pa1) {}
	if (mKeyboardTracker.pressed.PageUp) {}
	if (mKeyboardTracker.pressed.Pause) {}
	if (mKeyboardTracker.pressed.Play) {}
	if (mKeyboardTracker.pressed.Print) {}
	if (mKeyboardTracker.pressed.PrintScreen) {}
	if (mKeyboardTracker.pressed.ProcessKey) {}
	if (mKeyboardTracker.pressed.Right) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_RIGHT_ARROW; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.RightAlt) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_RIGHT_ALT; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.RightControl) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_RIGHT_CTRL; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.RightShift) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_RIGHT_SHIFT; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.RightWindows) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_RIGHT_WINDOWS; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.Scroll) {}
	if (mKeyboardTracker.pressed.Select) {}
	if (mKeyboardTracker.pressed.SelectMedia) {}
	if (mKeyboardTracker.pressed.Separator) {}
	if (mKeyboardTracker.pressed.Sleep) {}
	if (mKeyboardTracker.pressed.Space) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_SPACE; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.Subtract) {}
	if (mKeyboardTracker.pressed.Tab) {}
	if (mKeyboardTracker.pressed.Up) { pressedKeyboardButton = KEYBOARD_BUTTONS::KEY_UP_ARROW; keyboardButtonState = KEYBOARD_BUTTON_STATE::KEY_DOWN; }
	if (mKeyboardTracker.pressed.VolumeDown) {}
	if (mKeyboardTracker.pressed.VolumeUp) {}
	if (mKeyboardTracker.pressed.VolumeMute) {}
	if (mKeyboardTracker.pressed.Zoom) {}
}

const void InputManager::HeldDownKeys(DirectX::Keyboard::State &state)
{
	if (state.A) {}
	if (state.B) {}
	if (state.C) {}
	if (state.D) {}
	if (state.E) {}
	if (state.F) {}
	if (state.G) {}
	if (state.H) {}
	if (state.I) {}
	if (state.J) {}
	if (state.K) {}
	if (state.L) {}
	if (state.M) {}
	if (state.N) {}
	if (state.O) {}
	if (state.P) {}
	if (state.Q) {}
	if (state.R) {}
	if (state.S) {}
	if (state.T) {}
	if (state.U) {}
	if (state.V) {}
	if (state.W) {}
	if (state.X) {}
	if (state.Y) {}
	if (state.Z) {}

	if (state.NumPad0) {}
	if (state.NumPad1) {}
	if (state.NumPad2) {}
	if (state.NumPad3) {}
	if (state.NumPad4) {}
	if (state.NumPad5) {}
	if (state.NumPad6) {}
	if (state.NumPad7) {}
	if (state.NumPad8) {}
	if (state.NumPad9) {}

	if (state.Add) {}
	if (state.Apps) {}
	if (state.Attn) {}
	if (state.Back) {}
	if (state.BrowserBack) {}
	if (state.BrowserFavorites) {}
	if (state.BrowserForward) {}
	if (state.BrowserHome) {}
	if (state.BrowserRefresh) {}
	if (state.BrowserSearch) {}
	if (state.BrowserStop) {}
	if (state.CapsLock) {}
	if (state.Crsel) {}
	if (state.D0) {}
	if (state.D1) {}
	if (state.D2) {}
	if (state.D3) {}
	if (state.D4) {}
	if (state.D5) {}
	if (state.D6) {}
	if (state.D7) {}
	if (state.D8) {}
	if (state.D9) {}
	if (state.Decimal) {}
	if (state.Delete) {}
	if (state.Divide) {}
	if (state.Down) {}
	if (state.End) {}
	if (state.Enter) {}
	if (state.EraseEof) {}
	if (state.Escape) {}
	if (state.Execute) {}
	if (state.Exsel) {}

	if (state.F1) {}
	if (state.F2) {}
	if (state.F3) {}
	if (state.F4) {}
	if (state.F5) {}
	if (state.F6) {}
	if (state.F7) {}
	if (state.F8) {}
	if (state.F9) {}
	if (state.F10) {}
	if (state.F11) {}
	if (state.F12) {}
	if (state.F13) {}
	if (state.F14) {}
	if (state.F15) {}
	if (state.F16) {}
	if (state.F17) {}
	if (state.F18) {}
	if (state.F19) {}
	if (state.F20) {}

	if (state.Help) {}
	if (state.Home) {}
	if (state.ImeConvert) {}
	if (state.ImeNoConvert) {}
	if (state.Insert) {}
	if (state.Kana) {}
	if (state.Kanji) {}
	if (state.LaunchApplication1) {}
	if (state.LaunchApplication2) {}
	if (state.LaunchMail) {}
	if (state.Left) {}
	if (state.LeftAlt) {}
	if (state.LeftControl) {}
	if (state.LeftShift) {}
	if (state.LeftWindows) {}
	if (state.MediaNextTrack) {}
	if (state.MediaPlayPause) {}
	if (state.MediaPreviousTrack) {}
	if (state.MediaStop) {}
	if (state.Multiply) {}
	if (state.Oem8) {}
	if (state.OemAuto) {}
	if (state.OemBackslash) {}
	if (state.OemClear) {}
	if (state.OemCloseBrackets) {}
	if (state.OemComma) {}
	if (state.OemCopy) {}
	if (state.OemEnlW) {}
	if (state.OemMinus) {}
	if (state.OemOpenBrackets) {}
	if (state.OemPeriod) {}
	if (state.OemPipe) {}
	if (state.OemPlus) {}
	if (state.OemQuestion) {}
	if (state.OemQuotes) {}
	if (state.OemSemicolon) {}
	if (state.OemTilde) {}
	if (state.PageDown) {}
	if (state.Pa1) {}
	if (state.PageUp) {}
	if (state.Pause) {}
	if (state.Play) {}
	if (state.Print) {}
	if (state.PrintScreen) {}
	if (state.ProcessKey) {}
	if (state.Right) {}
	if (state.RightAlt) {}
	if (state.RightControl) {}
	if (state.RightShift) {}
	if (state.RightWindows) {}
	if (state.Scroll) {}
	if (state.Select) {}
	if (state.SelectMedia) {}
	if (state.Separator) {}
	if (state.Sleep) {}
	if (state.Space) {}
	if (state.Subtract) {}
	if (state.Tab) {}
	if (state.Up) {}
	if (state.VolumeDown) {}
	if (state.VolumeUp) {}
	if (state.VolumeMute) {}
	if (state.Zoom) {}
}