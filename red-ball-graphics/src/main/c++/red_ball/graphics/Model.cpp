#include "Model.hpp"

#include "red_ball/utils/pointee.hpp"
#include "MatrixBuffer.hpp"

using namespace red_ball::graphics;

namespace {

static const float FULL_ANGLE = D3DX_PI * 2.0f;

float removeFullRotations(float radians) {
    while (radians > FULL_ANGLE) {
        radians -= FULL_ANGLE;
    }
    return radians;
}

} // anonymous namespace

Model::Model() :
        translation_(0.0f, 0.0f, 0.0f),
        rotation_(0.0f, 0.0f, 0.0f),
        scale_(1.0f, 1.0f, 1.0f)
{
}

void Model::translateBy(float x, float y, float z) {
    translateBy(D3DXVECTOR3(x, y, z));
}

void Model::translateBy(const D3DXVECTOR3& location) {
    translation_ += location;
}

void Model::rotateBy(float pitch, float yaw, float roll) {
    rotateBy(D3DXVECTOR3(pitch, yaw, roll));
}

void Model::rotateBy(const D3DXVECTOR3& radians) {
    rotation_ += radians;
    rotation_.x = removeFullRotations(rotation_.x);
    rotation_.y = removeFullRotations(rotation_.y);
    rotation_.z = removeFullRotations(rotation_.z);
}

void Model::scaleBy(const D3DXVECTOR3& times) {
    scale_ += times;
}

D3DXMATRIX Model::worldMatrix() const {
    D3DXMATRIX rotationMatrix;
    D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation_.y, rotation_.x, rotation_.z);

    D3DXMATRIX scaleMatrix;
    D3DXMatrixScaling(&scaleMatrix, scale_.x, scale_.y, scale_.z);

    D3DXMATRIX translationMatrix;
    D3DXMatrixTranslation(&translationMatrix, translation_.x, translation_.y, translation_.z);

    return rotationMatrix * scaleMatrix * translationMatrix;
}
