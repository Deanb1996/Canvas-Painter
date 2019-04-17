#include "RenderSystem_DX.h"

using namespace DirectX;

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
RenderSystem_DX::RenderSystem_DX(const HWND& pWindow) : RenderSystem(ComponentType::COMPONENT_TRANSFORM | ComponentType::COMPONENT_GEOMETRY | ComponentType::COMPONENT_SHADER | ComponentType::COMPONENT_COLOUR),
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
RenderSystem_DX::~RenderSystem_DX()
{

}

/// <summary>
/// Initialises directx device and resources
/// </summary>
/// <returns>HRESULT status code</returns>
HRESULT RenderSystem_DX::Init()
{
	auto hr{ S_OK };

	RECT rc;
	GetClientRect(mWindow, &rc);
	mWidth = rc.right - rc.left;
	height = rc.bottom - rc.top;

	hr = CreateDevice();
	if (FAILED(hr))
		return hr;

	mAntTweakManager->Init(TW_DIRECT3D11, mDevice.Get(), mWidth, height);

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
	mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//improved object reporting for finding memory leaks
	//#ifdef _DEBUG
	//	Microsoft::WRL::ComPtr<ID3D11Debug> DebugDevice = nullptr;
	//	hr = mDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(DebugDevice.GetAddressOf()));
	//
	//	hr = DebugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	//#endif

	return hr;
}

/// <summary>
/// Creates directx device
/// </summary>
/// <returns>HRESULT status code</returns>
HRESULT RenderSystem_DX::CreateDevice()
{
	auto hr{ S_OK };

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	const UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[]
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	const auto numFeatureLevels = ARRAYSIZE(featureLevels);

	for (const auto driverType : driverTypes)
	{
		hr = D3D11CreateDevice(nullptr, driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, mDevice.GetAddressOf(), &mFeatureLevel, mContext.GetAddressOf());

		if (hr == E_INVALIDARG)
		{
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDevice(nullptr, driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, mDevice.GetAddressOf(), &mFeatureLevel, mContext.GetAddressOf());
		}

		if (SUCCEEDED(hr))
			break;
	}

	return hr;
}

/// <summary>
/// Creates swap chain for directx 
/// </summary>
/// <returns>HRESULT status code</returns>
HRESULT RenderSystem_DX::CreateSwapChain()
{
	auto hr{ S_OK };

	Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory = nullptr;
	{
		Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice = nullptr;
		hr = mDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(dxgiDevice.GetAddressOf()));
		if (SUCCEEDED(hr))
		{
			Microsoft::WRL::ComPtr<IDXGIAdapter> adapter = nullptr;
			hr = dxgiDevice->GetAdapter(adapter.GetAddressOf());
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(dxgiFactory.GetAddressOf()));
			}
		}
	}
	if (FAILED(hr))
		return hr;

	// Create swap chain
	Microsoft::WRL::ComPtr<IDXGIFactory2> dxgiFactory2 = nullptr;
	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(dxgiFactory2.GetAddressOf()));
	if (dxgiFactory2)
	{
		// DirectX 11.1 or later
		hr = mDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(mDevice1.GetAddressOf()));
		if (SUCCEEDED(hr))
		{
			mContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(mContext1.GetAddressOf()));
		}

		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Width = mWidth;
		sd.Height = height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		hr = dxgiFactory2->CreateSwapChainForHwnd(mDevice.Get(), mWindow, &sd, nullptr, nullptr, mSwapChain1.GetAddressOf());
		if (SUCCEEDED(hr))
		{
			hr = mSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(mSwapChain.GetAddressOf()));
		}

	}
	else
	{
		// DirectX 11.0 systems
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = mWidth;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = mWindow;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		hr = dxgiFactory->CreateSwapChain(mDevice.Get(), &sd, mSwapChain.GetAddressOf());
	}

	dxgiFactory->MakeWindowAssociation(mWindow, DXGI_MWA_NO_ALT_ENTER);

	if (FAILED(hr))
		return hr;

	return  hr;
}

/// <summary>
/// Creates render target for directx
/// </summary>
/// <returns>HRESULT status code</returns>
HRESULT RenderSystem_DX::CreateRenderTarget()
{
	auto hr{ S_OK };

	// Create a render target view
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	if (FAILED(hr))
		return hr;

	hr = mDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, mRenderTargetView.GetAddressOf());
	if (FAILED(hr))
		return hr;

	return hr;
}

/// <summary>
/// Creates depth stencil for diretx
/// </summary>
/// <returns>HRESULT status code</returns>
HRESULT RenderSystem_DX::CreateDepth()
{
	auto hr{ S_OK };

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(depthDesc));
	depthDesc.Width = mWidth;
	depthDesc.Height = height;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;
	hr = mDevice->CreateTexture2D(&depthDesc, nullptr, mDepthStencil.GetAddressOf());
	if (FAILED(hr))
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = depthDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = mDevice->CreateDepthStencilView(mDepthStencil.Get(), &depthStencilViewDesc, mDepthStencilView.GetAddressOf());
	if (FAILED(hr))
		return hr;

	mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = mDevice->CreateDepthStencilState(&depthStencilDesc, mDepthStencilState.GetAddressOf());
	if (FAILED(hr))
		return hr;

	return hr;
}

/// <summary>
/// Creates rasteriser states
/// </summary>
/// <returns>HRESULT status code</returns>
HRESULT RenderSystem_DX::CreateRasterizer()
{
	auto hr{ S_OK };

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;

	hr = mDevice->CreateRasterizerState(&rasterizerDesc, mDefaultRasterizerState.GetAddressOf());

	return hr;
}

/// <summary>
/// Creates blend states
/// </summary>
/// <returns>HRESULT status code</returns>
HRESULT RenderSystem_DX::CreateBlend()
{
	return E_NOTIMPL;
}

/// <summary>
/// Creates sampler state
/// </summary>
/// <returns>HRESULT status code</returns>
HRESULT RenderSystem_DX::CreateSampler()
{
	auto hr{ S_OK };

	//Set the sampler
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	hr = mDevice->CreateSamplerState(&samplerDesc, mTexSampler.GetAddressOf());

	return hr;
}

/// <summary>
/// Creates viewport
/// </summary>
void RenderSystem_DX::CreateViewport() const
{
	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<float>(mWidth);
	vp.Height = static_cast<float>(height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	mContext->RSSetViewports(1, &vp);
}

/// <summary>
/// Creates constant buffers
/// </summary>
/// <returns>HRESULT status code</returns>
HRESULT RenderSystem_DX::CreateConstantBuffers()
{
	auto hr = S_OK;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(ConstantBuffer);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	hr = mDevice->CreateBuffer(&bufferDesc, nullptr, mConstantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}
	mContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());
	mContext->PSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());

	return hr;
}

/// <summary>
/// Cleans up system resources used by directx   IS THIS EVEN NEEDED???
/// </summary>
void RenderSystem_DX::Cleanup()
{
	mAntTweakManager->Cleanup();
}

/// <summary>
/// Assigns entity to system if the entities mask matches the system mask
/// </summary>
/// <param name="pEntity">Entity to be assigned</param>
void RenderSystem_DX::AssignEntity(const Entity & pEntity)
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
void RenderSystem_DX::ReAssignEntity(const Entity & pEntity)
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
void RenderSystem_DX::Process()
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
				mCB.mWorld = XMFLOAT4X4(reinterpret_cast<float*>(&(mEcsManager->TransformComp(entity.ID)->transform)));
				mCB.colour = XMFLOAT4(reinterpret_cast<float*>(&(mEcsManager->ColourComp(entity.ID)->colour)));
				mContext->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, &mCB, 0, 0);

				//mContext->OMSetBlendState()
				//mContext->OMSetDepthStencilState(NULL, 1);
				mContext->RSSetState(mDefaultRasterizerState.Get());

				mGeometry->Draw(this);
			}
		}

		mAntTweakManager->Draw();

		SwapBuffers();
}

/// <summary>
/// Clears screen to cornflower blue background
/// </summary>
void RenderSystem_DX::ClearView() const
{
	mContext->ClearRenderTargetView(mRenderTargetView.Get(), DirectX::Colors::CornflowerBlue);
	mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

/// <summary>
/// Swaps buffers in swap chain
/// </summary>
void RenderSystem_DX::SwapBuffers() const
{
	mSwapChain->Present(1, 0);
}

/// <summary>
/// Loads the geometry for the given entity into a VBO object 
/// </summary>
/// <param name="pEntity">Entity to load geometry for</param>
VBO * const RenderSystem_DX::LoadGeometry(const Entity& pEntity) const
{
	const auto geometry = mResourceManager->LoadGeometry(this, mEcsManager->GeometryComp(pEntity.ID)->filename);
	return geometry;
}

/// <summary>
/// Loads the shader for the given entity into a shader object
/// </summary>
/// <param name="pEntity">Entity to load shader for</param>
void RenderSystem_DX::LoadShaders(const Entity & pEntity) const
{
	const auto shader = mResourceManager->LoadShader(this, mEcsManager->ShaderComp(pEntity.ID)->filename);
	shader->Load(this);
}

/// <summary>
/// Loads the texture for the given entity into a texture object
/// </summary>
/// <param name="pEntity">Entity to load texture for</param>
void RenderSystem_DX::LoadTexture(const Entity & pEntity) const
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
void RenderSystem_DX::SetViewProj()
{
	//Calculates the view matrix and sets it in the constant buffer
	const XMFLOAT4 position(reinterpret_cast<float*>(&(mEcsManager->TransformComp(mActiveCamera->ID)->translation)));
	mCB.mCameraPosition = XMFLOAT4(reinterpret_cast<float*>(&(mEcsManager->TransformComp(mActiveCamera->ID)->translation)));
	const XMFLOAT4 lookAt(reinterpret_cast<float*>(&(mEcsManager->CameraComp(mActiveCamera->ID)->lookAt)));
	const XMFLOAT4 up(reinterpret_cast<float*>(&(mEcsManager->CameraComp(mActiveCamera->ID)->up)));

	const XMVECTOR posVec = XMLoadFloat4(&position);
	const XMVECTOR lookAtVec = XMLoadFloat4(&lookAt);
	const XMVECTOR upVec = XMLoadFloat4(&up);

	XMStoreFloat4x4(&mCB.mView, XMMatrixTranspose(XMMatrixLookAtLH(posVec, lookAtVec, upVec)));

	//Calculates the projection matrix and sets it in the constant buffer
	const float fov = XMConvertToRadians(mEcsManager->CameraComp(mActiveCamera->ID)->FOV);
	const float aspectRatio = static_cast<float>(mWidth) / static_cast<float>(height);
	const float nearClip = mEcsManager->CameraComp(mActiveCamera->ID)->nearPlane;
	const float farClip = mEcsManager->CameraComp(mActiveCamera->ID)->farPlane;

	XMStoreFloat4x4(&mCB.mProj, XMMatrixTranspose(XMMatrixPerspectiveFovLH(fov, aspectRatio, nearClip, farClip)));
}

/// <summary>
/// Sets the light position and colour in the constant buffer
/// </summary>
void RenderSystem_DX::SetLights()
{
	mCB.mLightPosition = XMFLOAT4(reinterpret_cast<float*>(&(mEcsManager->TransformComp(mLights[0].ID)->translation)));
	mCB.mLightColour = XMFLOAT4(reinterpret_cast<float*>(&(mEcsManager->LightComp(mLights[0].ID)->colour)));
}

