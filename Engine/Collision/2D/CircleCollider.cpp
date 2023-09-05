#include "CircleCollider.h"


using namespace DirectX;

void CircleCollider::Update()
{
	// ���[���h�s�񂩂���W�𒊏o
	const M4& matWorld = object_->GetMatWorld();

	// �~�̃����o�ϐ����X�V
	Circle::center = {
		(float)matWorld.m_[3][0],
	(float)matWorld.m_[3][1],
	};
	//���[���h�s��̃X�P�[�����甼�a
	Circle::radius =
		(object_->GetScale().x_ + object_->GetScale().y_ + object_->GetScale().z_) / 3.0f;
}
