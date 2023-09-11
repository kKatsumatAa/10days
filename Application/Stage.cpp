#include "Stage.h"
#include "Util.h"
#include <cstdint>

void Stage::Initialize(const Vec2& lt, const Vec2& rb)
{
	leftTop_ = lt;
	RightBottom_ = rb;

	//�摜�ǂݍ���
	png_Stage_ = TextureManager::LoadGraph("stage.png");
}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
	//�X�e�[�W�摜�`��
	obj_.SetTrans({ 640.0f,360.0f,0 });
	obj_.SetScale({ 1.1f,1.1f,1.0f });

	obj_.DrawBoxSprite(png_Stage_, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f });
	// �s�������͈͕\��
	//DrawBox((int32_t)leftTop_.x, (int32_t)leftTop_.y, (int32_t)RightBottom_.x, (int32_t)RightBottom_.y, 0x00ff00, false);
}
