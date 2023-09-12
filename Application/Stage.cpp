#include "Stage.h"
#include "Util.h"
#include <cstdint>

void Stage::Initialize(const Vec2& lt, const Vec2& rb)
{
	leftTop_ = lt;
	RightBottom_ = rb;

	//画像読み込み
	png_Stage_ = TextureManager::LoadGraph("stage2.png");
	png_Background_ = TextureManager::LoadGraph("background.png");
}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
	//ステージ画像描画
	objStage_.SetTrans({ 640.0f,360.0f,0 });
	objStage_.SetScale({ 1.f,1.f,1.0f });

	objBack_.SetTrans({ 640.0f,360.0f,0 });
	objBack_.SetScale({ 2.f,2.f,1.f });

	objBack_.DrawBoxSprite(nullptr, png_Background_, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f });
	objStage_.DrawBoxSprite(nullptr, png_Stage_, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f });
	// 行動制限範囲表示
	//DrawBox((int32_t)leftTop_.x, (int32_t)leftTop_.y, (int32_t)RightBottom_.x, (int32_t)RightBottom_.y, 0x00ff00, false);
}
