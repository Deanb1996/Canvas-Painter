#include "RenderSystem_GL.h"

//NOT YET IMPLEMENTED:
//Blend states
//Depth states
//Cull states
//Texture loading
//Instance rendering


/// <summary>
/// Constructor
/// Sets component mask to contain a transform component, a geometry and a shader component
/// Initialises directX device, and cleans up directX resources if failed
/// </summary>
/// <param name="pWindow">A handle to the win32 window</param>
RenderSystem_GL::RenderSystem_GL(const HWND& pWindow) : RenderSystem(ComponentType::COMPONENT_TRANSFORM | ComponentType::COMPONENT_GEOMETRY | ComponentType::COMPONENT_SHADER | ComponentType::COMPONENT_COLOUR),
mWindow(pWindow), mActiveCamera(nullptr)
{
	if (FAILED(Init()))
	{
		Cleanup();
	}
}

/// <summary>
/// Default destructor
/// </summary>
RenderSystem_GL::~RenderSystem_GL()
{

}

/// <summary>
/// Initialises directx device and resources
/// </summary>
/// <returns>HRESULT status code</returns>
HRESULT RenderSystem_GL::Init()
{
	auto hr{ S_OK };

	RECT rc;
	GetClientRect(mWindow, &rc);
	mWidth = rc.right - rc.left;
	height = rc.bottom - rc.top;

	hr = CreateDevice();
	if (FAILED(hr))
		return hr;

	//mAntTweakManager->Init(TW_DIRECT3D11, mDevice.Get(), mWidth, height);

	hr = CreateSwapChain();
	if (FAILED(hr))
		return hr;

	hr = CreateRenderTarget();
	if (FAILED(hr))
		return hr;

	hr = CreateDepth();
	if (FAILED(hr))
		return hr;

	hr = CreateRasterizer();
	if (FAILED(hr))
		return hr;

	hr = CreateSampler();
	if (FAILED(hr))
		return hr;

	hr = CreateConstantBuffers();
	if (FAILED(hr))
		return hr;

	CreateViewport();

	//Set primitive topology
	//mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return hr;
}

/// <summary>
/// Creates directx device
/// </summary>
/// <returns>HRESULT status code</returns>
HRESULT RenderSystem_GL::CreateDevice()
{
	auto hr{ S_OK };

	return hr;
}

/// <summary>
/// Creates swap chain for directx 
/// </summary>
/// <returns>HRESULT status code</returns>
HRESULT RenderSystem_GL::CreateSwapChain()
{
	auto hr{ S_OK };

	return  hr;
}

/// <summary>
/// Creates render target for directx
/// </summary>
/// <returns>HRESULT status code</returns>
HRESULT RenderSystem_GL::CreateRenderTarget()
{
	auto hr{ S_OK };

	return hr;
}

/// <summary>
/// Creates depth stencil for diretx
/// </summary>
/// <returns>HRESULT status code</returns>
HRESULT RenderSystem_GL::CreateDepth()
{
	auto hr{ S_OK };

	return hr;
}

/// <summary>
/// Creates rasteriser states
/// </summary>
/// <returns>HRESULT status code</returns>
HRESULT RenderSystem_GL::CreateRasterizer()
{
	auto hr{ S_OK };

	return hr;
}

/// <summary>
/// Creates blend states
/// </summary>
/// <returns>HRESULT status code</returns>
HRESULT RenderSystem_GL::CreateBlend()
{
	return E_NOTIMPL;
}

/// <summary>
/// Creates sampler state
/// </summary>
/// <returns>HRESULT status code</returns>
HRESULT RenderSystem_GL::CreateSampler()
{
	auto hr{ S_OK };

	return hr;
}

/// <summary>
/// Creates viewport
/// </summary>
void RenderSystem_GL::CreateViewport() const
{
}

/// <summary>
/// Creates constant buffers
/// </summary>
/// <returns>HRESULT status code</returns>
HRESULT RenderSystem_GL::CreateConstantBuffers()
{
	auto hr = S_OK;

	return hr;
}

/// <summary>
/// Cleans up system resources used by directx   IS THIS EVEN NEEDED???
/// </summary>
void RenderSystem_GL::Cleanup()
{
	mAntTweakManager->Cleanup();
}

/// <summary>
/// Assigns entity to system if the entities mask matches the system mask
/// </summary>
/// <param name="pEntity">Entity to be assigned</param>
void RenderSystem_GL::AssignEntity(const Entity & pEntity)
{
	//Checks if entity mask matches the renderable mask
	if ((pEntity.componentMask & mMask) == mMask)
	{
		//Update entry in systems entity list
		mEntities[pEntity.ID] = pEntity;
	}

	//Checks if entity mask matches the light mask
	if ((pEntity.componentMask & ComponentType::COMPONENT_LIGHT) == ComponentType::COMPONENT_LIGHT)
	{
		//If the entity has a light component then find it in the lights
		const auto entity = find_if(mLights.begin(), mLights.end(), [&](const Entity& entity) {return entity.ID == pEntity.ID; });
		if (entity == mLights.end())
		{
			//If not found then add it
			mLights.push_back(pEntity);
		}
		else
		{
			//If already in the list, then update mask
			entity->componentMask = pEntity.componentMask;
		}
	}

	//If the entity is marked as a camera set it to the active camera
	//TODO: Implement multiple cameras
	if ((pEntity.componentMask & ComponentType::COMPONENT_CAMERA) == ComponentType::COMPONENT_CAMERA)
	{
		mActiveCamera = &pEntity;

		//if already in the list, then update mask
		//mActiveCamera->componentMask = pEntity.componentMask;
	}
}

/// <summary>
/// Re-assigns entity to system when component is removed from entity
/// </summary>
/// <param name="pEntity">Entity to re-assign</param>
void RenderSystem_GL::ReAssignEntity(const Entity & pEntity)
{
	//Checks if entity mask matches the renderable mask
	if ((pEntity.componentMask & mMask) == mMask)
	{
		//If the entity matches renderable mask then update entry in systems entity list
		mEntities[pEntity.ID] = pEntity;
	}
	else
	{
		//If the mask doesn't match then set ID to -1
		mEntities[pEntity.ID].ID = -1;
	}

	//Checks if entity mask matches the light mask
	if ((pEntity.componentMask & ComponentType::COMPONENT_LIGHT) == ComponentType::COMPONENT_LIGHT)
	{
		//If the entity has a light component then find it in the lights
		const auto entity = find_if(mLights.begin(), mLights.end(), [&](const Entity& entity) {return entity.ID == pEntity.ID; });
		if (entity == mLights.end())
		{
			//If not found then add it
			mLights.push_back(pEntity);
		}
		else
		{
			//If already in the list, then update mask
			entity->componentMask = pEntity.componentMask;
		}
	}
	else
	{
		//If the mask doesn't match then remove it (if it wasn't in the list then the remove acts as a search to confirm it is not there)
		mLights.erase(remove_if(mLights.begin(), mLights.end(), [&](const Entity& entity) {return entity.ID == pEntity.ID; }), mLights.end());
	}

	//TODO: Implement multiple cameras
}

/// <summary>
/// Systems process function, core logic of system
/// Renders every renderable entity, as well as updating all lighting and camera data
/// </summary>
void RenderSystem_GL::Process()
{
	ClearView();

	if (mActiveCamera)
	{
		SetViewProj();
	}
	if (mLights.size() > 0)
	{
		SetLights();
	}
	for (const Entity& entity : mEntities)
	{
		if (entity.ID != -1)
		{
			//If geometry of entity is not already in the buffers, load entities geometry
			if (mEcsManager->GeometryComp(entity.ID)->filename != mActiveGeometry)
			{
				mGeometry = LoadGeometry(entity);
				mGeometry->Load(this);
				mActiveGeometry = mEcsManager->GeometryComp(entity.ID)->filename;
			}
			//LoadTexture(entity);
			//If shader of entity is not already in the buffers, load entities shader
			if (mEcsManager->ShaderComp(entity.ID)->filename != mActiveShader)
			{
				LoadShaders(entity);
				mActiveShader = mEcsManager->ShaderComp(entity.ID)->filename;
			}

			//Update constant buffer with world matrix and object colour
			//mCB.mWorld = XMFLOAT4X4(reinterpret_cast<float*>(&(mEcsManager->TransformComp(entity.ID)->transform)));
			//mCB.colour = XMFLOAT4(reinterpret_cast<float*>(&(mEcsManager->ColourComp(entity.ID)->colour)));
			//mContext->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, &mCB, 0, 0);

			//mContext->OMSetBlendState()
			//mContext->OMSetDepthStencilState(NULL, 1);
			//mContext->RSSetState(mDefaultRasterizerState.Get());

			mGeometry->Draw(this);
		}
	}

	mAntTweakManager->Draw();

	SwapBuffers();
}

/// <summary>
/// Clears screen to cornflower blue background
/// </summary>
void RenderSystem_GL::ClearView() const
{

}

/// <summary>
/// Swaps buffers in swap chain
/// </summary>
void RenderSystem_GL::SwapBuffers() const
{
	//mSwapChain->Present(1, 0);
}

/// <summary>
/// Loads the geometry for the given entity into a VBO object 
/// </summary>
/// <param name="pEntity">Entity to load geometry for</param>
VBO * const RenderSystem_GL::LoadGeometry(const Entity& pEntity) const
{
	const auto geometry = mResourceManager->LoadGeometry(this, mEcsManager->GeometryComp(pEntity.ID)->filename);
	return geometry;
}

/// <summary>
/// Loads the shader for the given entity into a shader object
/// </summary>
/// <param name="pEntity">Entity to load shader for</param>
void RenderSystem_GL::LoadShaders(const Entity & pEntity) const
{
	const auto shader = mResourceManager->LoadShader(this, mEcsManager->ShaderComp(pEntity.ID)->filename);
	shader->Load(this);
}

/// <summary>
/// Loads the texture for the given entity into a texture object
/// </summary>
/// <param name="pEntity">Entity to load texture for</param>
void RenderSystem_GL::LoadTexture(const Entity & pEntity) const
{
	//Loads diffuse texture from texture component
	auto texture = mResourceManager->LoadTexture(this, mEcsManager->TextureComp(pEntity.ID)->diffuse);
	if (texture)
	{
		texture->Load(this, 0);
	}

	//Loads normal map texture from texture component
	texture = mResourceManager->LoadTexture(this, mEcsManager->TextureComp(pEntity.ID)->normal);
	if (texture)
	{
		texture->Load(this, 1);
	}

	//Loads height map texture from texture component
	texture = mResourceManager->LoadTexture(this, mEcsManager->TextureComp(pEntity.ID)->height);
	if (texture)
	{
		texture->Load(this, 2);
	}
}

/// <summary>
/// Sets the view and projection matrices in the constant buffer
/// </summary>
void RenderSystem_GL::SetViewProj()
{
}

/// <summary>
/// Sets the light position and colour in the constant buffer
/// </summary>
void RenderSystem_GL::SetLights()
{
}

