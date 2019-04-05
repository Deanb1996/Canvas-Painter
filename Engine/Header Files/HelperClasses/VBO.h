#pragma once
#include <wrl.h>
#include <string>
#include <vector>
#include <algorithm>

class RenderSystem;

class VBO
{
protected:
	VBO();

	int mIndexCount;
public:
	virtual ~VBO() = default;

	virtual HRESULT Create(const RenderSystem* pRenderer, const std::wstring& pFilename) = 0;
	virtual void Load(const RenderSystem* pRenderer) = 0;
	virtual void Draw(const RenderSystem* pRenderer) const = 0;
};
