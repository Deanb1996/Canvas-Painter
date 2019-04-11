#pragma once
#include "ShaderObject_GL.h"
#include "RenderSystem_GL.h"

/// <summary>
/// Default constructor
/// </summary>
ShaderObject_GL::ShaderObject_GL()
{
}

/// <summary>
/// Default destructor
/// </summary>
ShaderObject_GL::~ShaderObject_GL()
{
}

/// <summary>
/// Creates a vertex shader and stores it in the shader object
/// </summary>
/// <param name="pFilename">File name of the vertex shader</param>
/// <param name="pEntryPoint">Entry point of the vertex shader</param>
/// <param name="pShaderModel">Shader model of the vertex shader</param>
/// <returns>HRESULT status code</returns>
HRESULT ShaderObject_GL::CreateVertex(const RenderSystem* pRenderer, const std::wstring & pFilename, const std::string& pEntryPoint, const std::string& pShaderModel)
{
	HRESULT hr = S_OK;

	return hr;
}

/// <summary>
/// Creates a pixel shader and stores it in the shader object
/// </summary>
/// <param name="pFilename">File name of the pixel shader</param>
/// <param name="pEntryPoint">Entry point of the pixel shader</param>
/// <param name="pShaderModel">Shader model of the pixel shader</param>
/// <returns>HRESULT status code</returns>
HRESULT ShaderObject_GL::CreatePixel(const RenderSystem* pRenderer, const std::wstring & pFilename, const std::string & pEntryPoint, const std::string & pShaderModel)
{
	HRESULT hr = S_OK;

	return hr;
}

/// <summary>
/// Loads the shaders contained in the shader object into the buffers of the render system
/// </summary>
/// <param name="pRenderer">Render system to assign the buffers to</param>
void ShaderObject_GL::Load(const RenderSystem * pRenderer) const
{
}

