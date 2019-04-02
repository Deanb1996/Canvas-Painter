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
	int lightID = mEcsManager->CreateEntity();

	//Create lights transform component
	Transform transform;
	transform.mTranslation = Vector4(0.0f, 10.0f, -5.0f, 1.0f);
	mEcsManager->AddTransformComp(transform, lightID);

	//Create lights light component
	Light light{ Vector4(1.0f, 1.0f, 1.0f, 1.0f) };
	mEcsManager->AddLightComp(light, lightID);
}

/// <summary>
/// Creates the painting canvas made of voxel cubes
/// </summary>
void GameScene::CreateCanvas()
{
	int cubeID = 0;
	for (float k = -2; k < 2; k++)
	{
		for (float j = -100; j < 100; j++)
		{
			for (float i = -100; i < 100; i++)
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
				Colour colour{Vector4(1.0f, 1.0f, 1.0f, 1.0f)};
				mEcsManager->AddColourComp(colour, cubeID);

				//Creates cubes box collider component
				//Cubes are 2 width, height and depth, scaled by 0.1 to 0.2 width, height and depth.
				//+/- half the width, height and depth to cubes translation to get max/min bounds of box collider
				BoxCollider boxCollider{
					Vector3(transform.mTranslation.X - 0.1f, transform.mTranslation.Y - 0.1f, transform.mTranslation.Z - 0.1f),
					Vector3(transform.mTranslation.X + 0.1f, transform.mTranslation.Y + 0.1f, transform.mTranslation.Z + 0.1f)
				};
				mEcsManager->AddBoxColliderComp(boxCollider, cubeID);
			}
		}
	}
}

/// <summary>
/// Manages camera movement based on user input
/// </summary>
void GameScene::CameraControls()
{
	//Forwards
	if (mInputManager->KeyHeld(KEYS::KEY_W))
	{
		mEcsManager->TransformComp(mCameraID)->mTranslation += Vector4(0, 0, 1, 0) * mSceneManager->DeltaTime();
		mEcsManager->CameraComp(mCameraID)->mLookAt += Vector4(0, 0, 1, 0)  * mSceneManager->DeltaTime();
	}
	//Left
	if (mInputManager->KeyHeld(KEYS::KEY_A))
	{
		mEcsManager->TransformComp(mCameraID)->mTranslation += Vector4(-1, 0, 0, 0) * mSceneManager->DeltaTime();
		mEcsManager->CameraComp(mCameraID)->mLookAt += Vector4(-1, 0, 0, 0)  * mSceneManager->DeltaTime();
	}
	//Backwards
	if (mInputManager->KeyHeld(KEYS::KEY_S))
	{
		mEcsManager->TransformComp(mCameraID)->mTranslation += Vector4(0, 0, -1, 0) * mSceneManager->DeltaTime();
		mEcsManager->CameraComp(mCameraID)->mLookAt += Vector4(0, 0, -1, 0)  * mSceneManager->DeltaTime();
	}
	//Right
	if (mInputManager->KeyHeld(KEYS::KEY_D))
	{
		mEcsManager->TransformComp(mCameraID)->mTranslation += Vector4(1, 0, 0, 0) * mSceneManager->DeltaTime();
		mEcsManager->CameraComp(mCameraID)->mLookAt += Vector4(1, 0, 0, 0)  * mSceneManager->DeltaTime();
	}
	//Up
	if (mInputManager->KeyHeld(KEYS::KEY_SPACE))
	{
		mEcsManager->TransformComp(mCameraID)->mTranslation += Vector4(0, 1, 0, 0) * mSceneManager->DeltaTime();
		mEcsManager->CameraComp(mCameraID)->mLookAt += Vector4(0, 1, 0, 0)  * mSceneManager->DeltaTime();
	}
	//Down
	if (mInputManager->KeyHeld(KEYS::KEY_LEFT_CTRL))
	{
		mEcsManager->TransformComp(mCameraID)->mTranslation += Vector4(0, -1, 0, 0) * mSceneManager->DeltaTime();
		mEcsManager->CameraComp(mCameraID)->mLookAt += Vector4(0, -1, 0, 0)  * mSceneManager->DeltaTime();
	}
}

/// <summary>
/// Default constructor
/// </summary>
GameScene::GameScene()
	:mRayID(-1)
{
}

/// <summary>
/// Default destructor
/// </summary>
GameScene::~GameScene()
{
}

/// <summary>
/// Update loop for the scene
/// </summary>
void GameScene::Update()
{
	//If ray existed, retrieve intersection result
	if (mRayID != -1)
	{
		Ray rayComp = *mEcsManager->RayComp(mRayID);

		//If ray has intersected with a cube, set colour of intersected cube
		int intersectedCube = rayComp.mIntersectedWith;
		if (intersectedCube != -1)
		{
			mEcsManager->ColourComp(intersectedCube)->Colour = mPlayerColour;
		}

		//Destroy ray once done with it
		mEcsManager->DestroyEntity(mRayID);
		mRayID = -1;
	}

	CameraControls();

	//Cast ray on mouse click
	if (mInputManager->KeyHeld(KEYS::MOUSE_BUTTON_LEFT))
	{
		//Converts mouse position into world co-ords for mouse picking
		Vector4 ray = mInputManager->RayFromMouse(mInverseLookAt, mInverseProjection, mSceneManager->WindowWidth(), mSceneManager->WindowHeight());
		
		//Creates ray entity
		mRayID = mEcsManager->CreateEntity();

		//Creates rays ray component
		Ray rayComp{ mEcsManager->TransformComp(mCameraID)->mTranslation.XYZ(), ray.XYZ(), MathsHelper::Vector3(), -1 };
		mEcsManager->AddRayComp(rayComp, mRayID);
	}
}

/// <summary>
/// Scene setup on load
/// </summary>
void GameScene::OnLoad()
{
	mPlayerColour = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

	CreateCamera();
	CreateCanvas();
	CreateLight();
}

/// <summary>
/// Clear up scene resources
/// </summary>
void GameScene::OnUnload()
{
}
