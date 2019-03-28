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
	std::vector<std::pair<std::string, int>> mEntityNames;
	std::vector<std::pair<int, Transform>> mTransforms;
	std::vector<std::pair<int, BoxCollider>> mBoxColliders;
	std::vector<std::pair<int, Geometry>> mGeometries;
	std::vector<std::pair<int, Texture>> mTextures;
	std::vector<std::pair<int, Shader>> mShaders;
	std::vector<std::pair<int, Camera>> mCameras;
	std::vector<std::pair<int, Light>> mLights;
	std::vector<std::shared_ptr<ISystem>> mRenderSystems;
	std::vector<std::shared_ptr<ISystem>> mUpdateSystems;
	int mEntityID;

	ECSManager();

	//Entity management
	void AssignEntity(const Entity& pEntity);
	Entity* const FindEntityByName(const std::string& pEntityName);

public:
	~ECSManager();

	//Singleton pattern
	//Deleted copy constructor and assignment operator so no copies of the singleton instance can be made
	ECSManager(ECSManager const&) = delete;
	ECSManager& operator=(ECSManager const&) = delete;	
	static std::shared_ptr<ECSManager> Instance();

	//Entity creation
	void CreateEntity(const std::string& pEntityName);
	void DestroyEntity(const std::string& pEntityName);

	//System management
	void AddUpdateSystem(std::shared_ptr<ISystem> pSystem);
	void AddRenderSystem(std::shared_ptr<ISystem> pSystem);
	void ProcessSystems();

	//Add/Remove methods for components
	void AddBoxColliderComp(const BoxCollider& pBoxCollider, const std::string& pEntityName);
	void RemoveBoxColliderComp(const std::string& pEntityName);
	void AddCameraComp(const Camera& pCamera, const std::string& pEntityName);
	void RemoveCameraComp(const std::string& pEntityName);
	void AddGeometryComp(const Geometry& pGeometry, const std::string& pEntityName);
	void RemoveGeometryComp(const std::string& pEntityName);
	void AddLightComp(const Light& pLight, const std::string& pEntityName);
	void RemoveLightComp(const std::string& pEntityName);
	void AddShaderComp(const Shader& pShader, const std::string& pEntityName);
	void RemoveShaderComp(const std::string& pEntityName);
	void AddTextureComp(const Texture& pTexture, const std::string& pEntityName);
	void RemoveTextureComp(const std::string& pEntityName);
	void AddTransformComp(const Transform& pTransform, const std::string& pEntityName);
	void RemoveTransformComp(const std::string& pEntityName);

	//Accessors
	BoxCollider* const BoxColliderComp(const int& pEntityID);
	BoxCollider* const BoxColliderComp(const std::string& pEntityName);
	Camera* const CameraComp(const int& pEntityID);
	Camera* const CameraComp(const std::string pEntityName);
	Geometry* const GeometryComp(const int& pEntityID);
	Geometry* const GeometryComp(const std::string& pEntityName);
	Light* const LightComp(const int& pEntityID);
	Light* const LightComp(const std::string& pEntityName);
	Shader* const ShaderComp(const int& pEntityID);
	Shader* const ShaderComp(const std::string& pEntityName);
	Texture* const TextureComp(const int& pEntityID);
	Texture* const TextureComp(const std::string& pEntityName);
	Transform* const TransformComp(const int& pEntityID);
	Transform* const TransformComp(const std::string& pEntityName);
};
