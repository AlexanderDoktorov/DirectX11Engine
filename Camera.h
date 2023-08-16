#pragma once
#include <DirectXMath.h>

class Camera
{
public:
	Camera() = default;

	DirectX::XMMATRIX GetCameraMatrix() const noexcept;
	void RotateYaw(float dyaw);

	void ShowControlWindow(bool* p_open = (bool*)0) noexcept;
	void Reset();

private:
	DirectX::XMFLOAT3 CameraPosition { 0.f, 0.f, -10.f };
	DirectX::XMVECTOR EyeDirection { DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f) }; // along Z axis
	float direction_pitch = 0.f;
	float direction_yaw = 0.f;
	float updown_angle = 0.f;
	float leftright_angle = 0.f;
private:
	float speed_factor = 1.f;
};