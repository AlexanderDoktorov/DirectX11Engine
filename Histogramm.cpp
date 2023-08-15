#include "Histogramm.h"

std::vector<dx::XMFLOAT4> Histogramm::color_palette =
{
	{ 0.941176534f, 0.972549081f, 1.000000000f, 1.000000000f },
	{ 0.980392218f, 0.921568692f, 0.843137324f, 1.000000000f },
	{ 0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f },
	{ 0.498039246f, 1.000000000f, 0.831372619f, 1.000000000f },
	{ 0.941176534f, 1.000000000f, 1.000000000f, 1.000000000f },
	{ 0.960784376f, 0.960784376f, 0.862745166f, 1.000000000f },
	{ 1.000000000f, 0.894117713f, 0.768627524f, 1.000000000f },
	{ 0.000000000f, 0.000000000f, 0.000000000f, 1.000000000f },
	{ 1.000000000f, 0.921568692f, 0.803921640f, 1.000000000f },
	{ 0.000000000f, 0.000000000f, 1.000000000f, 1.000000000f },
	{ 0.541176498f, 0.168627456f, 0.886274576f, 1.000000000f },
	{ 0.647058845f, 0.164705887f, 0.164705887f, 1.000000000f },
	{ 0.870588303f, 0.721568644f, 0.529411793f, 1.000000000f },
	{ 0.372549027f, 0.619607866f, 0.627451003f, 1.000000000f },
	{ 0.498039246f, 1.000000000f, 0.000000000f, 1.000000000f }
};

void Histogramm::Draw(Graphics& gfx)
{
	for (auto& clmn : columns)
	{
		clmn->Draw(gfx);
	}
}

void Histogramm::PushColumn(Graphics& gfx, const float& height_in_percent)
{
	columns.push_back(std::make_unique<Column>(gfx));
	columns.back()->SetScaleFactorY( (height_in_percent / 100.f) * (Column::GetInitialHeight() / max_height) );
	UpdateHistogramm();
}

float Histogramm::GetColumnWidth() const noexcept
{
	return max_width / (float)columns.size();
}

void Histogramm::Clear()
{
	columns.clear();
}

void Histogramm::UpdateHistogramm()
{
	for (size_t i = 0; i < columns.size(); ++i)
	{
		columns[i]->SetX(i * max_width / columns.size());
		columns[i]->SetScaleFactorX(max_width / (Column::GetInitialWidth() * columns.size()));
		columns[i]->SetColor(color_palette[i % color_palette.size()]);
	}
}
