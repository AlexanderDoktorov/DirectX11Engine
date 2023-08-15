#pragma once
#include "PipelineObject.h"
#include "VertexConstantBuffer.h"

class TransformBuffer : public IPipelineElement
{
public:

	TransformBuffer(Graphics& Gfx, PipelineObject& parent);

	virtual void Attach(Graphics& Gfx) noexcept override;

private:
	const PipelineObject& parent;
	static	std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> Cbuff;
};