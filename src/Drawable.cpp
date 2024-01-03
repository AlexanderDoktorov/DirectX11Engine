#include "Drawable.h"
#include "Graphics.h"
#include <algorithm>

void Drawable::Draw(Graphics& Gfx) const noexcept
{
	DOK_assert(pIndexBuffer != nullptr, L"IndexBuffer hasn't been set before drawing object, add it with AddBindable()");

	std::for_each(m_Bindables.begin(), m_Bindables.end(), [&Gfx](auto& p_Bind) {
		p_Bind->Bind(Gfx);
	});
	if (pIndexBuffer)
		Gfx.DrawIndexed(pIndexBuffer->GetCount());
}

DirectX::XMMATRIX Drawable::GetTransform() const noexcept
{
	return DirectX::XMMatrixIdentity();
}

void Drawable::AddBindable(std::shared_ptr<IBindable> element) noexcept
{
	if (auto p = dynamic_cast<IndexBuffer*>(element.get()))
	{
		DOK_assert(pIndexBuffer == nullptr, L"Trying to add [NonStatic] IndexBuffer twice");
		pIndexBuffer = p;
	}
	m_Bindables.push_back(std::move(element));
}