#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include "hrException.h"
#include "IBlob.h"

class Blob : virtual public IBlob
{
public:
	Blob(const wchar_t* ShaderPath);
	Blob() = default;

	ID3DBlob*			GetBlob() noexcept override;
	void				ReadFile(const wchar_t* ShaderPath);

private:
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
};