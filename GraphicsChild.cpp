#include "GraphicsChild.h"
#include "Graphics.h"

ID3D11Device* GraphicsChild::GetDevice(Graphics& Gfx)
{
    return Gfx.p_Device.Get();
}

ID3D11DeviceContext* GraphicsChild::GetContext(Graphics& Gfx)
{
    return Gfx.p_Context.Get();
}
