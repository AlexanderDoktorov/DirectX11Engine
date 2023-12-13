#include "InputLayout.h"
#include "Exceptions.h"

InputLayout::InputLayout(Graphics& Gfx, const DynamicVertex::VertexLayout& vl, IShader* pShader) 
	:
	layout(vl)
{
	DOK_assert(pShader->GetBlob() != nullptr, L"Unable to create layout with null shader bytecode");
	CHECK_HR ( GetDevice(Gfx)->CreateInputLayout(vl.GetD3DLayout().data(), (UINT)vl.GetElementCount(), pShader->GetBlob()->GetBufferPointer(), pShader->GetBlob()->GetBufferSize(), &p_IL));
}

void InputLayout::Bind(Graphics& Gfx) noexcept
{
	GetContext(Gfx)->IASetInputLayout(p_IL.Get());
}

std::string InputLayout::GetID() const noexcept
{
	return GenerateID(layout);
}

std::string InputLayout::GenerateID(const DynamicVertex::VertexLayout& vl, IShader*)
{
	return std::string(typeid(InputLayout).name()).append("#").append(vl.GetCode());
}

std::shared_ptr<InputLayout> InputLayout::Resolve(Graphics& Gfx,const DynamicVertex::VertexLayout& vl, IShader* pShader)
{
	return BindableSystem::Resolve<InputLayout>(Gfx, vl, pShader);
}
