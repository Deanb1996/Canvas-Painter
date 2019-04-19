#pragma once
#include "Scene.h"
#include <memory>
#include "Managers.h"
#include "MathsHelper.h"

class GameScene : public Scene
{
private:
	//Managers
	std::shared_ptr<ECSManager> mEcsManager = ECSManager::Instance();
	std::shared_ptr<NetworkManager> mNetworkManager = NetworkManager::Instance();
#ifdef  DIRECTX
	std::shared_ptr<InputManager_DX> mInputManager = InputManager_DX::Instance();
#elif OPENGL
	std::shared_ptr<InputManager_GL> mInputManager = InputManager_GL::Instance();
#endif
	std::shared_ptr<SceneManager> mSceneManager = SceneManager::Instance();
	std::shared_ptr<AntTweakManager> mAntTweakManager = AntTweakManager::Instance();

	//Entity IDs
	int mCameraID;
	int mRayID;
	int mCubeCount;
	int mPlayerEntity;

	//Camera
	Camera mActiveCamera;
	MathsHelper::Matrix4 mLookAt;
	MathsHelper::Matrix4 mInverseLookAt;
	MathsHelper::Matrix4 mProjection;
	MathsHelper::Matrix4 mInverseProjection;

	//Game stats
	MathsHelper::Vector4 mPlayerColour;
	int mPlayerCount;
	int mPlayerNumber;

	//On load functions
	void CreatePlayer();
	void CreateCamera();
	void CreateLight();
	void CreateCanvas();

	//Game logic functions
	void CameraControls();
	void ColourCanvas();
	void CubeClicked();

public:
	//Structors
	GameScene();
	~GameScene();

	void Update() override;
	void OnLoad() override;
	void OnUnload() override;
};