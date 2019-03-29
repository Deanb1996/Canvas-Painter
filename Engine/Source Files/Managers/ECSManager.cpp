#include "ECSManager.h"

using namespace std;

/// <summary>
/// Assigns given entity to all appropriate systems upon addition of new component
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
/// Re-assigns given entity to all appropriate systems upon removal of component
/// </summary>
/// <param name="pEntity">Entity to re-assign</param>
void ECSManager::ReAssignEntity(const Entity & pEntity)
{
	for (auto & system : mRenderSystems)
	{
		system->ReAssignEntity(pEntity);
	}

	for (auto & system : mUpdateSystems)
	{
		system->ReAssignEntity(pEntity);
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
	mTransforms.resize(200000);
	mBoxColliders.resize(200000);
	mGeometries.resize(200000);
	mTextures.resize(200000);
	mShaders.resize(200000);
	mCameras.resize(200000);
	mLights.resize(200000);
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
/// </summary>
/// <param name="pEntityName">Given name of the entity to create</param>
const int ECSManager::CreateEntity()
{
	int entityID = 0;
	if (mFreeEntityIDs.size() > 0)
	{
		entityID = mFreeEntityIDs.back();
		mFreeEntityIDs.pop_back();
	}
	else
	{
		mEntities.emplace_back(Entity{ mEntityID, ComponentType::COMPONENT_NONE });
		entityID = mEntityID;
		mEntityID++;
	}
	return entityID;
}

/// <summary>
/// Destroys an entity with the given name and all components owned by it
/// </summary>
/// <param name="pEntityName">Given name of the entity to delete</param>
void ECSManager::DestroyEntity(const int pEntityID)
{
	//Find entity with matching ID
	Entity* entity = &mEntities[pEntityID];

	//Removes all components owned by this entity
	//AI Comp
	if ((entity->mComponentMask & ComponentType::COMPONENT_AI) == ComponentType::COMPONENT_AI)
	{
		RemoveAIComp(pEntityID);
	}
	//Audio Comp
	if ((entity->mComponentMask & ComponentType::COMPONENT_AUDIO) == ComponentType::COMPONENT_AUDIO)
	{
		RemoveAudioComp(pEntityID);
	}
	//BoxCollider Comp
	if ((entity->mComponentMask & ComponentType::COMPONENT_BOXCOLLIDER) == ComponentType::COMPONENT_BOXCOLLIDER)
	{
		RemoveBoxColliderComp(pEntityID);
	}
	//Camera Comp
	if ((entity->mComponentMask & ComponentType::COMPONENT_CAMERA) == ComponentType::COMPONENT_CAMERA)
	{
		RemoveCameraComp(pEntityID);
	}
	//Colour Comp
	if ((entity->mComponentMask & ComponentType::COMPONENT_COLOUR) == ComponentType::COMPONENT_COLOUR)
	{
		RemoveColourComp(pEntityID);
	}
	//Geometry Comp
	if ((entity->mComponentMask & ComponentType::COMPONENT_GEOMETRY) == ComponentType::COMPONENT_GEOMETRY)
	{
		RemoveGeometryComp(pEntityID);
	}
	//Light Comp
	if ((entity->mComponentMask & ComponentType::COMPONENT_LIGHT) == ComponentType::COMPONENT_LIGHT)
	{
		RemoveLightComp(pEntityID);
	}
	//Shader Comp
	if ((entity->mComponentMask & ComponentType::COMPONENT_SHADER) == ComponentType::COMPONENT_SHADER)
	{
		RemoveShaderComp(pEntityID);
	}
	//SphereCollider Comp
	if ((entity->mComponentMask & ComponentType::COMPONENT_SPHERECOLLIDER) == ComponentType::COMPONENT_SPHERECOLLIDER)
	{
		RemoveSphereColliderComp(pEntityID);
	}
	//Texture Comp
	if ((entity->mComponentMask & ComponentType::COMPONENT_TEXTURE) == ComponentType::COMPONENT_TEXTURE)
	{
		RemoveTextureComp(pEntityID);
	}
	//Transform Comp
	if ((entity->mComponentMask & ComponentType::COMPONENT_TRANSFORM) == ComponentType::COMPONENT_TRANSFORM)
	{
		RemoveTransformComp(pEntityID);
	}
	//Velocity Comp
	if ((entity->mComponentMask & ComponentType::COMPONENT_VELOCITY) == ComponentType::COMPONENT_VELOCITY)
	{
		RemoveVelocityComp(pEntityID);
	}

	//Finds the entity with the matching ID and removes it from the entities vector
	mEntities[pEntityID] = Entity{};
	mFreeEntityIDs.push_back(pEntityID);
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
/// 
/// </summary>
/// <param name="pAI"></param>
/// <param name="pEntityID"></param>
void ECSManager::AddAIComp(const AI & pAI, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mAIs[pEntityID] = pAI;
	entity->mComponentMask |= ComponentType::COMPONENT_AI;
	AssignEntity(*entity);
}

/// <summary>
/// 
/// </summary>
/// <param name="pAudio"></param>
/// <param name="pEntityID"></param>
void ECSManager::AddAudioComp(const Audio & pAudio, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mAudios[pEntityID] = pAudio;
	entity->mComponentMask |= ComponentType::COMPONENT_AUDIO;
	AssignEntity(*entity);
}

/// <summary>
/// Adds a BoxCollider component to the entity with a given name
/// </summary>
/// <param name="pBoxCollider">BoxCollider component to add</param>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::AddBoxColliderComp(const BoxCollider & pBoxCollider, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mBoxColliders[pEntityID] = pBoxCollider;
	entity->mComponentMask |= ComponentType::COMPONENT_BOXCOLLIDER;
	AssignEntity(*entity);
}

/// <summary>
/// Adds a Camera component to the entity with a given name
/// </summary>
/// <param name="pCamera">Camera component to add</param>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::AddCameraComp(const Camera & pCamera, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mCameras[pEntityID] = pCamera;
	entity->mComponentMask |= ComponentType::COMPONENT_CAMERA;
	AssignEntity(*entity);
}

/// <summary>
/// 
/// </summary>
/// <param name="pColour"></param>
/// <param name="pEntityID"></param>
void ECSManager::AddColourComp(const Colour & pColour, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mColours[pEntityID] = pColour;
	entity->mComponentMask |= ComponentType::COMPONENT_COLOUR;
	AssignEntity(*entity);
}

/// <summary>
/// Adds a Geometry component to the entity with a given name
/// </summary>
/// <param name="pGeometry">Geometry component to add</param>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::AddGeometryComp(const Geometry & pGeometry, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mGeometries[pEntityID] = pGeometry;
	entity->mComponentMask |= ComponentType::COMPONENT_GEOMETRY;
	AssignEntity(*entity);
}

/// <summary>
/// Adds a Light component to the entity with a given name
/// </summary>
/// <param name="pLight">Light component to add</param>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::AddLightComp(const Light & pLight, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mLights[pEntityID] = pLight;
	entity->mComponentMask |= ComponentType::COMPONENT_LIGHT;
	AssignEntity(*entity);
}

/// <summary>
/// Adds a Shader component to the entity with a given name
/// </summary>
/// <param name="pShader">Shader component to add</param>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::AddShaderComp(const Shader & pShader, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mShaders[pEntityID] = pShader;
	entity->mComponentMask |= ComponentType::COMPONENT_SHADER;
	AssignEntity(*entity);
}

/// <summary>
/// 
/// </summary>
/// <param name="pSphereCollider"></param>
/// <param name="pEntityID"></param>
void ECSManager::AddSphereColliderComp(const SphereCollider & pSphereCollider, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mSphereColliders[pEntityID] = pSphereCollider;
	entity->mComponentMask |= ComponentType::COMPONENT_SPHERECOLLIDER;
	AssignEntity(*entity);
}

/// <summary>
/// Adds a Texture component to the entity with a given name
/// </summary>
/// <param name="pTexture">Texture component to add</param>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::AddTextureComp(const Texture & pTexture, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mTextures[pEntityID] = pTexture;
	entity->mComponentMask |= ComponentType::COMPONENT_TEXTURE;
	AssignEntity(*entity);
}

/// <summary>
/// Adds a Transform component to the entity with a given name
/// </summary>
/// <param name="pTransform">Transform component to add</param>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::AddTransformComp(const Transform & pTransform, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mTransforms[pEntityID] = pTransform;
	entity->mComponentMask |= ComponentType::COMPONENT_TRANSFORM;
	AssignEntity(*entity);
}

/// <summary>
/// 
/// </summary>
/// <param name="pVelocity"></param>
/// <param name="pEntityID"></param>
void ECSManager::AddVelocityComp(const Velocity & pVelocity, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mVelocities[pEntityID] = pVelocity;
	entity->mComponentMask |= ComponentType::COMPONENT_VELOCITY;
	AssignEntity(*entity);
}

/// <summary>
/// 
/// </summary>
/// <param name="pEntityID"></param>
void ECSManager::RemoveAIComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->mComponentMask = entity->mComponentMask &= ~ComponentType::COMPONENT_AI; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}

/// <summary>
/// 
/// </summary>
/// <param name="pEntityID"></param>
void ECSManager::RemoveAudioComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->mComponentMask = entity->mComponentMask &= ~ComponentType::COMPONENT_AUDIO; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}

/// <summary>
/// Removes a BoxCollider component from the entity with a given name
/// </summary>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::RemoveBoxColliderComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->mComponentMask = entity->mComponentMask &= ~ComponentType::COMPONENT_BOXCOLLIDER; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}


/// <summary>
/// Removes a Camera component from the entity with a given name
/// </summary>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::RemoveCameraComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->mComponentMask = entity->mComponentMask &= ~ComponentType::COMPONENT_CAMERA; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}

/// <summary>
/// 
/// </summary>
/// <param name="pEntityID"></param>
void ECSManager::RemoveColourComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->mComponentMask = entity->mComponentMask &= ~ComponentType::COMPONENT_COLOUR; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}


/// <summary>
/// Removes a Geometry component from the entity with a given name
/// </summary>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::RemoveGeometryComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->mComponentMask = entity->mComponentMask &= ~ComponentType::COMPONENT_GEOMETRY; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}


/// <summary>
/// Removes a light component from the entity with a given name
/// </summary>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::RemoveLightComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->mComponentMask = entity->mComponentMask &= ~ComponentType::COMPONENT_LIGHT; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}


/// <summary>
/// Removes a Shader component from the entity with a given name
/// </summary>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::RemoveShaderComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->mComponentMask = entity->mComponentMask &= ~ComponentType::COMPONENT_SHADER; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}

/// <summary>
/// 
/// </summary>
/// <param name="pEntityID"></param>
void ECSManager::RemoveSphereColliderComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->mComponentMask = entity->mComponentMask &= ~ComponentType::COMPONENT_SPHERECOLLIDER; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}

/// <summary>
/// Removes a Texture component from the entity with a given name
/// </summary>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::RemoveTextureComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->mComponentMask = entity->mComponentMask &= ~ComponentType::COMPONENT_TEXTURE; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}


/// <summary>
/// Removes a Transform component from the entity with a given name
/// </summary>
/// <param name="pEntityName">Given name of the entity</param>
void ECSManager::RemoveTransformComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->mComponentMask = entity->mComponentMask &= ~ComponentType::COMPONENT_TRANSFORM; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}

/// <summary>
/// 
/// </summary>
/// <param name="pEntityID"></param>
void ECSManager::RemoveVelocityComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->mComponentMask = entity->mComponentMask &= ~ComponentType::COMPONENT_VELOCITY; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}

/// <summary>
/// 
/// </summary>
/// <param name="pEntityID"></param>
/// <returns></returns>
AI * const ECSManager::AIComp(const int pEntityID)
{
	return &mAIs[pEntityID];
}

/// <summary>
/// 
/// </summary>
/// <param name="pEntityID"></param>
/// <returns></returns>
Audio * const ECSManager::AudioComp(const int pEntityID)
{
	return &mAudios[pEntityID];
}

/// <summary>
/// Returns a modifiable handle to the BoxCollider component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to BoxCollider component</returns>
BoxCollider * const ECSManager::BoxColliderComp(const int pEntityID)
{
	return &mBoxColliders[pEntityID];
}

/// <summary>
/// Returns a modifiable handle to the Camera component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to Camera component</returns>
Camera * const ECSManager::CameraComp(const int pEntityID)
{
	return &mCameras[pEntityID];
}

/// <summary>
/// 
/// </summary>
/// <param name="pEntityID"></param>
/// <returns></returns>
Colour * const ECSManager::ColourComp(const int pEntityID)
{
	return &mColours[pEntityID];
}

/// <summary>
/// Returns a modifiable handle to the Geometry component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to Geometry component</returns>
Geometry * const ECSManager::GeometryComp(const int pEntityID)
{
	return &mGeometries[pEntityID];
}

/// <summary>
/// 
/// </summary>
/// <param name="pEntityID"></param>
/// <returns></returns>
Gravity * const ECSManager::GravityComp(const int pEntityID)
{
	return &mGravities[pEntityID];
}

/// <summary>
/// Returns a modifiable handle to the Light component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to AI component</returns>
Light * const ECSManager::LightComp(const int pEntityID)
{
	return &mLights[pEntityID];
}

/// <summary>
/// Returns a modifiable handle to the Shader component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to Shader component</returns>
Shader * const ECSManager::ShaderComp(const int pEntityID)
{
	return &mShaders[pEntityID];
}

/// <summary>
/// 
/// </summary>
/// <param name="pEntityID"></param>
/// <returns></returns>
SphereCollider * const ECSManager::SphereColliderComp(const int pEntityID)
{
	return &mSphereColliders[pEntityID];
}

/// <summary>
/// Returns a modifiable handle to the Texture component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to Texture component</returns>
Texture * const ECSManager::TextureComp(const int pEntityID)
{
	return &mTextures[pEntityID];
}

/// <summary>
/// Returns a modifiable handle to the Transform component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to Transform component</returns>
Transform * const ECSManager::TransformComp(const int pEntityID)
{
	return &mTransforms[pEntityID];
}

/// <summary>
/// 
/// </summary>
/// <param name="pEntityID"></param>
/// <returns></returns>
Velocity * const ECSManager::VelocityComp(const int pEntityID)
{
	return &mVelocities[pEntityID];
}
