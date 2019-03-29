#pragma once
#include <memory>
#include <vector>
#include "Components.h"
#include "ISystem.h"
#include "Entity.h"
#include <algorithm>
#include <string>

class ECSManager
{
private:
	std::vector<Entity> mEntities;
	std::vector<int> mFreeEntityIDs;
	std::vector<Transform> mTransforms;
	std::vector<BoxCollider> mBoxColliders;
	std::vector<Geometry> mGeometries;
	std::vector<Texture> mTextures;
	std::vector<Shader> mShaders;
	std::vector<Camera> mCameras;
	std::vector<Light> mLights;
	std::vector<std::shared_ptr<ISystem>> mRenderSystems;
	std::vector<std::shared_ptr<ISystem>> mUpdateSystems;
	int mEntityID;

	ECSManager();

	//Entity management
	void AssignEntity(const Entity& pEntity);
	void ReAssignEntity(const Entity& pEntity);

public:
	~ECSManager();

	//Singleton pattern
	//Deleted copy constructor and assignment operator so no copies of the singleton instance can be made
	ECSManager(ECSManager const&) = delete;
	ECSManager& operator=(ECSManager const&) = delete;	
	static std::shared_ptr<ECSManager> Instance();

	//Entity creation
	const int CreateEntity();
	void DestroyEntity(const int pEntityID);

	//System management
	void AddUpdateSystem(std::shared_ptr<ISystem> pSystem);
	void AddRenderSystem(std::shared_ptr<ISystem> pSystem);
	void ProcessSystems();

	//Add/Remove methods for components
	void AddBoxColliderComp(const BoxCollider& pBoxCollider, const int pEntityID);
	void RemoveBoxColliderComp(const int pEntityID);
	void AddCameraComp(const Camera& pCamera, const int pEntityID);
	void RemoveCameraComp(const int pEntityID);
	void AddGeometryComp(const Geometry& pGeometry, const int pEntityID);
	void RemoveGeometryComp(const int pEntityID);
	void AddLightComp(const Light& pLight, const int pEntityID);
	void RemoveLightComp(const int pEntityID);
	void AddShaderComp(const Shader& pShader, const int pEntityID);
	void RemoveShaderComp(const int pEntityID);
	void AddTextureComp(const Texture& pTexture, const int pEntityID);
	void RemoveTextureComp(const int pEntityID);
	void AddTransformComp(const Transform& pTransform, const int pEntityID);
	void RemoveTransformComp(const int pEntityID);

	//Accessors
	BoxCollider* const BoxColliderComp(const int& pEntityID);
	Camera* const CameraComp(const int& pEntityID);
	Geometry* const GeometryComp(const int& pEntityID);
	Light* const LightComp(const int& pEntityID);
	Shader* const ShaderComp(const int& pEntityID);
	Texture* const TextureComp(const int& pEntityID);
	Transform* const TransformComp(const int& pEntityID);
};
