#include "TestChara.h"
#include "CircleCollider.h"


bool TestChara::Initialize(std::unique_ptr<WorldMat> worldMat)
{
	//親クラスの
	Object::Initialize();

	//自クラス
	SetCollider(std::make_unique<CircleCollider>());
	SetScale({ 30.0f,30.0f,1.0f });
	SetTrans({ WindowsApp::WINDOW_WIDTH_ / 2.0f, WindowsApp::WINDOW_HEIGHT_ / 2.0f,0 });

	return true;
}

void TestChara::Draw()
{
	DrawBoxSprite(NULL, color_, { 0.5f,0.5f });
	color_ = { 1.0f,1.0f,1.0f,1.0f };
}

void TestChara::OnCollision(const CollisionInfo& info)
{
	color_ = { 1.0f,0,0,1.0f };
}
