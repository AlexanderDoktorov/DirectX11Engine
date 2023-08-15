#pragma once
#include "IPipelineElement.h"
#include "DOK_assert.h"
#include "IndexBuffer.h"
#include <memory>

class PipelineObject
{
public:
	~PipelineObject() = default;

	virtual DirectX::XMMATRIX GetTransform() const noexcept = 0;
	virtual void Update(float dt) noexcept = 0;

	void AddElement(std::unique_ptr<IPipelineElement> element) noexcept { 
		if (auto p = dynamic_cast<IndexBuffer*>(element.get()))
		{
			DOK_assert(pIndexBuffer == nullptr, L"Trying to add [NonStatic] IndexBuffer twice");
			pIndexBuffer = p;
		}
		PipelineElements.push_back(std::move(element)); 
	}

	virtual void Draw(Graphics& Gfx)
	{
		DOK_assert(pIndexBuffer != nullptr, L"IndexBuffer hasn't been set before drawing object, add it with AddElement()");

		for (auto& el : PipelineElements)
		{
			el->Attach(Gfx);
		}
		if (pIndexBuffer)
			Gfx.DrawIndexed(pIndexBuffer->GetCount());
	}	

protected:
	std::vector<std::unique_ptr<IPipelineElement>> PipelineElements;
	IndexBuffer* pIndexBuffer = nullptr;
};