#include "Drawable.h"
#include "Graphics.h"

void Drawable::Draw(Graphics& Gfx)
{
	DOK_assert(pIndexBuffer != nullptr, L"IndexBuffer hasn't been set before drawing object, add it with AddBindable()");

	for (auto& el : PipelineElements)
	{
		el->Bind(Gfx);
	}
	if (pIndexBuffer)
		Gfx.DrawIndexed(pIndexBuffer->GetCount());
}

DirectX::XMMATRIX Drawable::GetTransform() const noexcept
{
	return DirectX::XMMatrixIdentity();
}

void Drawable::AddBindable(std::unique_ptr<IBindable> element) noexcept
{
	if (auto p = dynamic_cast<IndexBuffer*>(element.get()))
	{
		DOK_assert(pIndexBuffer == nullptr, L"Trying to add [NonStatic] IndexBuffer twice");
		pIndexBuffer = p;
	}
	PipelineElements.push_back(std::move(element));
}