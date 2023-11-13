#pragma once
#include "IBindable.h"

class ComputeShader : public IBindable
{
public:
	ComputeShader(Graphics& Gfx, const wchar_t* fileName);

	ID3DBlob* GetBlob() noexcept;

	void Bind(Graphics& Gfx) noexcept override;
	void Unbind(Graphics& Gfx) noexcept;
	void Execute(Graphics& Gfx, UINT num_threads_x, UINT num_threads_y, UINT num_threads_z) noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> p_ComputeShader;
	Microsoft::WRL::ComPtr<ID3DBlob> p_Blob;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> p_resultUAV;
};