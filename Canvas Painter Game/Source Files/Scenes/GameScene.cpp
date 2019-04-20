#include "GameScene.h"

using namespace KodeboldsMath;

/// <summary>
/// Creates a player entity so the network system can access and update the player colour
/// </summary>
void GameScene::CreatePlayer()
{
	//Create player entity
	mPlayerEntity = mEcsManager->CreateEntity();

	//Creates players colour component
	Colour colour{ Vector4(1.0f, 1.0f, 1.0f, 1.0f) };
	mPlayerColour = colour.colour;

	//Set red if first player
	if (mNetworkManager->PeerCount() == 0)
	{
		mPlayerColour = colour.colour = Vector4(0.5f, 0.0f, 0.0f, 1.0f);
		mPlayerNumber = 1;
	}

	mEcsManager->AddColourComp(colour, mPlayerEntity);
}

/// <summary>
/// Creates the camera for the scene
/// </summary>
void GameScene::CreateCamera()
{
	//Create camera entity
	mCameraID = mEcsManager->CreateEntity();

	//Create cameras transform component
	Transform transform;
	transform.translation = Vector4(0.0f, 0.0f, -30.0f, 1.0f);
	mEcsManager->AddTransformComp(transform, mCameraID);

	//Creates cameras camera component
	Camera camera{ Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), 60, 1, 500 };
	mEcsManager->AddCameraComp(camera, mCameraID);

	//Sets camera variables for scene
	mActiveCamera = *mEcsManager->CameraComp(mCameraID);

	mLookAt = LookAt(transform.translation, mActiveCamera.lookAt, mActiveCamera.up);
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
	transform.translation = Vector4(0.0f, 10.0f, -20.0f, 1.0f);
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
		for (float j = -75; j < 75; j++)
		{
			for (float i = -135; i < 135; i++)
			{
				//Create cube entity
				cubeID = mEcsManager->CreateEntity();

				//Create cubes transform component
				Transform transform;
				transform.translation = Vector4(i / 5, j / 5, k / 5, 1.0f);
				transform.transform *= TranslationMatrix(transform.translation) * ScaleMatrix(Vector4(0.1f, 0.1f, 0.1f, 1.0f));// *RotationMatrixX(DegreesToRadians(45)) * ScaleMatrix(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
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

				//Creates cubes weight component
				Weight weight{1};
				mEcsManager->AddWeightComp(weight, cubeID);

				//Creates cubes box collider component
				//Cubes are 2 width, height and depth, scaled by 0.1 to 0.2 width, height and depth.
				//+/- half the width, height and depth to cubes translation to get max/min bounds of box collider
				BoxCollider boxCollider{
					Vector3(transform.translation.X - 0.1f, transform.translation.Y - 0.1f, transform.translation.Z - 0.1f),
					Vector3(transform.translation.X + 0.1f, transform.translation.Y + 0.1f, transform.translation.Z + 0.1f)
				};
				mEcsManager->AddBoxColliderComp(boxCollider, cubeID);

				mCubeCount++;
			}
		}
	}
}

/// <summary>
/// Manages camera movement based on user input
/// </summary>
void GameScene::CameraControls()
{
	float deltaTime = static_cast<float>(mSceneManager->DeltaTime());

	//Forwards
	if (mInputManager->KeyHeld(KEYS::KEY_W))
	{
		mEcsManager->TransformComp(mCameraID)->translation += Vector4(0, 0, mMoveSpeed, 0) * deltaTime;
		mEcsManager->CameraComp(mCameraID)->lookAt += Vector4(0, 0, mMoveSpeed, 0)  * deltaTime;
	}
	//Left
	if (mInputManager->KeyHeld(KEYS::KEY_A))
	{
		mEcsManager->TransformComp(mCameraID)->translation += Vector4(-mMoveSpeed, 0, 0, 0) * deltaTime;
		mEcsManager->CameraComp(mCameraID)->lookAt += Vector4(-mMoveSpeed, 0, 0, 0)  * deltaTime;
	}
	//Backwards
	if (mInputManager->KeyHeld(KEYS::KEY_S))
	{
		mEcsManager->TransformComp(mCameraID)->translation += Vector4(0, 0, -mMoveSpeed, 0) * deltaTime;
		mEcsManager->CameraComp(mCameraID)->lookAt += Vector4(0, 0, -mMoveSpeed, 0)  * deltaTime;
	}
	//Right
	if (mInputManager->KeyHeld(KEYS::KEY_D))
	{
		mEcsManager->TransformComp(mCameraID)->translation += Vector4(mMoveSpeed, 0, 0, 0) * deltaTime;
		mEcsManager->CameraComp(mCameraID)->lookAt += Vector4(mMoveSpeed, 0, 0, 0)  * deltaTime;
	}
	//Up
	if (mInputManager->KeyHeld(KEYS::KEY_SPACE))
	{
		mEcsManager->TransformComp(mCameraID)->translation += Vector4(0, mMoveSpeed, 0, 0) * deltaTime;
		mEcsManager->CameraComp(mCameraID)->lookAt += Vector4(0, mMoveSpeed, 0, 0)  * deltaTime;
	}
	//Down
	if (mInputManager->KeyHeld(KEYS::KEY_LEFT_CTRL))
	{
		mEcsManager->TransformComp(mCameraID)->translation += Vector4(0, -mMoveSpeed, 0, 0) * deltaTime;
		mEcsManager->CameraComp(mCameraID)->lookAt += Vector4(0, -mMoveSpeed, 0, 0)  * deltaTime;
	}
}

/// <summary>
/// Colours the canvas to match the assigned player colour
/// </summary>
void GameScene::ColourCanvas()
{
	//Loops through every voxel of the canvas and colours it
	for (int i = 1; i < mCubeCount; i++)
	{
		mEcsManager->ColourComp(i)->colour = mPlayerColour;
	}
}

/// <summary>
/// Handles the response for cube clicks
/// </summary>
void GameScene::CubeClicked()
{
	//If ray existed, retrieve intersection result
	if (mRayID != -1)
	{
		Ray rayComp = *mEcsManager->RayComp(mRayID);

		//If ray has intersected with a cube, send clicked message
		int intersectedCube = rayComp.intersectedWith;
		if (intersectedCube != -1)
		{
			//If game is multiplayer, handle multiplayer interaction
			if (mPeerCount > 0)
			{
				//Randomly choose a player to steal from
				int playerToStealFrom = 0;
				do
				{
					playerToStealFrom = rand() % mPlayerCount + 1;
				} 
				while (playerToStealFrom == mPlayerNumber);

				//Send message to the chosen player requesting to steal a voxel
				mNetworkManager->AddMessage("CLICKED:" + std::to_string(intersectedCube) + ":" + std::to_string(playerToStealFrom) + ":" + std::to_string(mPlayerNumber));
			}
			//If single player, just draw
			else
			{
				mEcsManager->ColourComp(intersectedCube)->colour = Vector4(1, 1, 1, 1);
			}
		}

		//Destroy ray once done with it
		mEcsManager->DestroyEntity(mRayID);
		mRayID = -1;
	}
}

/// <summary>
/// Updates the frequency of the rendering and networking based on the user inputs
/// </summary>
void GameScene::ControlFrequency()
{
	//Increase rendering frequency
	if (mInputManager->KeyDown(KEYS::KEY_U))
	{
		mEcsManager->TargetRenderingFrequency()++;
	}
	//Decrease rendering frequency
	if (mInputManager->KeyDown(KEYS::KEY_J))
	{
		mEcsManager->TargetRenderingFrequency()--;
	}
	//Increase networking frequency
	if (mInputManager->KeyDown(KEYS::KEY_Y))
	{
		mEcsManager->TargetNetworkingFrequency()++;
	}
	//Decrease networking frequency
	if (mInputManager->KeyDown(KEYS::KEY_H))
	{
		mEcsManager->TargetNetworkingFrequency()--;
	}
}

/// <summary>
/// 
/// </summary>
void GameScene::IntegrityCheck()
{

}

/// <summary>
/// 
/// </summary>
void GameScene::Reset()
{

}

/// <summary>
/// Default constructor
/// </summary>
GameScene::GameScene()
	:mCameraID(-1), mRayID(-1), mCubeCount(0), mPeerCount(0), mPlayerCount(1), mPlayerNumber(0), mTotalMass(0)
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
	//Keep games peer count and player count up to date
	if (mPeerCount != mNetworkManager->PeerCount())
	{
		mPeerCount = mNetworkManager->PeerCount();
		mPlayerCount = mPeerCount + 1;
	}

	//Keep games player colour and player number up to date
	if (mPlayerColour.X != mEcsManager->ColourComp(0)->colour.X)
	{
		mPlayerColour = mEcsManager->ColourComp(0)->colour;
		mPlayerNumber = static_cast<int>(mEcsManager->ColourComp(0)->colour.W);
		ColourCanvas();
	}

	CubeClicked();
	CameraControls();
	ControlFrequency();

	//Run integrity check when M is pressed
	if (mInputManager->KeyHeld(KEYS::KEY_M))
	{
		IntegrityCheck();
	}

	//Reset canvas when R is press
	if (mInputManager->KeyHeld(KEYS::KEY_R))
	{
		Reset();
	}

	//Cast ray on mouse click
	if (mInputManager->KeyHeld(KEYS::MOUSE_BUTTON_LEFT))
	{
		//Converts mouse position into world co-ords for mouse picking
		Vector4 ray = mInputManager->RayFromMouse(mInverseLookAt, mInverseProjection, mSceneManager->WindowWidth(), mSceneManager->WindowHeight());
		
		//Creates ray entity
		mRayID = mEcsManager->CreateEntity();

		//Creates rays ray component
		Ray rayComp{ mEcsManager->TransformComp(mCameraID)->translation.XYZ(), ray.XYZ(), KodeboldsMath::Vector3(), -1 };
		mEcsManager->AddRayComp(rayComp, mRayID);
	}
}

/// <summary>
/// Scene setup on load
/// </summary>
void GameScene::OnLoad()
{
	//AntTweak
	mAntTweakManager->AddBar("Game stats");
	mAntTweakManager->AddVariable("Game stats", "Player Count", TW_TYPE_INT32, &mPlayerCount, "group=PlayerStats");
	mAntTweakManager->AddVariable("Game stats", "Player Number", TW_TYPE_INT32, &mPlayerNumber, "group=PlayerStats");
	mAntTweakManager->AddVariable("Game stats", "Mass on PC", TW_TYPE_INT32, &mCubeCount, "group=CubeMass");
	mAntTweakManager->AddVariable("Game stats", "Starting Mass", TW_TYPE_INT32, &mStartingMass, "group=CubeMass");
	mAntTweakManager->AddVariable("Game stats", "Total Mass", TW_TYPE_INT32, &mTotalMass, "group=CubeMass");
	mAntTweakManager->AddVariable("Game stats", "Target Rendering Frequency", TW_TYPE_INT32, &mEcsManager->TargetRenderingFrequency(), "group=Frequencies");
	mAntTweakManager->AddVariable("Game stats", "Target Networking Frequency", TW_TYPE_INT32, &mEcsManager->TargetNetworkingFrequency(), "group=Frequencies");
	mAntTweakManager->AddVariable("Game stats", "Actual Rendering Frequency", TW_TYPE_INT32, &mEcsManager->RenderingFrequency(), "group=Frequencies");
	mAntTweakManager->AddVariable("Game stats", "Actual Networking Frequency", TW_TYPE_INT32, &mEcsManager->NetworkingFrequency(), "group=Frequencies");

	//Create game entities
	CreatePlayer();
	CreateCanvas();
	mStartingMass = mCubeCount * 4;
	CreateCamera();
	CreateLight();
}

/// <summary>
/// Clear up scene resources
/// </summary>
void GameScene::OnUnload()
{
}
