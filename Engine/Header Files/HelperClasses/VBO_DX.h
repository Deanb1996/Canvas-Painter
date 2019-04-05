#pragma once
#include "VBO.h"
#include <d3d11.h>
#include "ObjLoader.h"

class RenderSystem_DX;

class VBO_DX :
	public VBO
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> mVertices{};
	Microsoft::WRL::ComPtr<ID3D11Buffer> mIndices{};

public:
	VBO_DX();
	~VBO_DX();

	HRESULT Create(const RenderSystem* pRenderer, const std::wstring& pFilename) override;
	void Load(const RenderSystem* pRenderer) override;
	void Draw(const RenderSystem* pRenderer) const override;
};

