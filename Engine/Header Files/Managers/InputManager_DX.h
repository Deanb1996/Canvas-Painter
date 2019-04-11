#pragma once
#include <windows.h>
#include <Keyboard.h>
#include <DirectXMath.h>
#include <Mouse.h>
#include <memory>
#include "InputManager.h"
#include "AntTweakBar.h"

class InputManager_DX : public InputManager
{
private:
	//Keyboard
	std::shared_ptr<DirectX::Keyboard> mKeyboard;
	DirectX::Keyboard::KeyboardStateTracker mKeyboardTracker;
	DirectX::Keyboard::State mKeyboardState;

	//Mouse
	std::shared_ptr<DirectX::Mouse> mMouse;
	DirectX::Mouse::ButtonStateTracker mMouseTracker;
	DirectX::Mouse::State mMouseState;

	void KeyboardInput() override;
	void SinglePressKeys() override;
	void ReleasedKeys() override;
	void HeldDownKeys() override;
	void MouseInput() override;

	//Private constructor for singleton pattern
	InputManager_DX();

public:
	static LRESULT CALLBACK WndProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam);
	~InputManager_DX();

	//Singleton pattern
	//Deleted copy constructor and assignment operator so no copies of the singleton instance can be made
	InputManager_DX(InputManager_DX const&) = delete;
	InputManager_DX& operator=(InputManager_DX const&) = delete;
	static std::shared_ptr<InputManager_DX> Instance();

	void CenterCursor() override;
	void CursorVisible(const bool pVisible) override;
};