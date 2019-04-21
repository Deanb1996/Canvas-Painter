#include "ECSManager.h"
#include "RenderSystem_DX.h"

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
	:mEntityID(0), mTargetRenderingFrequency(60), mTargetNetworkingFrequency(10)
{
	mEntities.reserve(200000);
	mAIs.resize(200000);
	mAudios.resize(20000);
	mBoxColliders.resize(200000);
	mCameras.resize(200000);
	mColours.resize(200000);
	mGeometries.resize(200000);
	mGravities.resize(200000);
	mLights.resize(200000);
	mRays.resize(200000);
	mShaders.resize(200000);
	mSphereColliders.resize(200000);
	mTextures.resize(200000);
	mTransforms.resize(200000);
	mVelocities.resize(200000);
	mWeights.resize(200000);
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
/// Get/Set method for the target rendering frequency
/// </summary>
/// <returns>Modifiable handle to </returns>
int & ECSManager::TargetRenderingFrequency()
{
	return mTargetRenderingFrequency;
}

/// <summary>
/// Get/Set method for the target networking frequency
/// </summary>
/// <returns>Modifiable handle to the networking frequency</returns>
int & ECSManager::TargetNetworkingFrequency()
{
	return mTargetNetworkingFrequency;
}

/// <summary>
/// Get method for the rendering frequency
/// </summary>
/// <returns>Rendering frequency</returns>
const int& ECSManager::RenderingFrequency()
{
	return mRenderingFrequency;
}

/// <summary>
/// Get method for the networking frequency
/// </summary>
/// <returns>Networking frequency</returns>
const int& ECSManager::NetworkingFrequency()
{
	return mNetworkingFrequency;
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
		mEntities[entityID] = Entity{ entityID, ComponentType::COMPONENT_NONE };
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
	if ((entity->componentMask & ComponentType::COMPONENT_AI) == ComponentType::COMPONENT_AI)
	{
		RemoveAIComp(pEntityID);
	}
	//Audio Comp
	if ((entity->componentMask & ComponentType::COMPONENT_AUDIO) == ComponentType::COMPONENT_AUDIO)
	{
		RemoveAudioComp(pEntityID);
	}
	//BoxCollider Comp
	if ((entity->componentMask & ComponentType::COMPONENT_BOXCOLLIDER) == ComponentType::COMPONENT_BOXCOLLIDER)
	{
		RemoveBoxColliderComp(pEntityID);
	}
	//Camera Comp
	if ((entity->componentMask & ComponentType::COMPONENT_CAMERA) == ComponentType::COMPONENT_CAMERA)
	{
		RemoveCameraComp(pEntityID);
	}
	//Colour Comp
	if ((entity->componentMask & ComponentType::COMPONENT_COLOUR) == ComponentType::COMPONENT_COLOUR)
	{
		RemoveColourComp(pEntityID);
	}
	//Geometry Comp
	if ((entity->componentMask & ComponentType::COMPONENT_GEOMETRY) == ComponentType::COMPONENT_GEOMETRY)
	{
		RemoveGeometryComp(pEntityID);
	}
	//Light Comp
	if ((entity->componentMask & ComponentType::COMPONENT_LIGHT) == ComponentType::COMPONENT_LIGHT)
	{
		RemoveLightComp(pEntityID);
	}
	//Ray Comp
	if ((entity->componentMask & ComponentType::COMPONENT_RAY) == ComponentType::COMPONENT_RAY)
	{
		RemoveRayComp(pEntityID);
	}
	//Shader Comp
	if ((entity->componentMask & ComponentType::COMPONENT_SHADER) == ComponentType::COMPONENT_SHADER)
	{
		RemoveShaderComp(pEntityID);
	}
	//SphereCollider Comp
	if ((entity->componentMask & ComponentType::COMPONENT_SPHERECOLLIDER) == ComponentType::COMPONENT_SPHERECOLLIDER)
	{
		RemoveSphereColliderComp(pEntityID);
	}
	//Texture Comp
	if ((entity->componentMask & ComponentType::COMPONENT_TEXTURE) == ComponentType::COMPONENT_TEXTURE)
	{
		RemoveTextureComp(pEntityID);
	}
	//Transform Comp
	if ((entity->componentMask & ComponentType::COMPONENT_TRANSFORM) == ComponentType::COMPONENT_TRANSFORM)
	{
		RemoveTransformComp(pEntityID);
	}
	//Velocity Comp
	if ((entity->componentMask & ComponentType::COMPONENT_VELOCITY) == ComponentType::COMPONENT_VELOCITY)
	{
		RemoveVelocityComp(pEntityID);
	}
	//Weight Comp
	if ((entity->componentMask & ComponentType::COMPONENT_WEIGHT) == ComponentType::COMPONENT_WEIGHT)
	{
		RemoveWeightComp(pEntityID);
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
/// Adds the given system to the network system vector
/// </summary>
/// <param name="pSystem">Pointer to the given system</param>
void ECSManager::AddNetworkSystem(std::shared_ptr<ISystem> pSystem)
{
	mNetworkSystems.push_back(pSystem);
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
		//If render task has already been assigned
		if (mRenderTask)
		{
			//Check if render task has been completed
			if (mRenderTask->IsDone())
			{
				//Calculate time taken
				mRenderFinish = std::chrono::high_resolution_clock::now();
				mRenderTime = mRenderFinish - mRenderStart;

				//Convert timings to miliseconds for frequency calculations
				float targetMiliseconds = static_cast<float>(1000 / mTargetRenderingFrequency);
				float renderTimeMiliseconds = static_cast<float>(mRenderTime.count() / pow(10, 6));

				//If render task took longer than target frequency, create new task
				//Else wait until the time meets the target frequency
				if (renderTimeMiliseconds >= targetMiliseconds)
				{
					//Calculate the actual rendering frequency
					mRenderingFrequency = static_cast<int>(1000 / renderTimeMiliseconds);

					// Cleanup then create new task and set start time
					mRenderTask->CleanUpTask();
					mRenderTask = mThreadManager->AddTask(std::bind(&ISystem::Process, system), nullptr, nullptr, std::vector<int>{0});
					mRenderStart = std::chrono::high_resolution_clock::now();
				}
			}
		}
		else
		{
			//Create render task and set start time
			mRenderTask = mThreadManager->AddTask(std::bind(&ISystem::Process, system), nullptr, nullptr, std::vector<int>{0});
			mRenderStart = std::chrono::high_resolution_clock::now();
		}
		//system->Process();
	}

	for (auto & system : mNetworkSystems)
	{
		//If network task has already been assigned
		if (mNetworkingTask)
		{
			//Check if network task has been completed
			if (mNetworkingTask->IsDone())
			{
				//Calculate time taken
				mNetworkFinish = std::chrono::high_resolution_clock::now();
				mNetworkTime = mNetworkFinish - mNetworkStart;

				//Convert timings to miliseconds for frequency calculations
				float targetMiliseconds = static_cast<float>(1000 / mTargetNetworkingFrequency);
				float networkTimeMiliseconds = static_cast<float>(mNetworkTime.count() / pow(10, 6));

				//If networking task took longer than target frequency, create new task
				//Else wait until the time meets the target frequency
				if (networkTimeMiliseconds >= targetMiliseconds)
				{
					//Calculate the actual networking frequency
					mNetworkingFrequency = static_cast<int>(1000 / networkTimeMiliseconds);

					// Cleanup then create new task and set start time
					mNetworkingTask->CleanUpTask();
					mNetworkingTask = mThreadManager->AddTask(std::bind(&ISystem::Process, system), nullptr, nullptr, std::vector<int>{1});
					mNetworkStart = std::chrono::high_resolution_clock::now();
				}
			}
		}
		else
		{
			//Create network task
			mNetworkingTask = mThreadManager->AddTask(std::bind(&ISystem::Process, system), nullptr, nullptr, std::vector<int>{1});
			mNetworkStart = std::chrono::high_resolution_clock::now();
		}
	}
}

/// <summary>
/// Adds an AI component to the entity with a given ID
/// </summary>
/// <param name="pAI">AI component to add</param>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::AddAIComp(const AI & pAI, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mAIs[pEntityID] = pAI;
	entity->componentMask |= ComponentType::COMPONENT_AI;
	AssignEntity(*entity);
}

/// <summary>
/// Adds an Audio component to the entity with a given ID
/// </summary>
/// <param name="pAudio">Audio component to add</param>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::AddAudioComp(const Audio & pAudio, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mAudios[pEntityID] = pAudio;
	entity->componentMask |= ComponentType::COMPONENT_AUDIO;
	AssignEntity(*entity);
}

/// <summary>
/// Adds a Box Collider component to the entity with a given ID
/// </summary>
/// <param name="pBoxCollider">Box Collider component to add</param>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::AddBoxColliderComp(const BoxCollider & pBoxCollider, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mBoxColliders[pEntityID] = pBoxCollider;
	entity->componentMask |= ComponentType::COMPONENT_BOXCOLLIDER;
	AssignEntity(*entity);
}

/// <summary>
/// Adds a Camera component to the entity with a given ID
/// </summary>
/// <param name="pCamera">Camera component to add</param>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::AddCameraComp(const Camera & pCamera, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mCameras[pEntityID] = pCamera;
	entity->componentMask |= ComponentType::COMPONENT_CAMERA;
	AssignEntity(*entity);
}

/// <summary>
/// Adds a Colour component to the entity with a given ID
/// </summary>
/// <param name="pColour">Colour component to add</param>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::AddColourComp(const Colour & pColour, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mColours[pEntityID] = pColour;
	entity->componentMask |= ComponentType::COMPONENT_COLOUR;
	AssignEntity(*entity);
}

/// <summary>
/// Adds a Geometry component to the entity with a given ID
/// </summary>
/// <param name="pGeometry">Geometry component to add</param>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::AddGeometryComp(const Geometry & pGeometry, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mGeometries[pEntityID] = pGeometry;
	entity->componentMask |= ComponentType::COMPONENT_GEOMETRY;
	AssignEntity(*entity);
}

/// <summary>
/// Adds a Light component to the entity with a given ID
/// </summary>
/// <param name="pLight">Light component to add</param>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::AddLightComp(const Light & pLight, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mLights[pEntityID] = pLight;
	entity->componentMask |= ComponentType::COMPONENT_LIGHT;
	AssignEntity(*entity);
}

/// <summary>
/// Adds a Ray component to the entity with a given ID
/// </summary>
/// <param name="pRay">Ray component to add</param>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::AddRayComp(const Ray & pRay, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mRays[pEntityID] = pRay;
	entity->componentMask |= ComponentType::COMPONENT_RAY;
	AssignEntity(*entity);
}

/// <summary>
/// Adds a Shader component to the entity with a given ID
/// </summary>
/// <param name="pShader">Shader component to add</param>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::AddShaderComp(const Shader & pShader, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mShaders[pEntityID] = pShader;
	entity->componentMask |= ComponentType::COMPONENT_SHADER;
	AssignEntity(*entity);
}

/// <summary>
/// Adds a Sphere Collider component to the entity with a given ID
/// </summary>
/// <param name="pSphereCollider">Sphere Collider component to add</param>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::AddSphereColliderComp(const SphereCollider & pSphereCollider, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mSphereColliders[pEntityID] = pSphereCollider;
	entity->componentMask |= ComponentType::COMPONENT_SPHERECOLLIDER;
	AssignEntity(*entity);
}

/// <summary>
/// Adds a Texture component to the entity with a given ID
/// </summary>
/// <param name="pTexture">Texture component to add</param>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::AddTextureComp(const Texture & pTexture, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mTextures[pEntityID] = pTexture;
	entity->componentMask |= ComponentType::COMPONENT_TEXTURE;
	AssignEntity(*entity);
}

/// <summary>
/// Adds a Transform component to the entity with a given ID
/// </summary>
/// <param name="pTransform">Transform component to add</param>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::AddTransformComp(const Transform & pTransform, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mTransforms[pEntityID] = pTransform;
	entity->componentMask |= ComponentType::COMPONENT_TRANSFORM;
	AssignEntity(*entity);
}

/// <summary>
/// Adds a Velocity component to the entity with a given ID
/// </summary>
/// <param name="pVelocity">Velocity component to add</param>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::AddVelocityComp(const Velocity & pVelocity, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mVelocities[pEntityID] = pVelocity;
	entity->componentMask |= ComponentType::COMPONENT_VELOCITY;
	AssignEntity(*entity);
}

/// <summary>
/// Adds a Weight component to the entity with a given ID
/// </summary>
/// <param name="pVelocity">Weight component to add</param>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::AddWeightComp(const Weight & pWeight, const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	mWeights[pEntityID] = pWeight;
	entity->componentMask |= ComponentType::COMPONENT_WEIGHT;
	AssignEntity(*entity);
}

/// <summary>
/// Removes an AI component from the entity with a given ID
/// </summary>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::RemoveAIComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->componentMask = entity->componentMask &= ~ComponentType::COMPONENT_AI; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}

/// <summary>
/// Removes an Audio component from the entity with a given ID
/// </summary>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::RemoveAudioComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->componentMask = entity->componentMask &= ~ComponentType::COMPONENT_AUDIO; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}

/// <summary>
/// Removes a BoxCollider component from the entity with a given ID
/// </summary>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::RemoveBoxColliderComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->componentMask = entity->componentMask &= ~ComponentType::COMPONENT_BOXCOLLIDER; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}


/// <summary>
/// Removes a Camera component from the entity with a given ID
/// </summary>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::RemoveCameraComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->componentMask = entity->componentMask &= ~ComponentType::COMPONENT_CAMERA; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}

/// <summary>
/// Removes a Colour component from the entity with a given ID
/// </summary>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::RemoveColourComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->componentMask = entity->componentMask &= ~ComponentType::COMPONENT_COLOUR; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}


/// <summary>
/// Removes a Geometry component from the entity with a given ID
/// </summary>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::RemoveGeometryComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->componentMask = entity->componentMask &= ~ComponentType::COMPONENT_GEOMETRY; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}


/// <summary>
/// Removes a Light component from the entity with a given ID
/// </summary>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::RemoveLightComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->componentMask = entity->componentMask &= ~ComponentType::COMPONENT_LIGHT; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}

/// <summary>
/// Removes a Ray component from the entity with a given ID
/// </summary>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::RemoveRayComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->componentMask = entity->componentMask &= ~ComponentType::COMPONENT_RAY; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}


/// <summary>
/// Removes a Shader component from the entity with a given ID
/// </summary>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::RemoveShaderComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->componentMask = entity->componentMask &= ~ComponentType::COMPONENT_SHADER; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}

/// <summary>
/// Removes a Sphere Collider component from the entity with a given ID
/// </summary>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::RemoveSphereColliderComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->componentMask = entity->componentMask &= ~ComponentType::COMPONENT_SPHERECOLLIDER; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}

/// <summary>
/// Removes a Texture component from the entity with a given ID
/// </summary>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::RemoveTextureComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->componentMask = entity->componentMask &= ~ComponentType::COMPONENT_TEXTURE; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}


/// <summary>
/// Removes a Transform component from the entity with a given ID
/// </summary>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::RemoveTransformComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->componentMask = entity->componentMask &= ~ComponentType::COMPONENT_TRANSFORM; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}

/// <summary>
/// Removes a Velocity component from the entity with a given ID
/// </summary>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::RemoveVelocityComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->componentMask = entity->componentMask &= ~ComponentType::COMPONENT_VELOCITY; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}

/// <summary>
/// Removes a Weight component from the entity with a given ID
/// </summary>
/// <param name="pEntityID">Given ID of the entity</param>
void ECSManager::RemoveWeightComp(const int pEntityID)
{
	Entity* entity = &mEntities[pEntityID];
	entity->componentMask = entity->componentMask &= ~ComponentType::COMPONENT_WEIGHT; //Performs a bitwise & between the entities mask and the bitwise complement of the components mask
	ReAssignEntity(*entity);
}

/// <summary>
/// Returns a modifiable handle to the AI component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to AI component</returns>
AI * const ECSManager::AIComp(const int pEntityID)
{
	//Checks if entity actually owns a component of this type
	if ((mEntities[pEntityID].componentMask & ComponentType::COMPONENT_AI) == ComponentType::COMPONENT_AI)
	{
		return &mAIs[pEntityID];
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Audio component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to Audio component</returns>
Audio * const ECSManager::AudioComp(const int pEntityID)
{
	//Checks if entity actually owns a component of this type
	if ((mEntities[pEntityID].componentMask & ComponentType::COMPONENT_AUDIO) == ComponentType::COMPONENT_AUDIO)
	{
		return &mAudios[pEntityID];
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the BoxCollider component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to BoxCollider component</returns>
BoxCollider * const ECSManager::BoxColliderComp(const int pEntityID)
{
	//Checks if entity actually owns a component of this type
	if ((mEntities[pEntityID].componentMask & ComponentType::COMPONENT_BOXCOLLIDER) == ComponentType::COMPONENT_BOXCOLLIDER)
	{
		return &mBoxColliders[pEntityID];
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Camera component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to Camera component</returns>
Camera * const ECSManager::CameraComp(const int pEntityID)
{
	//Checks if entity actually owns a component of this type
	if ((mEntities[pEntityID].componentMask & ComponentType::COMPONENT_CAMERA) == ComponentType::COMPONENT_CAMERA)
	{
		return &mCameras[pEntityID];
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Colour component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to Colour component</returns>
Colour * const ECSManager::ColourComp(const int pEntityID)
{
	//Checks if entity actually owns a component of this type
	if ((mEntities[pEntityID].componentMask & ComponentType::COMPONENT_COLOUR) == ComponentType::COMPONENT_COLOUR)
	{
		return &mColours[pEntityID];
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Geometry component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to Geometry component</returns>
Geometry * const ECSManager::GeometryComp(const int pEntityID)
{
	//Checks if entity actually owns a component of this type
	if ((mEntities[pEntityID].componentMask & ComponentType::COMPONENT_GEOMETRY) == ComponentType::COMPONENT_GEOMETRY)
	{
		return &mGeometries[pEntityID];
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Gravity component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to Geometry component</returns>
Gravity * const ECSManager::GravityComp(const int pEntityID)
{
	//Checks if entity actually owns a component of this type
	if ((mEntities[pEntityID].componentMask & ComponentType::COMPONENT_GRAVITY) == ComponentType::COMPONENT_GRAVITY)
	{
		return &mGravities[pEntityID];
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Light component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to AI component</returns>
Light * const ECSManager::LightComp(const int pEntityID)
{
	//Checks if entity actually owns a component of this type
	if ((mEntities[pEntityID].componentMask & ComponentType::COMPONENT_LIGHT) == ComponentType::COMPONENT_LIGHT)
	{
		return &mLights[pEntityID];
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Ray component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to Ray component</returns>
Ray * const ECSManager::RayComp(const int pEntityID)
{
	//Checks if entity actually owns a component of this type
	if ((mEntities[pEntityID].componentMask & ComponentType::COMPONENT_RAY) == ComponentType::COMPONENT_RAY)
	{
		return &mRays[pEntityID];
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Shader component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to Shader component</returns>
Shader * const ECSManager::ShaderComp(const int pEntityID)
{
	//Checks if entity actually owns a component of this type
	if ((mEntities[pEntityID].componentMask & ComponentType::COMPONENT_SHADER) == ComponentType::COMPONENT_SHADER)
	{
		return &mShaders[pEntityID];
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Sphere Collider component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to Sphere Collider component</returns>
SphereCollider * const ECSManager::SphereColliderComp(const int pEntityID)
{
	//Checks if entity actually owns a component of this type
	if ((mEntities[pEntityID].componentMask & ComponentType::COMPONENT_SPHERECOLLIDER) == ComponentType::COMPONENT_SPHERECOLLIDER)
	{
		return &mSphereColliders[pEntityID];
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Texture component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to Texture component</returns>
Texture * const ECSManager::TextureComp(const int pEntityID)
{
	//Checks if entity actually owns a component of this type
	if ((mEntities[pEntityID].componentMask & ComponentType::COMPONENT_TEXTURE) == ComponentType::COMPONENT_TEXTURE)
	{
		return &mTextures[pEntityID];
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Transform component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to Transform component</returns>
Transform * const ECSManager::TransformComp(const int pEntityID)
{
	//Checks if entity actually owns a component of this type
	if ((mEntities[pEntityID].componentMask & ComponentType::COMPONENT_TRANSFORM) == ComponentType::COMPONENT_TRANSFORM)
	{
		return &mTransforms[pEntityID];
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Velocity component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to Velocity component</returns>
Velocity * const ECSManager::VelocityComp(const int pEntityID)
{
	//Checks if entity actually owns a component of this type
	if ((mEntities[pEntityID].componentMask & ComponentType::COMPONENT_VELOCITY) == ComponentType::COMPONENT_VELOCITY)
	{
		return &mVelocities[pEntityID];
	}
	return nullptr;
}

/// <summary>
/// Returns a modifiable handle to the Weight component associated with the given entity ID
/// </summary>
/// <param name="pEntityID">Given entity ID</param>
/// <returns>Modifiable handle to Weight component</returns>
Weight * const ECSManager::WeightComp(const int pEntityID)
{
	//Checks if entity actually owns a component of this type
	if ((mEntities[pEntityID].componentMask & ComponentType::COMPONENT_WEIGHT) == ComponentType::COMPONENT_WEIGHT)
	{
		return &mWeights[pEntityID];
	}
	return nullptr;
}
