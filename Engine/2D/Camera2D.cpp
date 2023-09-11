#include "Camera2D.h"

using namespace DirectX;

void Camera2D::Initialize()
{
}

void Camera2D::Update()
{
	//見える範囲更新
	VisiableUpdate();

	//行列更新
	MatrixUpdate();
}

void Camera2D::VisiableUpdate()
{
	visiableArea_.x = pos_.x - visiableArea_.z / 2.0f;
	visiableArea_.y = pos_.y - visiableArea_.w / 2.0f;
}

void Camera2D::MatrixUpdate()
{
	//3Dに変換
	Vec3 pos = { pos_.x,pos_.y,0 };
	Vec3 screenPos = { screenCenterPos_.x,screenCenterPos_.y,0 };

	//行列作成
	cameraMatrix_ = XMMatrixIdentity();
	cameraMatrix_ *= XMMatrixTranslation(-pos.x, -pos.y, -pos.z);
	cameraMatrix_ *= XMMatrixScaling(zoom_.x, zoom_.y, 1.0f);
	cameraMatrix_ *= XMMatrixRotationZ(rot_);
	cameraMatrix_ *= XMMatrixTranslation(screenPos.x, screenPos.y, screenPos.z);
}
