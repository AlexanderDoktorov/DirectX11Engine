#pragma once
#include "BaseWindow.h"

struct ResizeInfo
{
	ResizeInfo() = default;
	ResizeInfo(const UINT& w, const UINT& h) : g_ResizeWidth(w), g_ResizeHeight(h) {}

	UINT g_ResizeWidth = 0U;
	UINT g_ResizeHeight = 0U;
};

template <class T>
class ResizingBaseWindow : public BaseWindow<T>
{
private:
	mutable ResizeInfo res_info;
public:
	ResizeInfo	GetResizeInfo() const noexcept { return res_info; }
	void		ZeroResizeInfo() const noexcept { res_info.g_ResizeHeight = 0U; res_info.g_ResizeWidth = 0U; }
protected:
	virtual void SetResizeInfo(const ResizeInfo& rinfo) { res_info = rinfo; }
};