#pragma once
#include "DOK_assert.h"
#include "Drawable.h"

template <class T>
class DrawableBase : public Drawable
{
protected:
	DrawableBase() = default;

	static bool Initilized() noexcept { return initilized; }

	static void AddStaticElement(std::unique_ptr<IBindable> _elem) noexcept
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
		//DOK_assert_noexit(staticIndexBuffer != nullptr || pIndexBuffer != nullptr, L"Non of the index buffer is set for drawing, add one with AddStaticElement() or AddElement()");
		DOK_assert_noexit(staticIndexBuffer == nullptr || pIndexBuffer == nullptr, L"Both of the index buffers [static] and [non-static] is set for drawing, which one to use?");

		for (auto& non_static_el : PipelineElements)
		{
			non_static_el->Bind(Gfx);
		}
		for (auto& static_el : StaticPipelineElements)
		{
			static_el->Bind(Gfx);
		}
		if(staticIndexBuffer)
			Gfx.DrawIndexed(staticIndexBuffer->GetCount());
		if(pIndexBuffer)
			Gfx.DrawIndexed(pIndexBuffer->GetCount());
	}

	void DrawIntoBackBuffer(Graphics& Gfx)
	{
		Gfx.BindBackBufferAsRenderTarget();
		Draw(Gfx);
	}

	void DrawIntoGBuffer(Graphics& Gfx)
	{
		Gfx.BindGBufferRenderTargets();
		Draw(Gfx);
	}

	virtual ~DrawableBase() = default;
private:
	static std::vector<std::unique_ptr<IBindable>> StaticPipelineElements;
	static IndexBuffer* staticIndexBuffer;
	static bool initilized;
};

template<class T>
bool DrawableBase<T>::initilized = false;

template<class T>
IndexBuffer* DrawableBase<T>::staticIndexBuffer = nullptr;

template<class T>
std::vector<std::unique_ptr<IBindable>> DrawableBase<T>::StaticPipelineElements {};
