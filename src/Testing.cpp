#include "Testing.h"
#include "Buffer.h"

static_assert(std::is_convertible_v<float, bool>);

void DoTest()
{
	buffer::BufferLayout layout{};
	layout.AddField<DirectX::XMFLOAT3>("diffuseColor");
	layout.AddField<DirectX::XMFLOAT3>("ambientColor");
	layout.AddField<DirectX::XMFLOAT3>("specularColor");
	layout.AddField<DirectX::XMFLOAT3>("worldPosition");
	layout.AddField<bool>("hasAnyMaps");

	buffer::Buffer buff(layout);
	buff["diffuseColor"] = DirectX::XMFLOAT3{ 1.f,1.f,1.f };
	buff["ambientColor"] = DirectX::XMFLOAT3{ 0.f,0.f,0.f };
	buff["specularColor"] = DirectX::XMFLOAT3{ 0.1f,0.1f,0.1f };
	DirectX::XMFLOAT3 color = buff["ambientColor"];
	DirectX::XMFLOAT3 spec  = buff["specularColor"];
	DirectX::XMFLOAT3 diff  = buff["diffuseColor"];
	// DirectX::XMFLOAT4 badTypesConvert = buff["diffuseColor"];
	buff["hasAnyMaps"] = true;
	bool hasAnyMaps = buff["hasAnyMaps"];
	
}