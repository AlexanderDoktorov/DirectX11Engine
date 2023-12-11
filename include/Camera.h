#pragma once
#include <DirectXMath.h>

class Camera
{
public:
	Camera()
	{
		Reset();
	}

	DirectX::XMMATRIX GetCameraMatrix() const noexcept;

	void ShowControlWindow(bool* p_open = (bool*)1) noexcept;
	void Rotate(float dx, float dy) noexcept;
	void Translate(DirectX::XMFLOAT3 translation) noexcept;
	void Reset();

	void SetPitch(float pitch);
	void SetYaw(float yaw);
	void SetPosition(DirectX::XMFLOAT3 pos);
	void SetPosX(float x);
	void SetPosY(float y);
	void SetPosZ(float z);
	void SetRotation(float pitch, float yaw);

	DirectX::XMFLOAT3	GetPos()				const noexcept;
	float				GetPitch()				const noexcept;
	float				GetYaw()				const noexcept;

	
private:
	DirectX::XMFLOAT3 pos;
	float pitch;
	float yaw;
	static constexpr float travelSpeed = 10.0f;
	static constexpr float rotationSpeed = 0.001f;
};