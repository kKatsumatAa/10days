#pragma once
#include"Object.h"


class TestChara : public Object
{
private:
	Vec4 color_ = { 1.0f,1.0f, 1.0f, 1.0f };

public:
	bool Initialize(std::unique_ptr<WorldMat> worldMat = nullptr) override;
	void Draw()override;
	void OnCollision(const CollisionInfo& info) override;

};