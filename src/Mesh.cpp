#include "Mesh.h"
#include "Exceptions.h"

// ********** Mesh **********

Mesh::Mesh(Graphics& Gfx, std::vector<std::unique_ptr<IBindable>> pBindables)
{
	AddBindable( std::make_unique<Topology>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	for (size_t i = 0; i < pBindables.size(); i++)
	{
		AddBindable(std::move(pBindables[i]));
	}

	AddBindable( std::make_unique<TransformBuffer>( Gfx,*this ) );
}