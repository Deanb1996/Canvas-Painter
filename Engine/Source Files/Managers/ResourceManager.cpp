#include "ResourceManager.h"


using namespace std;

/// <summary>
/// Default constructor
/// </summary>
ResourceManager::ResourceManager() 
{
}

/// <summary>
/// Default destructor
/// </summary>
ResourceManager::~ResourceManager() 
{
}

/// <summary>
/// If texture is not already loaded, loads and stores texture file into texture object
/// If texture is already loaded, retrieves the already created texture object
/// </summary>
/// <param name="pRenderer">The render system used for creation of texture object</param>
/// <param name="pFilename">Filename of the texture</param>
/// <returns>Handle to the texture object associated with the file name</returns>
const TextureObject * const ResourceManager::LoadTexture(const RenderSystem * const pRenderer, const std::wstring & pFilename)
{
	auto hr{ S_OK };
	//find and return from map
	for (const auto& pair : mTextures)
	{
		if (pair.first == pFilename)
		{
			return pair.second;
		}
	}
	//else create a new texture

#ifdef  DIRECTX
	TextureObject* newTexture = new TextureObject_DX();
#elif OPENGL
	TextureObject* newTexture = new TextureObject_GL();
#endif
	hr = newTexture->Create(pRenderer, pFilename);

	if (FAILED(hr))
	{
		return nullptr;
	}

	//add it to the map
	mTextures.emplace_back(make_pair(pFilename, newTexture));
	//return the last thing in the dictionary (the newly created texture)
	return mTextures.back().second;
}

/// <summary>
/// If geometry is not already loaded, loads and stores geometry file into geometry object
/// If geometry is already loaded, retrieves the already created geometry object
/// </summary>
/// <param name="pRenderer">The render system used for the creation of the geometry object</param>
/// <param name="pFilename">Filename of the geometry</param>
/// <returns>Handle to the VBO associated with the file name</returns>
VBO * const ResourceManager::LoadGeometry(const RenderSystem * const pRenderer, const std::wstring& pFilename)
{
	auto hr{ S_OK };
	//find and return from map
	for (const auto& pair : mGeometries)
	{
		if (pair.first == pFilename)
		{
			return pair.second;
		}
	}
	//else create a new geometry
#ifdef  DIRECTX
	VBO* newGeometry = new VBO_DX();
#elif OPENGL
	VBO* newGeometry = new VBO_GL();
#endif
	hr = newGeometry->Create(pRenderer, pFilename);
	if (FAILED(hr))
	{
		return nullptr;
	}
	//add it to the map
	mGeometries.emplace_back(make_pair(pFilename, newGeometry));
	//return the last thing in the dictionary (the newly created geometry)
	return mGeometries.back().second;
}

/// <summary>
/// If shader is not already loaded, loads and stores shader file into shader object
/// If shader is already loaded, retrieves the already created shader object
/// </summary>
/// <param name="pRenderer">The render system used for the creation of the shader object</param>
/// <param name="pFilename">Filename of the shader</param>
/// <returns>Handle to the shader object associated with the file name</returns>
const ShaderObject * const ResourceManager::LoadShader(const RenderSystem * const pRenderer, const std::wstring & pFilename)
{
	auto hr{ S_OK };
	//find and return from map
	for (const auto& pair : mShaders)
	{
		if (pair.first == pFilename)
		{
			return pair.second;
		}
	}
	//else create a new shader
#ifdef  DIRECTX
	ShaderObject* newShader =  new ShaderObject_DX();
#elif OPENGL
	ShaderObject* newShader = new ShaderObject_GL();
#endif
	hr = newShader->CreateVertex(pRenderer, pFilename, "VS", "vs_5_0");
	if (FAILED(hr))
	{
		return nullptr;
	}
	hr = newShader->CreatePixel(pRenderer, pFilename, "PS", "ps_5_0");
	if (FAILED(hr))
	{
		return nullptr;
	}
	//add it to the map
	mShaders.emplace_back(make_pair(pFilename, newShader));
	//return the last thing in the dictionary (the newly created shader)
	return mShaders.back().second;
}

/// <summary>
/// Creates a singleton instance of Resource Manager if one hasn't been created before
/// Returns pointer to the instance of Resource Manager
/// </summary>
/// <returns>Shared pointer to the Resource Manager instance</returns>
std::shared_ptr<ResourceManager> ResourceManager::Instance()
{
	static shared_ptr<ResourceManager> instance{ new ResourceManager };
	return instance;
}
