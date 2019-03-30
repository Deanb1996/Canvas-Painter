#include "InputManager.h"

/// <summary>
/// 
/// </summary>
InputManager::InputManager()
{
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
const MathsHelper::Vector2 & InputManager::MousePos() const
{
	return mMousePosition;
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
	float x = tanf(pFOV * 0.5f) * (mMousePosition.X / widthDiv2 - 1.0f) / aspectRatio;
	float y = tanf(pFOV * 0.5f) * (1.0f - mMousePosition.Y / heightDiv2);

	//Calculate ray
	MathsHelper::Vector4 origin = MathsHelper::Vector4(x * pNear, y * pNear, pNear, 1);
	MathsHelper::Vector4 destination = MathsHelper::Vector4(x * pFar, y * pFar, pFar, 1);

	//Multiply by inverse view
	origin = MathsHelper::MultiplyVectorMatrix(origin, pViewInverse);
	destination = MathsHelper::MultiplyVectorMatrix(destination, pViewInverse);

	//Return origin and direction of ray
	pOrigin = origin;
	pDirection = MathsHelper::Vector4(destination - origin).Normalise();
}

/// <summary>
/// 
/// </summary>
void InputManager::Update()
{
	KeyboardInput();
	MouseInput();
}