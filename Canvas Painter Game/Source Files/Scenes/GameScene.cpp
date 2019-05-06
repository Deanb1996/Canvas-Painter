#include "GameScene.h"

using namespace KodeboldsMath;

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
				Colour colour{GameStats::gPlayerColour};
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

				GameStats::gCubeCount++;
			}
		}
	}
	GameStats::gColoursBeforeIntegrity = std::vector<Vector4>(GameStats::gCubeCount);
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
	for (int i = 2; i < GameStats::gCubeCount + 2; i++)
	{
		//If colour exists, set colour to player colour
		if (mEcsManager->ColourComp(i))
		{
			mEcsManager->ColourComp(i)->colour = GameStats::gPlayerColour;
		}
		//Else create colour component with player colour
		else
		{
			Colour colour{ GameStats::gPlayerColour };
			mEcsManager->AddColourComp(colour, i);
		}
		//Set weight to 1
		mEcsManager->WeightComp(i)->weight = 1;
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
		if (intersectedCube != -1 && intersectedCube != mPreviousCube)
		{
			mPreviousCube = intersectedCube;
			//If game is multiplayer, handle multiplayer interaction
			if (mNetworkManager->PeerCount() > 0)
			{
				//Randomly choose a player to steal from
				int playerToStealFrom = 0;
				std::vector<int>::iterator it;
				do
				{
					playerToStealFrom = rand() % 4 + 1;

					it = std::find(GameStats::gTakenPlayerNumbers.begin(), GameStats::gTakenPlayerNumbers.end(), playerToStealFrom);
				} 
				while (playerToStealFrom == GameStats::gPlayerNumber || it == GameStats::gTakenPlayerNumbers.end());

				//Send message to the chosen player requesting to steal a voxel
				mNetworkManager->AddMessage("CLICKED:" + std::to_string(intersectedCube) + ":" + std::to_string(playerToStealFrom) + ":" + std::to_string(GameStats::gPlayerNumber));
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
/// Sends a message to the other peers requesting them to reset the canvas and then resets the local canvas
/// </summary>
void GameScene::Reset()
{
	mNetworkManager->AddMessage("RESET");
	ColourCanvas();
	GameStats::gCurrentMass = GameStats::gStartingMass;
}

/// <summary>
/// Default constructor
/// </summary>
GameScene::GameScene()
	:mCameraID(-1), mRayID(-1), mPreviousCube(0), mIntegrityMode(false)
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
	//Paint canvas when new colour assigned
	if (!GameStats::gCanvasColoured)
	{
		ColourCanvas();
		GameStats::gCanvasColoured = true;
	}

	CubeClicked();
	CameraControls();
	ControlFrequency();

	if (mInputManager->KeyDown(KEYS::KEY_M))
	{
		//Send integrity message check if not already in integrity mode
		if (!mIntegrityMode)
		{
			mNetworkManager->AddMessage("INTEGRITY:" + std::to_string(GameStats::gPlayerNumber));
			mIntegrityMode = true;
			GameStats::gTotalMass = 0;
		}
		//Else colour canvas to previous state
		else
		{
			//Loops through every voxel of the canvas and colours it back to its original colour before integrity
			for (int i = 2; i < GameStats::gCubeCount + 2; i++)
			{
				if (GameStats::gColoursBeforeIntegrity[i-2].W != 0)
				{
					mEcsManager->ColourComp(i)->colour = GameStats::gColoursBeforeIntegrity[i - 2];
				}
				else
				{
					mEcsManager->RemoveColourComp(i);
				}
			}
			mIntegrityMode = false;
		}
	}

	//Reset canvas when R is press
	if (mInputManager->KeyDown(KEYS::KEY_R))
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
	mAntTweakManager->AddVariable("Game stats", "Player Count", TW_TYPE_INT32, &GameStats::gPlayerCount, "group=PlayerStats");
	mAntTweakManager->AddVariable("Game stats", "Player Number", TW_TYPE_INT32, &GameStats::gPlayerNumber, "group=PlayerStats");
	mAntTweakManager->AddVariable("Game stats", "Mass on PC", TW_TYPE_INT32, &GameStats::gCurrentMass, "group=CubeMass");
	mAntTweakManager->AddVariable("Game stats", "Starting Mass", TW_TYPE_INT32, &GameStats::gStartingMass, "group=CubeMass");
	mAntTweakManager->AddVariable("Game stats", "Total Mass", TW_TYPE_INT32, &GameStats::gTotalMass, "group=CubeMass");
	mAntTweakManager->AddVariable("Game stats", "Target Rendering Frequency", TW_TYPE_INT32, &mEcsManager->TargetRenderingFrequency(), "group=Frequencies");
	mAntTweakManager->AddVariable("Game stats", "Target Networking Frequency", TW_TYPE_INT32, &mEcsManager->TargetNetworkingFrequency(), "group=Frequencies");
	mAntTweakManager->AddVariable("Game stats", "Actual Rendering Frequency", TW_TYPE_INT32, &mEcsManager->RenderingFrequency(), "group=Frequencies");
	mAntTweakManager->AddVariable("Game stats", "Actual Networking Frequency", TW_TYPE_INT32, &mEcsManager->NetworkingFrequency(), "group=Frequencies");

	//Create game entities
	CreateLight();
	CreateCamera();
	CreateCanvas();
	GameStats::gCurrentMass = GameStats::gCubeCount;
	GameStats::gStartingMass = GameStats::gCubeCount;
}

/// <summary>
/// Clear up scene resources
/// </summary>
void GameScene::OnUnload()
{
}
