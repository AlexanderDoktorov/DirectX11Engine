#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <vector>
#include "BindableSystem.h"
#include "hrException.h"
#include "noxnd.h"

class IndexBuffer : public IBindable, public IGetID
{
public:
	IndexBuffer(Graphics& Gfx, const std::string& nameTag, const std::vector<unsigned short>& indicies);
	IndexBuffer(Graphics& Gfx, const std::vector<unsigned short>& indicies);
	void Bind(Graphics& Gfx) noexcept;

	UINT GetCount() const noexcept;
	std::string GetID() const noexcept override;
	static std::string GenerateID(const std::string& nameTag, [[maybe_unused]] const std::vector<unsigned short>&);
	static std::shared_ptr<IndexBuffer> Resolve(Graphics& Gfx, const std::string& nameTag, const std::vector<unsigned short>& indices) noxnd;
protected:
	UINT Count;
	std::string nameTag;
	Microsoft::WRL::ComPtr<ID3D11Buffer> p_IndexBuffer;
};