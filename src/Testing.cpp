#include "Testing.h"
#include "Buffer.h"

void DoTest()
{
	buffer::BufferLayout layout{};
	layout.AddField<DirectX::XMFLOAT3>("diffuseColor");
	layout.AddField<DirectX::XMFLOAT4>("someOption");
	layout.AddField<DirectX::XMFLOAT3>("ambientColor");
	layout.AddField<DirectX::XMFLOAT3>("specularColor");
	layout.AddField<DirectX::XMFLOAT3>("worldPosition");
	layout.AddField<bool>("hasAnyMaps");

	buffer::Buffer buff(layout);
	buff["diffuseColor"] = DirectX::XMFLOAT3{ 1.f,1.f,1.f };
	buff["ambientColor"] = DirectX::XMFLOAT3{ 0.f,0.f,0.f };
	buff["specularColor"] = DirectX::XMFLOAT3{ 0.1f,0.1f,0.1f };
	buff["someOption"] = DirectX::XMFLOAT4{ 0.2f,0.2f,0.2f,0.2f };
	DirectX::XMFLOAT3 diff  = buff["diffuseColor"];
	DirectX::XMFLOAT4 someOption  = buff["someOption"];
	DirectX::XMFLOAT3 ambientColor  = buff["ambientColor"];
	DirectX::XMFLOAT3 specularColor  = buff["specularColor"];
	DirectX::XMFLOAT3 worldPosition  = buff["worldPosition"];
	// DirectX::XMFLOAT4 badTypesConvert = buff["diffuseColor"];
	buff["hasAnyMaps"] = true;
	bool hasAnyMaps = buff["hasAnyMaps"];
}