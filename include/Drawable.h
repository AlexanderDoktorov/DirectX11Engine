#pragma once
#include "IndexBuffer.h"
#include "IBindable.h"
#include "ITransformable.h"
#include <memory>

class Drawable : public ITransformable
{
public:
	~Drawable() = default;

	virtual void Draw(Graphics& Gfx) const noexcept;
	virtual DirectX::XMMATRIX GetTransform() const noexcept override;

protected:
	void AddBindable(std::shared_ptr<IBindable> element) noexcept;
	void AddDefferedBindable(std::shared_ptr<IBindable> element) noexcept;
	void AddForwardBindable(std::shared_ptr<IBindable> element) noexcept;

	template <class T>
	requires std::is_base_of_v<IBindable, T>
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
	requires std::is_base_of_v<IBindable, T>
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
	std::vector<std::shared_ptr<IBindable>> m_ForwardBindables; /* bindables used when rendering in forward mode */
	std::vector<std::shared_ptr<IBindable>> m_DefferedBindables; /* bindables used when rendering in deffered mode */
	IndexBuffer* pIndexBuffer = nullptr;
};