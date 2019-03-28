#include "GameScene.h"

using namespace MathsHelper;

/// <summary>
/// Creates the camera for the scene
/// </summary>
void GameScene::CreateCamera()
{
	//Create camera entity
	ecsManager->CreateEntity("Camera");

	//Create cameras transform component
	Transform transform;
	transform.mTranslation = Vector4(0.0f, 0.0f, -5.0f, 1.0f);
	ecsManager->AddTransformComp(transform, "Camera");

	//Creates cameras camera component
	Camera camera{ Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), 60, 1, 500 };
	ecsManager->AddCameraComp(camera, "Camera");
}

/// <summary>
/// Creates the light for the scene
/// </summary>
void GameScene::CreateLight()
{
	//Create light entity
	ecsManager->CreateEntity("Light");

	//Create lights transform component
	Transform transform;
	transform.mTranslation = Vector4(0.0f, 15.0f, -5.0f, 1.0f);
	ecsManager->AddTransformComp(transform, "Light");

	//Create lights light component
	Light light{ Vector4(1.0f, 0.0f, 0.0f, 1.0f) };
	ecsManager->AddLightComp(light, "Light");
}

/// <summary>
/// Creates the painting canvas made of voxel cubes
/// </summary>
void GameScene::CreateCanvas()
{
	int counter = 0;
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				//Creates cube entity
				ecsManager->CreateEntity("Cube" + std::to_string(counter));

				//Create cubes transform component
				Transform transform;
				transform.mTranslation = Vector4(i, k, j, 1.0f);
				transform.mTransform *= TranslationMatrix(transform.mTranslation) * ScaleMatrix(Vector4(0.5f, 0.5f, 0.5f, 1.0f));// *RotationMatrixX(DegreesToRadians(45)) * ScaleMatrix(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
				ecsManager->AddTransformComp(transform, "Cube" + std::to_string(counter));

				//Creates cubes geometry component
				Geometry geometry{ L"cube.obj" };
				ecsManager->AddGeometryComp(geometry, "Cube" + std::to_string(counter));

				//Creates cubes shader component
				Shader shader{ L"defaultShader.fx", BlendState::NOBLEND, CullState::NONE, DepthState::NONE };
				ecsManager->AddShaderComp(shader, "Cube" + std::to_string(counter));

				counter++;
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
}

/// <summary>
/// 
/// </summary>
void GameScene::OnLoad()
{
	CreateCamera();
	CreateLight();
	CreateCanvas();
}

/// <summary>
/// 
/// </summary>
void GameScene::OnUnload()
{
}
