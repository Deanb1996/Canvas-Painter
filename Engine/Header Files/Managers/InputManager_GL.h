#pragma once
#include <windows.h>
#include <memory>
#include "InputManager.h"
#include "AntTweakBar.h"

class InputManager_GL : public InputManager
{
private:

	void KeyboardInput() override;
	void SinglePressKeys() override;
	void ReleasedKeys() override;
	void HeldDownKeys() override;
	void MouseInput() override;

	//Private constructor for singleton pattern
	InputManager_GL();

public:
	static LRESULT CALLBACK WndProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam);
	~InputManager_GL();

	//Singleton pattern
	//Deleted copy constructor and assignment operator so no copies of the singleton instance can be made
	InputManager_GL(InputManager_GL const&) = delete;
	InputManager_GL& operator=(InputManager_GL const&) = delete;
	static std::shared_ptr<InputManager_GL> Instance();

	void CenterCursor() override;
	void CursorVisible(const bool pVisible) override;
};