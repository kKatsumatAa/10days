#include "Camera2D.h"

using namespace DirectX;

void Camera2D::Initialize()
{
}

void Camera2D::Update()
{
	//������͈͍X�V
	VisiableUpdate();

	//�s��X�V
	MatrixUpdate();
}

void Camera2D::VisiableUpdate()
{
	visiableArea_.x_ = pos_.x_ - visiableArea_.z_ / 2.0f;
	visiableArea_.y_ = pos_.y_ - visiableArea_.w_ / 2.0f;
}

void Camera2D::MatrixUpdate()
{
	//3D�ɕϊ�
	Vec3 pos = { pos_.x_,pos_.y_,0 };
	Vec3 screenPos = { screenCenterPos_.x_,screenCenterPos_.y_,0 };

	//�s��쐬
	cameraMatrix_ = XMMatrixIdentity();
	cameraMatrix_ *= XMMatrixTranslation(-pos.x_, -pos.y_, -pos.z_);
	cameraMatrix_ *= XMMatrixScaling(zoom_.x_, zoom_.y_, 1.0f);
	cameraMatrix_ *= XMMatrixRotationZ(rot_);
	cameraMatrix_ *= XMMatrixTranslation(screenPos.x_, screenPos.y_, screenPos.z_);
}
