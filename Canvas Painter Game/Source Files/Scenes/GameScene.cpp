#include "GameScene.h"

using namespace MathsHelper;

/// <summary>
/// Creates the camera for the scene
/// </summary>
void GameScene::CreateCamera()
{
	//Create camera entity
	mCameraID = mEcsManager->CreateEntity();

	//Create cameras transform component
	Transform transform;
	transform.mTranslation = Vector4(0.0f, 0.0f, -20.0f, 1.0f);
	mEcsManager->AddTransformComp(transform, mCameraID);

	//Creates cameras camera component
	Camera camera{ Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), 60, 1, 500 };
	mEcsManager->AddCameraComp(camera, mCameraID);
}

/// <summary>
/// Creates the light for the scene
/// </summary>
void GameScene::CreateLight()
{
	//Create light entity
	mLightID = mEcsManager->CreateEntity();

	//Create lights transform component
	Transform transform;
	transform.mTranslation = Vector4(15.0f, 10.0f, -15.0f, 1.0f);
	mEcsManager->AddTransformComp(transform, mLightID);

	//Create lights light component
	Light light{ Vector4(1.0f, 1.0f, 1.0f, 1.0f) };
	mEcsManager->AddLightComp(light, mLightID);
}

/// <summary>
/// Creates the painting canvas made of voxel cubes
/// </summary>
void GameScene::CreateCanvas()
{
	int cubeID = 0;
	for (float i = -100; i < 100; i++)
	{
		for (float j = -100; j < 100; j++)
		{
			for (float k = -2; k < 2; k++)
			{
				//Create cube entity
				cubeID = mEcsManager->CreateEntity();

				//Create cubes transform component
				Transform transform;
				transform.mTranslation = Vector4(i / 10, j / 10, k / 10, 1.0f);
				transform.mTransform *= TranslationMatrix(transform.mTranslation) * ScaleMatrix(Vector4(0.1f, 0.1f, 0.1f, 1.0f));// *RotationMatrixX(DegreesToRadians(45)) * ScaleMatrix(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
				mEcsManager->AddTransformComp(transform, cubeID);

				//Creates cubes geometry component
				Geometry geometry{ L"cube.obj" };
				mEcsManager->AddGeometryComp(geometry, cubeID);

				//Creates cubes shader component
				Shader shader{ L"defaultShader.fx", BlendState::NOBLEND, CullState::NONE, DepthState::NONE };
				mEcsManager->AddShaderComp(shader, cubeID);
			}
		}
	}
}

/// <summary>
/// 
/// </summary>
GameScene::GameScene()
{
}

/// <summary>
/// 
/// </summary>
GameScene::~GameScene()
{
}

/// <summary>
/// 
/// </summary>
void GameScene::Render()
{
}

/// <summary>
/// 
/// </summary>
void GameScene::Update()
{
	auto keyPresses = mInputManager->KeyPresses();
	if (keyPresses.size() > 0)
	{
		auto A_KEY = std::find_if(keyPresses.begin(), keyPresses.end(), [&](const std::pair<KEYBOARD_BUTTONS, KEYBOARD_BUTTON_STATE>& key)
		{
			return key.first == KEYBOARD_BUTTONS::KEY_A && key.second == KEYBOARD_BUTTON_STATE::KEY_DOWN;
		});
		if (A_KEY != keyPresses.end())
		{
			mEcsManager->TransformComp(mCameraID)->mTranslation += Vector4(1, 0, 0, 0) * mSceneManager->DeltaTime();
			mEcsManager->CameraComp(mCameraID)->mLookAt += Vector4(1, 0, 0, 0)  * mSceneManager->DeltaTime();
		}
	}

	auto mousePresses = mInputManager->MousePresses();
	if (mousePresses.size() > 0)
	{
		auto LEFT_BUTTON = std::find_if(mousePresses.begin(), mousePresses.end(), [&](const std::pair<MOUSE_BUTTONS, MOUSE_BUTTON_STATE>& button)
		{
			return button.first == MOUSE_BUTTONS::MOUSE_BUTTON_LEFT && button.second == MOUSE_BUTTON_STATE::MOUSE_DOWN;
		});
		if (LEFT_BUTTON != mousePresses.end())
		{
			mEcsManager->TransformComp(mCameraID)->mTranslation += Vector4(1, 0, 0, 0) * mSceneManager->DeltaTime();
			mEcsManager->CameraComp(mCameraID)->mLookAt += Vector4(1, 0, 0, 0) * mSceneManager->DeltaTime();
		}
	}
}

/// <summary>
/// 
/// </summary>
void GameScene::OnLoad()
{
	CreateCamera();
	CreateCanvas();
	CreateLight();
}

/// <summary>
/// 
/// </summary>
void GameScene::OnUnload()
{
}
