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
	std::shared_ptr<InputManager> mInputManager = InputManager::Instance();
	std::shared_ptr<SceneManager> mSceneManager = SceneManager::Instance();

	//Entity IDs
	int mCameraID;
	int mLightID;

	void CreateCamera();
	void CreateLight();
	void CreateCanvas();
public:
	//Structors
	GameScene();
	~GameScene();

	void Render() override;
	void Update() override;
	void OnLoad() override;
	void OnUnload() override;
};