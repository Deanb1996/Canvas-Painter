#include "VBO_GL.h"
#include "RenderSystem_GL.h"

/// <summary>
/// Default constructor
/// </summary>
VBO_GL::VBO_GL() : VBO()
{

}

/// <summary>
/// Default destructor
/// </summary>
VBO_GL::~VBO_GL()
{

}

/// <summary>
/// Creates the vertex and index buffers by loading in the obj file given a filename
/// </summary>
/// <param name="pRenderer"> the renderer to use to create the buffers with </param>
/// <param name="pFilename"> the filename of the geometry to load </param>
HRESULT VBO_GL::Create(const RenderSystem * pRenderer, const std::wstring& pFilename)
{
	auto hr{ S_OK };

	return hr;
}

/// <summary>
/// Loads the vertices and indices of the geometry into vertex and index buffers of the given directX device
/// </summary>
/// <param name="pRenderer">Renderer to retrieve the device from</param>
void VBO_GL::Load(const RenderSystem* pRenderer)
{
}

/// <summary>
/// Draws the vertices and indices in the VBO with the given directX device
/// </summary>
/// <param name="pRenderer">Renderer to retrieve the directX device from</param>
void VBO_GL::Draw(const RenderSystem * pRenderer) const
{
}
