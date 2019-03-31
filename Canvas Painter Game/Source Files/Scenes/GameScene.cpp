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

	//Sets camera variables for scene
	mActiveCamera = *mEcsManager->CameraComp(mCameraID);

	mLookAt = LookAt(transform.mTranslation, mActiveCamera.mLookAt, mActiveCamera.mUp);
	mInverseLookAt = Inverse(mLookAt);

	mProjection = Projection(DegreesToRadians(60), mSceneManager->WindowWidth() / mSceneManager->WindowHeight(), 1, 500);
	mInverseProjection = Inverse(mProjection);
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
	transform.mTranslation = Vector4(0.0f, 10.0f, -5.0f, 1.0f);
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

				//Creates cubes colour component
				Colour colour{mPlayerColour};
				mEcsManager->AddColourComp(colour, cubeID);

				//Creates cubes box collider component
				BoxCollider boxCollider{ std::vector<int>(), 0.2, 0.2, 0.2 };
				mEcsManager->AddBoxColliderComp(boxCollider, cubeID);
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
	if (mInputManager->KeyHeld(KEYS::KEY_D))
	{
		mEcsManager->TransformComp(mCameraID)->mTranslation += Vector4(1, 0, 0, 0) * mSceneManager->DeltaTime();
		mEcsManager->CameraComp(mCameraID)->mLookAt += Vector4(1, 0, 0, 0)  * mSceneManager->DeltaTime();
	}

	if (mInputManager->KeyHeld(KEYS::MOUSE_BUTTON_LEFT))
	{
		Vector4 ray = mInputManager->RayFromMouse(mInverseLookAt, mInverseProjection, mSceneManager->WindowWidth(), mSceneManager->WindowHeight());
		
		Ray rayComp{ mEcsManager->TransformComp(mCameraID)->mTranslation, ray };
		int rayID = mEcsManager->CreateEntity();
		mEcsManager->AddRayComp(rayComp, rayID);


		//Create cube entity
		int cubeID = mEcsManager->CreateEntity();

		//Create cubes transform component
		Transform transform;
		transform.mTranslation = mEcsManager->TransformComp(mCameraID)->mTranslation + (ray * 10);
		transform.mTransform *= TranslationMatrix(transform.mTranslation) * ScaleMatrix(Vector4(0.5f, 0.5f, 0.5f, 1.0f));// *RotationMatrixX(DegreesToRadians(45)) * ScaleMatrix(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
		mEcsManager->AddTransformComp(transform, cubeID);

		//Creates cubes geometry component
		Geometry geometry{ L"cube.obj" };
		mEcsManager->AddGeometryComp(geometry, cubeID);

		//Creates cubes shader component
		Shader shader{ L"defaultShader.fx", BlendState::NOBLEND, CullState::NONE, DepthState::NONE };
		mEcsManager->AddShaderComp(shader, cubeID);
	}
}

/// <summary>
/// 
/// </summary>
void GameScene::OnLoad()
{
	mPlayerColour = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

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
