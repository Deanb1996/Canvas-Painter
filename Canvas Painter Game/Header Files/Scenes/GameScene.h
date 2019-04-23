#pragma once
#include "Scene.h"
#include <memory>
#include "Managers.h"
#include "KodeboldsMath.h"
#include "GameStats.h"

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

	//Camera
	Camera mActiveCamera;
	KodeboldsMath::Matrix4 mLookAt;
	KodeboldsMath::Matrix4 mInverseLookAt;
	KodeboldsMath::Matrix4 mProjection;
	KodeboldsMath::Matrix4 mInverseProjection;

	//Game stats
	const float mMoveSpeed = 5;
	int mPreviousCube;
	bool mIntegrityMode;

	//On load functions
	void CreateCamera();
	void CreateLight();
	void CreateCanvas();

	//Game logic functions
	void CameraControls();
	void ColourCanvas();
	void CubeClicked();
	void ControlFrequency();
	void Reset();

public:
	//Structors
	GameScene();
	~GameScene();

	void Update() override;
	void OnLoad() override;
	void OnUnload() override;
};