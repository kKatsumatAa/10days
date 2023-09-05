#include "CircleCollider.h"


using namespace DirectX;

void CircleCollider::Update()
{
	// ワールド行列から座標を抽出
	const M4& matWorld = object_->GetMatWorld();

	// 円のメンバ変数を更新
	Circle::center = {
		(float)matWorld.m_[3][0],
	(float)matWorld.m_[3][1],
	};
	//ワールド行列のスケールから半径
	Circle::radius =
		(object_->GetScale().x_ + object_->GetScale().y_ + object_->GetScale().z_) / 3.0f;
}
