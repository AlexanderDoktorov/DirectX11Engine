#pragma once
#include "IBindable.h"
#include "DOK_assert.h"
#include "IndexBuffer.h"
#include "Topology.h"
#include <memory>

class Drawable
{
public:
	~Drawable() = default;

	virtual DirectX::XMMATRIX GetTransform() const noexcept = 0;
	virtual void Update(float dt) noexcept = 0;

	virtual void Draw(Graphics& Gfx)
	{
		DOK_assert(pIndexBuffer != nullptr, L"IndexBuffer hasn't been set before drawing object, add it with AddElement()");

		for (auto& el : PipelineElements)
		{
			el->Bind(Gfx);
		}
		if (pIndexBuffer)
			Gfx.DrawIndexed(pIndexBuffer->GetCount());
	}

protected:

	void AddElement(std::unique_ptr<IBindable> element) noexcept {
		if (auto p = dynamic_cast<IndexBuffer*>(element.get()))
		{
			DOK_assert(pIndexBuffer == nullptr, L"Trying to add [NonStatic] IndexBuffer twice");
			pIndexBuffer = p;
		}
		PipelineElements.push_back(std::move(element));
	}

	template <class T>
	T* QueryBindable()
	{
		for (auto& bindable : PipelineElements)
		{
			if (auto pt = dynamic_cast<T*>(bindable.get()))
			{
				return pt;
			}
		}
		return nullptr;
	}

	void SetTopology(const D3D11_PRIMITIVE_TOPOLOGY& tp)
	{
		if (Topology* topology = QueryBindable<Topology>())
		{
			topology->SetTopology(tp);
		}
	}

	std::vector<std::unique_ptr<IBindable>> PipelineElements;
	IndexBuffer* pIndexBuffer = nullptr;
};