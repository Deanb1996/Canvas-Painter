#include "ECSManager.h"

using namespace std;

/// <summary>
/// Assigns given entity to all appropriate systems
/// </summary>
/// <param name="pEntity">The given entity to assign to systems</param>
void ECSManager::AssignEntity(const Entity & pEntity)
{
	for (auto & system : mRenderSystems)
	{
		system->AssignEntity(pEntity);
	}

	for (auto & system : mUpdateSystems)
	{
		system->AssignEntity(pEntity);
	}
}

/// <summary>
/// Finds an entity based on a given name
/// </summary>
/// <param name="pEntityName">Given name of the entity</param>
/// <returns>Entity that matches given name</returns>
Entity* const ECSManager::FindEntityByName(const std::string & pEntityName)
{
	//Finds if an entity with this name already exists then returns an iterator to the entity if it does
	auto entity = find_if(mEntities.begin(), mEntities.end(), [&](const Entity& entity) {return entity.mName == pEntityName; });

	if (entity != mEntities.end())
	{
		return &*entity;
	}
	else
	{
		return nullptr;
	}
}

/// <summary>
/// Constructor for ECS Manager
/// Resizes entity and component vectors upon construction to a reasonably large size to avoid performance overhead of resizing
/// </summary>
ECSManager::ECSManager()
	:mEntityID(0)
{
		mEntities.reserve(200000);
		mTransforms.reserve(200000);
		mBoxColliders.reserve(200000);
		mGeometries.reserve(200000);
		mTextures.reserve(200000);
		mShaders.reserve(200000);
		mCameras.reserve(200000);
		mLights.reserve(200000);
}

/// <summary>
/// Default destructor
/// </summary>
ECSManager::~ECSManager()
{
}

/// <summary>
/// Creates a singleton instance of ECS Manager if one hasn't been created before
/// Returns pointer to the instance of ECS Manager
/// </summary>
/// <returns>Shared pointer to the ECS Manager instance</returns>
std::shared_ptr<ECSManager> ECSManager::Instance()
{
	static std::shared_ptr<ECSManager> instance{ new ECSManager };
	return instance;
}

/// <summary>
/// Creates an entity with the given name
/// Appends (nameCount) to the end of the entity name if entity name is duplicate
/// </summary>
/// <param name="pEntityName">Given name of the entity to create</param>
void ECSManager::CreateEntity(const std::string & pEntityName)
{
	Entity newEntity;

	//Finds if a pair with this name already exists and returns an iterator to the <name, count> pair value if it does otherwise returns an iterator pointing to the end of the vector
	auto entityName = find_if(mEntityNames.begin(), mEntityNames.end(), [&](const pair<string, int>& pair) {return pair.first == pEntityName; });

	//If the iterator is not pointing to the end of the vector, appends (nameCount) to the end of the given name and creates a new entity with that name, then iterates the name count by 1
	if (entityName != mEntityNames.end())
	{
		newEntity = Entity{ pEntityName + "(" + to_string(entityName->second) + ")", mEntityID, ComponentType::COMPONENT_NONE };
		mEntities.push_back(newEntity);
		entityName->second++;
		mEntityID++;
	}
	//Else creates entity with given name and adds name to name vector with count 1
	else
	{
		newEntity = Entity{ pEntityName, mEntityID, ComponentType::COMPONENT_NONE };
		mEntities.push_back(newEntity);
		mEntityNames.push_back(std::pair<string, int>(pEntityName, 1));
		mEntityID++;
	}
}

/// <summary>
/// Destroys an entity with the given name and all components owned by it
/// </summary>
/// <param name="pEntityName">Given name of the entity to delete</param>
void ECSManager::DestroyEntity(const std::string & pEntityName)
{
	Entity* entity = FindEntityByName(pEntityName);

	//Removes all components owned by this entity
	//BoxCollider Comp
	if ((entity->mComponentMask & ComponentType::COMPONENT_BOXCOLLIDER) == ComponentType::COMPONENT_BOXCOLLIDER)
	{
		RemoveBoxColliderComp(pEntityName);
	}
	//Camera Comp
	if ((entity->mComponentMask & ComponentType::COMPONENT_CAMERA) == ComponentType::COMPONENT_CAMERA)
	{
		RemoveCameraComp(pEntityName);
	}
	//Geometry Comp
	if ((entity->mComponentMask & ComponentType::COMPONENT_GEOMETRY) == ComponentType::COMPONENT_GEOMETRY)
	{
		RemoveGeometryComp(pEntityName);
	}
	//Light Comp
	if ((entity->mComponentMask & ComponentType::COMPONENT_LIGHT) == ComponentType::COMPONENT_LIGHT)
	{
		RemoveLightComp(pEntityName);
	}
	//Shader Comp
	if ((entity->mComponentMask & ComponentType::COMPONENT_SHADER) == ComponentType::COMPONENT_SHADER)
	{
		RemoveShaderComp(pEntityName);
	}
	//Texture Comp
	if ((entity->mComponentMask & ComponentType::COMPONENT_TEXTURE) == ComponentType::COMPONENT_TEXTURE)
	{
		RemoveTextureComp(pEntityName);
	}
	//Transform Comp
	if ((entity->mComponentMask & ComponentType::COMPONENT_TRANSFORM) == ComponentType::COMPONENT_TRANSFORM)
	{
		RemoveTransformComp(pEntityName);
	}

	//Finds the entity with the matching name and removes it from the entities vector
	mEntities.erase(remove_if(mEntities.begin(), mEntities.end(), [&](const Entity& entity) {return entity.mName == pEntityName; }), mEntities.end());
}

/// <summary>
/// Adds the given system to the update system vector
/// </summary>
/// <param name="pSystem">Pointer to the given system</param>
void ECSManager::AddUpdateSystem(shared_ptr<ISystem> pSystem)
{
	mUpdateSystems.push_back(pSystem);
}

/// <summary>
/// Adds the given system to the render system vector
/// </summary>
/// <param name="pSystem">Pointer to the given system</param>
void ECSManager::AddRenderSystem(shared_ptr<ISystem> pSystem)
{
	mRenderSystems.push_back(pSystem);
}

/// <summary>
/// Calls the process method for all systems in the ECS
/// </summary>
void ECSManager::ProcessSystems()
{
	for (auto & system : mUpdateSystems)
	{
		system->Process();
	}

	for (auto & system : mRenderSystems)
	{
		system->Process();
	}
}

/// <summary>
/// Adds a BoxCollider component to the entity with a given name
/// </summary>
/// <param name="pBoxCollider">BoxCollider component to add</param>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::AddBoxColliderComp(const BoxCollider & pBoxCollider, const std::string & pEntityName)
{
	Entity* entity = FindEntityByName(pEntityName);
	if (entity)
	{
		mBoxColliders.push_back(pair<int, BoxCollider>(entity->mID, pBoxCollider));
		entity->mComponentMask |= ComponentType::COMPONENT_BOXCOLLIDER;
		AssignEntity(*entity);
	}
}

/// <summary>
/// Removes a BoxCollider component from the entity with a given name
/// </summary>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::RemoveBoxColliderComp(const std::string & pEntityName)
{
	Entity* entity = FindEntityByName(pEntityName);
	if (entity)
	{
		mBoxColliders.erase(remove_if(mBoxColliders.begin(), mBoxColliders.end(), [&](const pair<int, BoxCollider>& pair) {return pair.first == entity->mID; }));
		entity->mComponentMask = entity->mComponentMask &= ~ComponentType::COMPONENT_BOXCOLLIDER; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
		AssignEntity(*entity);
	}
}

/// <summary>
/// Adds a Camera component to the entity with a given name
/// </summary>
/// <param name="pCamera">Camera component to add</param>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::AddCameraComp(const Camera & pCamera, const std::string & pEntityName)
{
	Entity* entity = FindEntityByName(pEntityName);
	if (entity)
	{
		mCameras.push_back(pair<int, Camera>(entity->mID, pCamera));
		entity->mComponentMask |= ComponentType::COMPONENT_CAMERA;
		AssignEntity(*entity);
	}
}

/// <summary>
/// Removes a Camera component from the entity with a given name
/// </summary>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::RemoveCameraComp(const std::string & pEntityName)
{
	Entity* entity = FindEntityByName(pEntityName);
	if (entity)
	{
		mCameras.erase(remove_if(mCameras.begin(), mCameras.end(), [&](const pair<int, Camera>& pair) {return pair.first == entity->mID; }));
		entity->mComponentMask = entity->mComponentMask &= ~ComponentType::COMPONENT_CAMERA; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
		AssignEntity(*entity);
	}
}

/// <summary>
/// Adds a Geometry component to the entity with a given name
/// </summary>
/// <param name="pGeometry">Geometry component to add</param>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::AddGeometryComp(const Geometry & pGeometry, const std::string & pEntityName)
{
	Entity* entity = FindEntityByName(pEntityName);
	if (entity)
	{
		mGeometries.push_back(pair<int, Geometry>(entity->mID, pGeometry));
		entity->mComponentMask |= ComponentType::COMPONENT_GEOMETRY;
		AssignEntity(*entity);
	}
}

/// <summary>
/// Removes a Geometry component from the entity with a given name
/// </summary>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::RemoveGeometryComp(const std::string & pEntityName)
{
	Entity* entity = FindEntityByName(pEntityName);
	if (entity)
	{
		mGeometries.erase(remove_if(mGeometries.begin(), mGeometries.end(), [&](const pair<int, Geometry>& pair) {return pair.first == entity->mID; }));
		entity->mComponentMask = entity->mComponentMask &= ~ComponentType::COMPONENT_GEOMETRY; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
		AssignEntity(*entity);
	}
}

/// <summary>
/// Adds a Light component to the entity with a given name
/// </summary>
/// <param name="pLight">Light component to add</param>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::AddLightComp(const Light & pLight, const std::string & pEntityName)
{
	Entity* entity = FindEntityByName(pEntityName);
	if (entity)
	{
		mLights.push_back(pair<int, Light>(entity->mID, pLight));
		entity->mComponentMask |= ComponentType::COMPONENT_LIGHT;
		AssignEntity(*entity);
	}
}

/// <summary>
/// Removes a light component from the entity with a given name
/// </summary>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::RemoveLightComp(const std::string & pEntityName)
{
	Entity* entity = FindEntityByName(pEntityName);
	if (entity)
	{
		mLights.erase(remove_if(mLights.begin(), mLights.end(), [&](const pair<int, Light>& pair) {return pair.first == entity->mID; }));
		entity->mComponentMask = entity->mComponentMask &= ~ComponentType::COMPONENT_LIGHT; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
		AssignEntity(*entity);
	}
}

/// <summary>
/// Adds a Shader component to the entity with a given name
/// </summary>
/// <param name="pShader">Shader component to add</param>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::AddShaderComp(const Shader & pShader, const std::string & pEntityName)
{
	Entity* entity = FindEntityByName(pEntityName);
	if (entity)
	{
		mShaders.push_back(pair<int, Shader>(entity->mID, pShader));
		entity->mComponentMask |= ComponentType::COMPONENT_SHADER;
		AssignEntity(*entity);
	}
}

/// <summary>
/// Removes a Shader component from the entity with a given name
/// </summary>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::RemoveShaderComp(const std::string & pEntityName)
{
	Entity* entity = FindEntityByName(pEntityName);
	if (entity)
	{
		mShaders.erase(remove_if(mShaders.begin(), mShaders.end(), [&](const pair<int, Shader>& pair) {return pair.first == entity->mID; }));
		entity->mComponentMask = entity->mComponentMask &= ~ComponentType::COMPONENT_SHADER; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
		AssignEntity(*entity);
	}
}

/// <summary>
/// Adds a Texture component to the entity with a given name
/// </summary>
/// <param name="pTexture">Texture component to add</param>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::AddTextureComp(const Texture & pTexture, const std::string & pEntityName)
{
	Entity* entity = FindEntityByName(pEntityName);
	if (entity)
	{
		mTextures.push_back(pair<int, Texture>(entity->mID, pTexture));
		entity->mComponentMask |= ComponentType::COMPONENT_TEXTURE;
		AssignEntity(*entity);
	}
}

/// <summary>
/// Removes a Texture component from the entity with a given name
/// </summary>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::RemoveTextureComp(const std::string & pEntityName)
{
	Entity* entity = FindEntityByName(pEntityName);
	if (entity)
	{
		mTextures.erase(remove_if(mTextures.begin(), mTextures.end(), [&](const pair<int, Texture>& pair) {return pair.first == entity->mID; }));
		entity->mComponentMask = entity->mComponentMask &= ~ComponentType::COMPONENT_TEXTURE; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
		AssignEntity(*entity);
	}
}

/// <summary>
/// Adds a Transform component to the entity with a given name
/// </summary>
/// <param name="pTransform">Transform component to add</param>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::AddTransformComp(const Transform & pTransform, const std::string & pEntityName)
{
	Entity* entity = FindEntityByName(pEntityName);
	if (entity)
	{
		mTransforms.push_back(pair<int, Transform>(entity->mID, pTransform));
		entity->mComponentMask |= ComponentType::COMPONENT_TRANSFORM;
		AssignEntity(*entity);
	}
}

/// <summary>
/// Removes a Transform component from the entity with a given name
/// </summary>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::RemoveTransformComp(const std::string & pEntityName)
{
	Entity* entity = FindEntityByName(pEntityName);
	if (entity)
	{
		mTransforms.erase(remove_if(mTransforms.begin(), mTransforms.end(), [&](const pair<int, Transform>& pair) {return pair.first == entity->mID; }));
		entity->mComponentMask = entity->mComponentMask &= ~ComponentType::COMPONENT_TRANSFORM; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
		AssignEntity(*entity);
	}
}

/// <summary>
/// Returns a modifiable handle to the BoxCollider component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to BoxCollider component</returns>
BoxCollider * const ECSManager::BoxColliderComp(const int & pEntityID)
{
	//Finds the ID/Component pair for the given entity ID
	auto comp = find_if(mBoxColliders.begin(), mBoxColliders.end(), [&](const pair<int, BoxCollider>& pair) {return pair.first == pEntityID; });

	if (comp != mBoxColliders.end())
	{
		return &comp->second;
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the BoxCollider component associated with the given entity name
/// </summary>
/// <param name="pEntityName">Given entity name</param>
/// <returns>Modifiable handle to the BoxCollider component</returns>
BoxCollider * const ECSManager::BoxColliderComp(const std::string & pEntityName)
{
	Entity* entity = FindEntityByName(pEntityName);
	if (entity)
	{
		//Finds the ID/Component pair for the given entity ID
		auto comp = find_if(mBoxColliders.begin(), mBoxColliders.end(), [&](const pair<int, BoxCollider>& pair) {return pair.first == entity->mID; });

		if (comp != mBoxColliders.end())
		{
			return &comp->second;
		}
		return nullptr;
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Camera component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to Camera component</returns>
Camera * const ECSManager::CameraComp(const int & pEntityID)
{
	//Finds the ID/Component pair for the given entity ID
	auto comp = find_if(mCameras.begin(), mCameras.end(), [&](const pair<int, Camera>& pair) {return pair.first == pEntityID; });

	if (comp != mCameras.end())
	{
		return &comp->second;
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Camera component associated with the given entity name
/// </summary>
/// <param name="pEntityName">Given entity name</param>
/// <returns>Modifiable handle to the Camera component</returns>
Camera * const ECSManager::CameraComp(const std::string pEntityName)
{
	Entity* entity = FindEntityByName(pEntityName);
	if (entity)
	{
		//Finds the ID/Component pair for the given entity ID
		auto comp = find_if(mCameras.begin(), mCameras.end(), [&](const pair<int, Camera>& pair) {return pair.first == entity->mID; });

		if (comp != mCameras.end())
		{
			return &comp->second;
		}
		return nullptr;
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Geometry component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to Geometry component</returns>
Geometry * const ECSManager::GeometryComp(const int & pEntityID)
{
	//Finds the ID/Component pair for the given entity ID
	auto comp = find_if(mGeometries.begin(), mGeometries.end(), [&](const pair<int, Geometry>& pair) {return pair.first == pEntityID; });

	if (comp != mGeometries.end())
	{
		return &comp->second;
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Geometry component associated with the given entity name
/// </summary>
/// <param name="pEntityName">Given entity name</param>
/// <returns>Modifiable handle to the Geometry component</returns>
Geometry * const ECSManager::GeometryComp(const std::string & pEntityName)
{
	Entity* entity = FindEntityByName(pEntityName);
	if (entity)
	{
		//Finds the ID/Component pair for the given entity ID
		auto comp = find_if(mGeometries.begin(), mGeometries.end(), [&](const pair<int, Geometry>& pair) {return pair.first == entity->mID; });

		if (comp != mGeometries.end())
		{
			return &comp->second;
		}
		return nullptr;
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Light component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to AI component</returns>
Light * const ECSManager::LightComp(const int & pEntityID)
{
	//Finds the ID/Component pair for the given entity ID
	auto comp = find_if(mLights.begin(), mLights.end(), [&](const pair<int, Light>& pair) {return pair.first == pEntityID; });

	if (comp != mLights.end())
	{
		return &comp->second;
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Light component associated with the given entity name
/// </summary>
/// <param name="pEntityName">Given entity name</param>
/// <returns>Modifiable handle to the Light component</returns>
Light * const ECSManager::LightComp(const std::string & pEntityName)
{
	Entity* entity = FindEntityByName(pEntityName);
	if (entity)
	{
		//Finds the ID/Component pair for the given entity ID
		auto comp = find_if(mLights.begin(), mLights.end(), [&](const pair<int, Light>& pair) {return pair.first == entity->mID; });

		if (comp != mLights.end())
		{
			return &comp->second;
		}
		return nullptr;
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Shader component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to Shader component</returns>
Shader * const ECSManager::ShaderComp(const int & pEntityID)
{
	//Finds the ID/Component pair for the given entity ID
	auto comp = find_if(mShaders.begin(), mShaders.end(), [&](const pair<int, Shader>& pair) {return pair.first == pEntityID; });

	if (comp != mShaders.end())
	{
		return &comp->second;
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Shader component associated with the given entity name
/// </summary>
/// <param name="pEntityName">Given entity name</param>
/// <returns>Modifiable handle to the Shader component</returns>
Shader * const ECSManager::ShaderComp(const std::string & pEntityName)
{
	Entity* entity = FindEntityByName(pEntityName);
	if (entity)
	{
		//Finds the ID/Component pair for the given entity ID
		auto comp = find_if(mShaders.begin(), mShaders.end(), [&](const pair<int, Shader>& pair) {return pair.first == entity->mID; });

		if (comp != mShaders.end())
		{
			return &comp->second;
		}
		return nullptr;
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Texture component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to Texture component</returns>
Texture * const ECSManager::TextureComp(const int & pEntityID)
{
	//Finds the ID/Component pair for the given entity ID
	auto comp = find_if(mTextures.begin(), mTextures.end(), [&](const pair<int, Texture>& pair) {return pair.first == pEntityID; });

	if (comp != mTextures.end())
	{
		return &comp->second;
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Texture component associated with the given entity name
/// </summary>
/// <param name="pEntityName">Given entity name</param>
/// <returns>Modifiable handle to the Texture component</returns>
Texture * const ECSManager::TextureComp(const std::string & pEntityName)
{
	Entity* entity = FindEntityByName(pEntityName);
	if (entity)
	{
		//Finds the ID/Component pair for the given entity ID
		auto comp = find_if(mTextures.begin(), mTextures.end(), [&](const pair<int, Texture>& pair) {return pair.first == entity->mID; });

		if (comp != mTextures.end())
		{
			return &comp->second;
		}
		return nullptr;
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Transform component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to Transform component</returns>
Transform * const ECSManager::TransformComp(const int & pEntityID)
{
	//Finds the ID/Component pair for the given entity ID
	auto comp = find_if(mTransforms.begin(), mTransforms.end(), [&](const pair<int, Transform>& pair) {return pair.first == pEntityID; });

	if (comp != mTransforms.end())
	{
		return &comp->second;
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Transform component associated with the given entity name
/// </summary>
/// <param name="pEntityName">Given entity name</param>
/// <returns>Modifiable handle to the Transform component</returns>
Transform * const ECSManager::TransformComp(const std::string & pEntityName)
{
	Entity* entity = FindEntityByName(pEntityName);
	if (entity)
	{
		//Finds the ID/Component pair for the given entity ID
		auto comp = find_if(mTransforms.begin(), mTransforms.end(), [&](const pair<int, Transform>& pair) {return pair.first == entity->mID; });

		if (comp != mTransforms.end())
		{
			return &comp->second;
		}
		return nullptr;
	}
	return nullptr;
}
