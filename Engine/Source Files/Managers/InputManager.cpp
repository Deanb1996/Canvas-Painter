#include "InputManager.h"

/// <summary>
/// Default constructor
/// </summary>
InputManager::InputManager()
{
}

/// <summary>
/// Default destructor
/// </summary>
InputManager::~InputManager()
{
}


/// <summary>
/// Checks to see if a given button was pushed this frame
/// </summary>
/// <param name="pButton">Given button to check</param>
/// <returns>True or false representing if button was pushed this frame or not</returns>
bool InputManager::KeyDown(const KEYS & pButton)
{
	auto key = std::find_if(mKeyStates.begin(), mKeyStates.end(), [&](const std::pair<KEYS, KEY_STATE>& key)
	{
		return key.first == pButton && key.second == KEY_STATE::KEY_DOWN;
	});
	return key != mKeyStates.end();
}

/// <summary>
/// Checks to see if a given button was released this frame
/// </summary>
/// <param name="pButton">Given button to check</param>
/// <returns>True or false representing if button was released this frame or not</returns>
bool InputManager::KeyUp(const KEYS & pButton)
{
	auto key = std::find_if(mKeyStates.begin(), mKeyStates.end(), [&](const std::pair<KEYS, KEY_STATE>& key)
	{
		return key.first == pButton && key.second == KEY_STATE::KEY_UP;
	});
	return key != mKeyStates.end();
}
/// <summary>
/// Checks to see if a given button was released this frame
/// </summary>
/// <param name="pButton">Given button to check</param>
/// <returns>True or false representing if button was released this frame or not</returns>
bool InputManager::KeyHeld(const KEYS & pButton)
{
	auto key = std::find_if(mKeyStates.begin(), mKeyStates.end(), [&](const std::pair<KEYS, KEY_STATE>& key)
	{
		return key.first == pButton && key.second == KEY_STATE::KEY_HELD;
	});
	return key != mKeyStates.end();
}

/// <summary>
/// Gets the value of the mouses scroll wheel this frame
/// </summary>
/// <returns>Value of the mouses scroll wheel</returns>
const int InputManager::ScrollWheel() const
{
	return mMouseWheelValue;
}

/// <summary>
/// Gets the position of the mouse this frame
/// </summary>
/// <returns>Position of the mouse</returns>
const KodeboldsMath::Vector2 & InputManager::MousePos() const
{
	return mMousePosition;
}

/// <summary>
/// Converts the mouses position from screen coords to world coords and calculates the direction from the eye position
/// </summary>
/// <param name="pViewInverse">Inverse view matrix</param>
/// <param name="pProjInverse">Inverse projection matrix</param>
/// <param name="pWidth">Width of window</param>
/// <param name="pHeight">Height of window</param>
/// <returns>Direction vector containing the direction of the world mouse pos from the eye position</returns>
const KodeboldsMath::Vector4 InputManager::RayFromMouse(const KodeboldsMath::Matrix4& pViewInverse, const KodeboldsMath::Matrix4& pProjInverse, const float& pWidth, const float& pHeight)
{
	//Normalised device coords
	const KodeboldsMath::Vector4 mousePosNormalised(
		(2.0f * mMousePosition.X) / pWidth - 1.0f,
		1.0f - (2.0f * mMousePosition.Y) / pHeight,
		1.0f,
		1.0f
	);

	//Multiply by inverse projection
	KodeboldsMath::Vector4 ray = KodeboldsMath::MultiplyVectorMatrix(mousePosNormalised, pProjInverse);
	ray.Z = 1.0f;
	ray.W = 0.0f;

	//Multiply by inverse view
	ray = KodeboldsMath::MultiplyVectorMatrix(ray, pViewInverse);
	ray.W = 0.0f;

	//Normalise to get direction of ray and return
	ray.Normalise();
	return ray;
}

/// <summary>
/// Updates the input handling every frame
/// </summary>
void InputManager::Update()
{
	mKeyStates.clear();
	KeyboardInput();
	MouseInput();
}