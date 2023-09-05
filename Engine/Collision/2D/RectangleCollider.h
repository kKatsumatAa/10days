#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

/// <summary>
/// �~�Փ˔���I�u�W�F�N�g
/// </summary>
class RectangleCollider : public BaseCollider, public Rectangle
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMVECTOR = DirectX::XMVECTOR;
public:
	RectangleCollider(const DirectX::XMFLOAT2 center = { 0,0 }, DirectX::XMFLOAT2 length = { 1.0f, 1.0f })
	{
		Rectangle::center = center;
		Rectangle::length = length;

		// ���`����Z�b�g
		shapeType_ = COLLISIONSHAPE_CIRCLE;

		is2D_ = true;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	inline void SetRadius(DirectX::XMFLOAT2 length) { Rectangle::length = length; }
	inline DirectX::XMFLOAT2 GetRadius() { return Rectangle::length; }
};

