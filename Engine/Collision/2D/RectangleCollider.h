#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

/// <summary>
/// 円衝突判定オブジェクト
/// </summary>
class RectangleCollider : public BaseCollider, public Rectangle
{
private: // エイリアス
	// DirectX::を省略
	using XMVECTOR = DirectX::XMVECTOR;
public:
	RectangleCollider(const DirectX::XMFLOAT2 center = { 0,0 }, DirectX::XMFLOAT2 length = { 1.0f, 1.0f })
	{
		Rectangle::center = center;
		Rectangle::length = length;

		// 球形状をセット
		shapeType_ = COLLISIONSHAPE_CIRCLE;

		is2D_ = true;
	}

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	inline void SetRadius(DirectX::XMFLOAT2 length) { Rectangle::length = length; }
	inline DirectX::XMFLOAT2 GetRadius() { return Rectangle::length; }
};

