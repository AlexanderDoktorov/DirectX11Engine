#pragma once
#include "DOK_assert.h"
#include "PipelineObject.h"

template <class T>
class DrawableBase : public PipelineObject
{
protected:
	DrawableBase() = default;

	static bool Initilized() noexcept { return initilized; }

	static void AddStaticElement(std::unique_ptr<IPipelineElement> _elem) noexcept
	{
		initilized = true;
		if (auto p = dynamic_cast<IndexBuffer*>(_elem.get()))
		{
			DOK_assert(staticIndexBuffer == nullptr, L"Trying to add [static] IndexBuffer twice");
			staticIndexBuffer = p;
		}
		StaticPipelineElements.push_back(std::move(_elem));
	}
public:
	void Draw(Graphics& Gfx) override
	{
		DOK_assert(staticIndexBuffer != nullptr || pIndexBuffer != nullptr, L"Non of the index buffer is set for drawing, add one with AddStaticElement() or AddElement()");
		DOK_assert(staticIndexBuffer == nullptr || pIndexBuffer == nullptr, L"Both of the index buffers [static] and [non-static] is set for drawing, which one to use?");

		for (auto& non_static_el : PipelineElements)
		{
			non_static_el->Attach(Gfx);
		}
		for (auto& static_el : StaticPipelineElements)
		{
			static_el->Attach(Gfx);
		}
		if(staticIndexBuffer)
			Gfx.DrawIndexed(staticIndexBuffer->GetCount());
		if(pIndexBuffer)
			Gfx.DrawIndexed(pIndexBuffer->GetCount());
	}

	virtual ~DrawableBase() = default;
private:
	static std::vector<std::unique_ptr<IPipelineElement>> StaticPipelineElements;
	static IndexBuffer* staticIndexBuffer;
	static bool initilized;
};

template<class T>
bool DrawableBase<T>::initilized = false;

template<class T>
IndexBuffer* DrawableBase<T>::staticIndexBuffer = nullptr;

template<class T>
std::vector<std::unique_ptr<IPipelineElement>> DrawableBase<T>::StaticPipelineElements {};
