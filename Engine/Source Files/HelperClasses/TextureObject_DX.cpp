#pragma once
#include "TextureObject_DX.h"
#include "RenderSystem_DX.h"

/// <summary>
/// Default constructor
/// </summary>
TextureObject_DX::TextureObject_DX()
{
}

/// <summary>
/// Default destructor
/// </summary>
TextureObject_DX::~TextureObject_DX()
{
}

/// <summary>
/// Loads a texture from a dds file and stores it in the texture object
/// </summary>
/// <param name="pRenderer">Renderer to get the DirectX device from</param>
/// <param name="pFilename">File name of the texture</param>
/// <returns>HRESULT status code</returns>
HRESULT TextureObject_DX::Create(const RenderSystem* pRenderer, const std::wstring& pFilename)
{
	HRESULT hr = S_OK;

	if (!pFilename.empty())
	{
		//Create shader resource for diffuse texture from file and store pointer to it
		hr = DirectX::CreateDDSTextureFromFile(reinterpret_cast<const RenderSystem_DX*>(pRenderer)->Device().Get(), pFilename.c_str(), nullptr, mTexture.GetAddressOf());
		if (FAILED(hr))
		{
			return hr;
		}
	}

	return hr;
}

/// <summary>
/// Loads the texture contained in the texture object into the shader resource of the render system
/// </summary>
/// <param name="pRenderer">Render system to assign the shader resource to</param>
void TextureObject_DX::Load(const RenderSystem * pRenderer, const int& pSlot) const
{
	reinterpret_cast<const RenderSystem_DX*>(pRenderer)->Context()->PSSetShaderResources(pSlot, 1, mTexture.GetAddressOf());
}
