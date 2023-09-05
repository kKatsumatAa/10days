#include "RectangleCollider.h"

void RectangleCollider::Update()
{
	// ���[���h�s�񂩂���W�𒊏o
	const M4& matWorld = object_->GetMatWorld();

	// ��`�̃����o�ϐ����X�V
	Rectangle::center = {
		(float)matWorld.m_[3][0],
	(float)matWorld.m_[3][1],
	};
	//���[���h�s��̃X�P�[�����甼�a
	Rectangle::length =
	{ object_->GetScale().x_ / 2.0f, object_->GetScale().y_ / 2.0f };
}
