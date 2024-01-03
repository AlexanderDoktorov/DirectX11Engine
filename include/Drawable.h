#pragma once
#include "IndexBuffer.h"
#include "IBindable.h"
#include "IObject.h"
#include <memory>

class Drawable : virtual public IObject
{
public:
	~Drawable() = default;

	virtual void Draw(Graphics& Gfx) const noexcept;
	virtual DirectX::XMMATRIX GetTransform() const noexcept override;

protected:

	void AddBindable(std::shared_ptr<IBindable> element) noexcept;

	template <class T>
	static void RemoveBindable()
	{
		for (auto it = m_Bindables.begin(); it != m_Bindables.end();) {
			if(T* ptr = dynamic_cast<T*>(it->get())) {
				it->reset();
				it = m_Bindables.erase(it);
			}
			else {
				++it;
			}
		}
	}

	template <class T>
	T* QueryBindable() const noexcept
	{
		for (auto& bindable : m_Bindables)
		{
			if (auto pt = dynamic_cast<T*>(bindable.get()))
			{
				return pt;
			}
		}
		return nullptr;
	}

	std::vector<std::shared_ptr<IBindable>> m_Bindables;
	IndexBuffer* pIndexBuffer = nullptr;
};