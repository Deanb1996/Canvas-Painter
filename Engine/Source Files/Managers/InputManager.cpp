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
bool InputManager::KeyDown(const KEYS & pButton)
{
	auto key = std::find_if(mKeyStates.begin(), mKeyStates.end(), [&](const std::pair<KEYS, KEY_STATE>& key)
	{
		return key.first == pButton && key.second == KEY_STATE::KEY_DOWN;
	});
	return key != mKeyStates.end();
}

/// <summary>
/// 
/// </summary>
/// <param name="pButton"></param>
/// <returns></returns>
bool InputManager::KeyUp(const KEYS & pButton)
{
	auto key = std::find_if(mKeyStates.begin(), mKeyStates.end(), [&](const std::pair<KEYS, KEY_STATE>& key)
	{
		return key.first == pButton && key.second == KEY_STATE::KEY_UP;
	});
	return key != mKeyStates.end();
}

/// <summary>
/// 
/// </summary>
/// <param name="pButton"></param>
/// <returns></returns>
bool InputManager::KeyHeld(const KEYS & pButton)
{
	auto key = std::find_if(mKeyStates.begin(), mKeyStates.end(), [&](const std::pair<KEYS, KEY_STATE>& key)
	{
		return key.first == pButton && key.second == KEY_STATE::KEY_HELD;
	});
	return key != mKeyStates.end();
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
const MathsHelper::Vector4 InputManager::RayFromMouse(const MathsHelper::Matrix4& pViewInverse, const MathsHelper::Matrix4& pProjInverse, const float& pWidth, const float& pHeight)
{
	//Normalised device coords
	const MathsHelper::Vector4 mousePosNormalised(
		(2.0f * mMousePosition.X) / pWidth - 1.0f,
		1.0f - (2.0f * mMousePosition.Y) / pHeight,
		1.0f,
		1.0f
	);

	//Multiply by inverse projection
	MathsHelper::Vector4 ray = MathsHelper::MultiplyVectorMatrix(mousePosNormalised, pProjInverse);
	ray.Z = 1.0f;
	ray.W = 0.0f;

	//Multiply by inverse view
	ray = MathsHelper::MultiplyVectorMatrix(ray, pViewInverse);
	ray.W = 0.0f;

	//Normalise to get direction of ray and return
	ray.Normalise();
	return ray;
}

/// <summary>
/// 
/// </summary>
void InputManager::Update()
{
	mKeyStates.clear();
	KeyboardInput();
	MouseInput();
}