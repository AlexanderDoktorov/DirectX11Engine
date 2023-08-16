#include "Camera.h"
#include "imgui.h"

DirectX::XMMATRIX Camera::GetCameraMatrix() const noexcept
{
    using namespace DirectX;
    const auto EyePosition = DirectX::XMVector3Transform ( DirectX::XMLoadFloat3(&CameraPosition), DirectX::XMMatrixRotationRollPitchYaw(updown_angle, leftright_angle, 0.f) );
    const auto TransformedEyeDirection = XMVector4Transform( EyeDirection, XMMatrixRotationRollPitchYaw(direction_pitch, direction_yaw, 0.f));

    return DirectX::XMMatrixLookToLH(EyePosition, TransformedEyeDirection, XMVectorSet(0.0f, 1.0f, 0.0f, 0.f));
}

void Camera::RotateYaw(float dyaw)
{
    direction_yaw += speed_factor * dyaw;
}

void Camera::ShowControlWindow(bool *p_open) noexcept
{
    using namespace DirectX;
    const auto EyePosition = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&CameraPosition), DirectX::XMMatrixRotationRollPitchYaw(updown_angle, leftright_angle, 0.f));
    const auto TransformedEyeDirection = XMVector4Transform(EyeDirection, XMMatrixRotationRollPitchYaw(direction_pitch, direction_yaw, 0.f));

    if(ImGui::Begin("Camera", p_open))
    {
        ImGui::Text("Position of the camera");
        ImGui::SliderFloat3("Position {X, Y, Z} ",      &CameraPosition.x, -30.f, 30.f, "%.1f");
        ImGui::SliderAngle("Pitch (around x) angle",    &updown_angle);
        ImGui::SliderAngle("Yaw (around y) angle",      &leftright_angle);
        ImGui::Text("Camera direction");
        ImGui::SliderAngle("Direction pitch",           &direction_pitch);
        ImGui::SliderAngle("Direction yaw ",            &direction_yaw);
        ImGui::SliderFloat("Speed Factor",              &speed_factor, 0.f, 30.f);
        if (ImGui::Button("Reset")) Reset();

        ImGui::BeginChild("Camera status", ImVec2(440, 130), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        ImGui::Text("Eye Position (%f, %f, %f) ", XMVectorGetX(EyePosition), XMVectorGetY(EyePosition), XMVectorGetZ(EyePosition));
        ImGui::Text("Transformed Eye Direction (%f, %f, %f) ", XMVectorGetX(TransformedEyeDirection), XMVectorGetY(TransformedEyeDirection), XMVectorGetZ(TransformedEyeDirection));
        ImGui::EndChild();
    }
    ImGui::End();
}

void Camera::Reset()
{
    CameraPosition  = { 0.f, 0.f, -10.f };
    updown_angle    = 0.f;
    leftright_angle = 0.f;
    direction_pitch = 0.f;
    direction_yaw   = 0.f;
    speed_factor    = 1.f;
}


