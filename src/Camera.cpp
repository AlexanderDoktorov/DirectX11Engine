#include "Camera.h"
#include "imgui.h"
#include "DOK_math.h"
#include <algorithm>

DirectX::XMMATRIX Camera::GetCameraMatrix() const noexcept
{
    namespace dx = DirectX;
    using namespace dx;

    const dx::XMVECTOR forwardBaseVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    // apply the camera rotations to a base vector
    const auto lookVector = XMVector3Transform(forwardBaseVector,
        XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f)
    );
    // generate camera transform (applied to all objects to arrange them relative
    // to camera position/orientation in world) from cam position and direction
    // camera "top" always faces towards +Y (cannot do a barrel roll)
    const auto camPosition = XMLoadFloat3(&pos);
    const auto camTarget = camPosition + lookVector;

    return XMMatrixLookAtLH(camPosition, camTarget, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}

void Camera::Translate(DirectX::XMFLOAT3 translation) noexcept
{
    namespace dx = DirectX;
    dx::XMStoreFloat3(&translation, dx::XMVector3Transform(
        dx::XMLoadFloat3(&translation),
        dx::XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f) *
        dx::XMMatrixScaling(travelSpeed, travelSpeed, travelSpeed)
    ));
    pos = {
        pos.x + translation.x,
        pos.y + translation.y,
        pos.z + translation.z
    };
}

void Camera::Rotate(float dyaw, float dpitch) noexcept
{
    yaw = wrap_angle(yaw + dyaw * rotationSpeed);
    pitch = std::clamp(pitch + dpitch * rotationSpeed, 0.995f * -PI / 2.0f, 0.995f * PI / 2.0f);
}

void Camera::ShowControlWindow(bool *p_open) noexcept
{
    if (ImGui::Begin("Camera"), p_open)
    {
        ImGui::Text("Position");
        ImGui::SliderFloat("X", &pos.x, -80.0f, 80.0f, "%.1f");
        ImGui::SliderFloat("Y", &pos.y, -80.0f, 80.0f, "%.1f");
        ImGui::SliderFloat("Z", &pos.z, -80.0f, 80.0f, "%.1f");
        ImGui::Text("Orientation");
        ImGui::SliderAngle("Pitch", &pitch, 0.995f * -90.0f, 0.995f * 90.0f);
        ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
        if (ImGui::Button("Camera reset")) {
            Reset();
        }
    }
    ImGui::End();
}

void Camera::Reset()
{
    pos = { 3.6f, 9.8f, -24.0f };
    pitch = 0.f;
    yaw = 0.f;
}

// Setters
void Camera::SetPosition(DirectX::XMFLOAT3 pos)
{
    this->pos = pos;
}

void Camera::SetPosX(float x)
{
    pos.x = x;
}

void Camera::SetPosY(float y)
{
    pos.y = y;
}

void Camera::SetPosZ(float z)
{
    pos.z = z;
}

void Camera::SetRotation(float pitch, float yaw)
{
    this->pitch = pitch;
    this->yaw = yaw;
}

void Camera::Accelerate(float dv)
{
    travelSpeed + dv >= 0 ? travelSpeed += dv : travelSpeed;
}

DirectX::XMFLOAT3 Camera::GetPos() const noexcept
{
    return pos;
}

float Camera::GetPitch() const noexcept
{
    return pitch;
}

float Camera::GetYaw() const noexcept
{
    return yaw;
}

void Camera::SetPitch(float pitch)
{
    this->pitch = pitch;
}

void Camera::SetYaw(float yaw)
{
    this->yaw = yaw;
}

