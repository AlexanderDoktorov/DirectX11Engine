#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include "Exceptions.h"

class Blob
{
public:
	Blob(const wchar_t* ShaderPath);
	Blob() = default;

	ID3DBlob*			GetBlob() noexcept;
	void				ReadFile(const wchar_t* ShaderPath);

	ID3DBlob* operator->() { return pBlob.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
};