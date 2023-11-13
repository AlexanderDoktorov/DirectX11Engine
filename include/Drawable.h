#pragma once
#include "Interfaces.h"
#include "DOK_assert.h"
#include "IndexBuffer.h"
#include "Topology.h"
#include <memory>

class Drawable : virtual public IObject
{
public:
	~Drawable() = default;

	virtual void Draw(Graphics& Gfx)
	{
		DOK_assert(pIndexBuffer != nullptr, L"IndexBuffer hasn't been set before drawing object, add it with AddBindable()");

		for (auto& el : PipelineElements)
		{
			el->Bind(Gfx);
		}
		if (pIndexBuffer)
			Gfx.DrawIndexed(pIndexBuffer->GetCount());
	}

protected:

	void AddBindable(std::unique_ptr<IBindable> element) noexcept {
		if (auto p = dynamic_cast<IndexBuffer*>(element.get()))
		{
			DOK_assert(pIndexBuffer == nullptr, L"Trying to add [NonStatic] IndexBuffer twice");
			pIndexBuffer = p;
		}
		PipelineElements.push_back(std::move(element));
	}

	template <class T>
	static void RemoveBindable()
	{
		for (auto it = PipelineElements.begin(); it != PipelineElements.end();) {
			if(T* ptr = dynamic_cast<T*>(it->get())) {
				it->reset();
				it = PipelineElements.erase(it);
			}
			else {
				++it;
			}
		}
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

	std::vector<std::unique_ptr<IBindable>> PipelineElements;
	IndexBuffer* pIndexBuffer = nullptr;
};