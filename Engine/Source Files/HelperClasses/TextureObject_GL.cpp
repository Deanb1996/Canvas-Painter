#pragma once
#include "TextureObject_GL.h"
#include "RenderSystem_GL.h"

/// <summary>
/// Default constructor
/// </summary>
TextureObject_GL::TextureObject_GL()
{
}

/// <summary>
/// Default destructor
/// </summary>
TextureObject_GL::~TextureObject_GL()
{
}

/// <summary>
/// Loads a texture from a dds file and stores it in the texture object
/// </summary>
/// <param name="pRenderer">Renderer to get the DirectX device from</param>
/// <param name="pFilename">File name of the texture</param>
/// <returns>HRESULT status code</returns>
HRESULT TextureObject_GL::Create(const RenderSystem* pRenderer, const std::wstring& pFilename)
{
	HRESULT hr = S_OK;

	return hr;
}

/// <summary>
/// Loads the texture contained in the texture object into the shader resource of the render system
/// </summary>
/// <param name="pRenderer">Render system to assign the shader resource to</param>
void TextureObject_GL::Load(const RenderSystem * pRenderer, const int& pSlot) const
{
}
