#include "VBO_DX.h"
#include "RenderSystem_DX.h"

/// <summary>
/// Default constructor
/// </summary>
VBO_DX::VBO_DX() : VBO() 
{

}

/// <summary>
/// Default destructor
/// </summary>
VBO_DX::~VBO_DX()
{

}

/// <summary>
/// Creates the vertex and index buffers by loading in the obj file given a filename
/// </summary>
/// <param name="pRenderer"> the renderer to use to create the buffers with </param>
/// <param name="pFilename"> the filename of the geometry to load </param>
HRESULT VBO_DX::Create(const RenderSystem * pRenderer, const std::wstring& pFilename)
{
	auto hr{ S_OK };

	auto geometry = ObjLoader::LoadObject(pFilename);
	mIndexCount = static_cast<int>(geometry.first.size());

	//Create vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = static_cast<UINT>(geometry.first.size() * sizeof(Vertex));
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = &(geometry.second[0]);
	hr = reinterpret_cast<const RenderSystem_DX*>(pRenderer)->Device()->CreateBuffer(&bd, &initData, mVertices.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	//Create index buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = static_cast<UINT>(geometry.first.size() * sizeof(unsigned));
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	initData.pSysMem = &(geometry.first[0]);
	hr = reinterpret_cast<const RenderSystem_DX*>(pRenderer)->Device()->CreateBuffer(&bd, &initData, mIndices.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

/// <summary>
/// Loads the vertices and indices of the geometry into vertex and index buffers of the given directX device
/// </summary>
/// <param name="pRenderer">Renderer to retrieve the device from</param>
void VBO_DX::Load(const RenderSystem* pRenderer)
{
	// Set vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	reinterpret_cast<const RenderSystem_DX*>(pRenderer)->Context()->IASetVertexBuffers(0, 1, mVertices.GetAddressOf(), &stride, &offset);

	// Set index buffer
	reinterpret_cast<const RenderSystem_DX*>(pRenderer)->Context()->IASetIndexBuffer(mIndices.Get(), DXGI_FORMAT_R32_UINT, 0);
}

/// <summary>
/// Draws the vertices and indices in the VBO with the given directX device
/// </summary>
/// <param name="pRenderer">Renderer to retrieve the directX device from</param>
void VBO_DX::Draw(const RenderSystem * pRenderer) const
{
	reinterpret_cast<const RenderSystem_DX*>(pRenderer)->Context()->DrawIndexed(mIndexCount, 0, 0);
}
