#include "Column.h"

rectangle Column::ColumnRect = rectangle();

rectangle Column::GetInitialRect() noexcept
{
	return ColumnRect;
}

float Column::GetInitialWidth() noexcept
{
	return GetInitialRect().right_bottom.x - GetInitialRect().left_bottom.x;
}

float Column::GetInitialHeight() noexcept
{
	return GetInitialRect().left_top.y - GetInitialRect().left_bottom.y;
}