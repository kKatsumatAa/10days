#include "RectangleCollider.h"

void RectangleCollider::Update()
{
	// ワールド行列から座標を抽出
	const M4& matWorld = object_->GetMatWorld();

	// 矩形のメンバ変数を更新
	Rectangle::center = {
		(float)matWorld.m_[3][0],
	(float)matWorld.m_[3][1],
	};
	//ワールド行列のスケールから半径
	Rectangle::length =
	{ object_->GetScale().x_ / 2.0f, object_->GetScale().y_ / 2.0f };
}
