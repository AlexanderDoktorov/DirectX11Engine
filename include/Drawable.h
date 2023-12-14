#pragma once
#include "IndexBuffer.h"
#include "IBindable.h"
#include "IObject.h"
#include <memory>

class Drawable : virtual public IObject
{
public:
	~Drawable() = default;

	virtual void Draw(Graphics& Gfx);
	virtual DirectX::XMMATRIX GetTransform() const noexcept override;

protected:
	void AddBindable(std::shared_ptr<IBindable> element) noexcept;

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

	std::vector<std::shared_ptr<IBindable>> PipelineElements;
	IndexBuffer* pIndexBuffer = nullptr;
};