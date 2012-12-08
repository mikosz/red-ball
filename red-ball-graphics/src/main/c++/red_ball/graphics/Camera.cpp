#include "Camera.hpp"

using namespace red_ball::graphics;

namespace {

inline float degreesToRadians(float degrees) {
    static const float DEG_180_BY_PI = 180.0f / D3DX_PI;
    return degrees * DEG_180_BY_PI;
}

} // anonymous namespace

Camera::Camera(const D3DXVECTOR3& position, const D3DXVECTOR3& rotation) :
    position_(position),
    rotation_(rotation) {
}

void Camera::viewMatrix(D3DXMATRIX* matrix) const {
    D3DXMATRIX rotationMatrix;
    D3DXMatrixRotationYawPitchRoll(
            &rotationMatrix,
            degreesToRadians(rotation_.y),
            degreesToRadians(rotation_.x),
            degreesToRadians(rotation_.z)
            );

    D3DXVECTOR3 lookAt(0.0f, 0.0f, 1.0f);
    D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
    lookAt += position_;

    D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
    D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

    D3DXMatrixLookAtLH(matrix, &position_, &lookAt, &up);
}
