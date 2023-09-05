#include "RectangleCollider.h"

void RectangleCollider::Update()
{
	// ���[���h�s�񂩂���W�𒊏o
	const M4& matWorld = object_->GetMatWorld();

	// ��`�̃����o�ϐ����X�V
	Vec2 center = {
		(float)matWorld.m_[3][0],
	(float)matWorld.m_[3][1],
	};

	//��]�p�̍s��
	WorldMat rotWM;
	rotWM.rot_.z_ = object_->GetRot().z_;
	rotWM.CalcRotMat();
	//�x�N�g������]�����Ē��_�Ƃ���
	Vec2 scale = { object_->GetScale().x_,object_->GetScale().y_ };

	Vec3 leftUp = GetVec3xM4({ -scale.x_,-scale.y_,0 }, rotWM.GetRotMat(), 0);
	Vec3 leftDown = GetVec3xM4({ -scale.x_,scale.y_,0 }, rotWM.GetRotMat(), 0);
	Vec3 rightUp = GetVec3xM4({ scale.x_,-scale.y_,0 }, rotWM.GetRotMat(), 0);
	Vec3 rightDown = GetVec3xM4({ scale.x_,scale.y_,0 }, rotWM.GetRotMat(), 0);

	//���S���W�ɑ�����
	MyRectangle::leftUp = center + Vec2(leftUp.x_, leftUp.y_);
	MyRectangle::leftDown = center + Vec2(leftDown.x_, leftDown.y_);
	MyRectangle::rightUp = center + Vec2(rightUp.x_, rightUp.y_);
	MyRectangle::rightDown = center + Vec2(rightDown.x_, rightDown.y_);
};


