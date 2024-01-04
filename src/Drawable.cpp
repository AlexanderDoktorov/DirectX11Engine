#include "Drawable.h"
#include "Graphics.h"
#include "DOK_assert.h"
#include <algorithm>

void Drawable::Draw(Graphics& Gfx) const noexcept
{
	DOK_assert(pIndexBuffer != nullptr, L"IndexBuffer hasn't been set before drawing object, add it with AddBindable()");

	switch (Gfx.GetRendererType())
	{
	case RENDERER_TYPE_DEFFERED:
		std::for_each(m_DefferedBindables.begin(), m_DefferedBindables.end(), [&Gfx](auto& p_Bind) {
			p_Bind->Bind(Gfx);
		});
		break;
	case RENDERER_TYPE_FORWARD:
		std::for_each(m_ForwardBindables.begin(), m_ForwardBindables.end(), [&Gfx](auto& p_Bind) {
			p_Bind->Bind(Gfx);
		});
		break;
	[[unlikely]] case RENDERER_TYPE_MIXED:
		break;
	default:
		break;
	}

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

void Drawable::AddDefferedBindable(std::shared_ptr<IBindable> element) noexcept
{
	assert( (dynamic_cast<IndexBuffer*>(element.get()) == nullptr) && "Trying to add deffered index buffer [WHAT?]");
	m_DefferedBindables.push_back(std::move(element));
}

void Drawable::AddForwardBindable(std::shared_ptr<IBindable> element) noexcept
{
	assert( (dynamic_cast<IndexBuffer*>(element.get()) == nullptr) && "Trying to add deffered index buffer [WHAT?]");
	m_ForwardBindables.push_back(std::move(element));
}
