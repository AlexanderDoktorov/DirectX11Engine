#pragma once
#include "Column.h"
#include <DirectXMath.h>

class Histogramm
{
	friend class Application;
public:
	Histogramm(Graphics& gfx, const rectangle& region)
		: max_height(region.right_top.y - region.right_bottom.y),
		  max_width(region.right_bottom.x - region.left_bottom.x)
	{
		Column::Make(gfx, region.left_bottom, max_height, max_width);
	}

	void	Clear();
	void	UpdateHistogramm();
	void	Draw(Graphics& gfx);
	void	PushColumn(Graphics& gfx, const float& height_in_percent);
	float	GetColumnWidth() const noexcept;

	static std::vector<dx::XMFLOAT4> color_palette;

private:
	std::vector<std::unique_ptr<Column>> columns;
	
	const float max_width;
	const float max_height;
};

