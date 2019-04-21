#pragma once
#include <memory>
#include <vector>
#include "Components.h"
#include "ISystem.h"
#include "Entity.h"
#include <algorithm>
#include <string>
#include "ThreadManager.h"
#include <chrono>

class RenderSystem_DX;

class ECSManager
{
private:
	std::shared_ptr<ThreadManager> mThreadManager = ThreadManager::Instance();

	//Entities and free ID list
	std::vector<Entity> mEntities;
	std::vector<int> mFreeEntityIDs;
	int mEntityID;

	//Components
	std::vector<AI> mAIs;
	std::vector<Audio> mAudios;
	std::vector<BoxCollider> mBoxColliders;
	std::vector<Camera> mCameras;
	std::vector<Colour> mColours;
	std::vector<Geometry> mGeometries;
	std::vector<Gravity> mGravities;
	std::vector<Light> mLights;
	std::vector<Ray> mRays;
	std::vector<Shader> mShaders;
	std::vector<SphereCollider> mSphereColliders;
	std::vector<Texture> mTextures;
	std::vector<Transform> mTransforms;
	std::vector<Velocity> mVelocities;
	std::vector<Weight> mWeights;

	//Systems
	std::vector<std::shared_ptr<ISystem>> mRenderSystems;
	std::vector<std::shared_ptr<ISystem>> mUpdateSystems;
	std::vector<std::shared_ptr<ISystem>> mNetworkSystems;

	//Render and network threads
	Task* mRenderTask = nullptr;
	Task* mNetworkingTask = nullptr;
	int mTargetRenderingFrequency;
	int mTargetNetworkingFrequency;
	int mRenderingFrequency;
	int mNetworkingFrequency;
	std::chrono::high_resolution_clock::time_point mRenderStart;
	std::chrono::high_resolution_clock::time_point mRenderFinish;
	std::chrono::nanoseconds mRenderTime;
	std::chrono::high_resolution_clock::time_point mNetworkStart;
	std::chrono::high_resolution_clock::time_point mNetworkFinish;
	std::chrono::nanoseconds mNetworkTime;

	//Entity management
	void AssignEntity(const Entity& pEntity);
	void ReAssignEntity(const Entity& pEntity);

	//Private constructor for singleton pattern
	ECSManager();

public:
	~ECSManager();

	//Singleton pattern
	//Deleted copy constructor and assignment operator so no copies of the singleton instance can be made
	ECSManager(ECSManager const&) = delete;
	ECSManager& operator=(ECSManager const&) = delete;	
	static std::shared_ptr<ECSManager> Instance();

	//Frequencies get/sets
	int& TargetRenderingFrequency();
	int& TargetNetworkingFrequency();
	const int& RenderingFrequency();
	const int& NetworkingFrequency();

	//Entity creation
	const int CreateEntity();
	void DestroyEntity(const int pEntityID);

	//System management
	void AddUpdateSystem(std::shared_ptr<ISystem> pSystem);
	void AddRenderSystem(std::shared_ptr<ISystem> pSystem);
	void AddNetworkSystem(std::shared_ptr<ISystem> pSystem);
	void ProcessSystems();

	//Add methods for components
	void AddAIComp(const AI& pAI, const int pEntityID);
	void AddAudioComp(const Audio& pAudio, const int pEntityID);
	void AddBoxColliderComp(const BoxCollider& pBoxCollider, const int pEntityID);
	void AddCameraComp(const Camera& pCamera, const int pEntityID);
	void AddColourComp(const Colour& pColour, const int pEntityID);
	void AddGeometryComp(const Geometry& pGeometry, const int pEntityID);
	void AddLightComp(const Light& pLight, const int pEntityID);
	void AddRayComp(const Ray& pRay, const int pEntityID);
	void AddShaderComp(const Shader& pShader, const int pEntityID);
	void AddSphereColliderComp(const SphereCollider& pSphereCollider, const int pEntityID);
	void AddTextureComp(const Texture& pTexture, const int pEntityID);
	void AddTransformComp(const Transform& pTransform, const int pEntityID);
	void AddVelocityComp(const Velocity& pVelocity, const int pEntityID);
	void AddWeightComp(const Weight& pWeight, const int pEntityID);

	//Remove methods for components
	void RemoveAIComp(const int pEntityID);
	void RemoveAudioComp(const int pEntityID);
	void RemoveBoxColliderComp(const int pEntityID);
	void RemoveCameraComp(const int pEntityID);
	void RemoveColourComp(const int pEntityID);
	void RemoveGeometryComp(const int pEntityID);
	void RemoveLightComp(const int pEntityID);
	void RemoveRayComp(const int pEntityID);
	void RemoveShaderComp(const int pEntityID);
	void RemoveSphereColliderComp(const int pEntityID);
	void RemoveTextureComp(const int pEntityID);
	void RemoveTransformComp(const int pEntityID);
	void RemoveVelocityComp(const int pEntityID);
	void RemoveWeightComp(const int pEntityID);


	//Accessors
	AI* const AIComp(const int pEntityID);
	Audio* const AudioComp(const int pEntityID);
	BoxCollider* const BoxColliderComp(const int pEntityID);
	Camera* const CameraComp(const int pEntityID);
	Colour* const ColourComp(const int pEntityID);
	Geometry* const GeometryComp(const int pEntityID);
	Gravity* const GravityComp(const int pEntityID);
	Light* const LightComp(const int pEntityID);
	Ray* const RayComp(const int pEntityID);
	Shader* const ShaderComp(const int pEntityID);
	SphereCollider* const SphereColliderComp(const int pEntityID);
	Texture* const TextureComp(const int pEntityID);
	Transform* const TransformComp(const int pEntityID);
	Velocity* const VelocityComp(const int pEntityID);
	Weight* const WeightComp(const int pEntityID);
};
