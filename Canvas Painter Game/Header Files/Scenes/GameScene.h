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
	std::shared_ptr<InputManager_DX> mInputManager = InputManager_DX::Instance();
	std::shared_ptr<SceneManager> mSceneManager = SceneManager::Instance();

	//Entity IDs
	int mCameraID;
	int mRayID;

	//Camera
	Camera mActiveCamera;
	MathsHelper::Matrix4 mLookAt;
	MathsHelper::Matrix4 mInverseLookAt;
	MathsHelper::Matrix4 mProjection;
	MathsHelper::Matrix4 mInverseProjection;

	MathsHelper::Vector4 mPlayerColour;

	//On load functions
	void CreateCamera();
	void CreateLight();
	void CreateCanvas();

	void CameraControls();
public:
	//Structors
	GameScene();
	~GameScene();

	void Render() override;
	void Update() override;
	void OnLoad() override;
	void OnUnload() override;
};