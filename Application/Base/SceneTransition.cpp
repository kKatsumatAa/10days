#include "SceneTransition.h"
#include <algorithm>
#include "MathUtil.h"


uint64_t SceneTransition::png_rolledScroll_;
uint64_t SceneTransition::png_unrollScroll_;
uint64_t SceneTransition::png_topBeltBG_;
uint64_t SceneTransition::png_bottomBeltBG_;


void SceneTransition::StaticInit()
{
	png_rolledScroll_ = TextureManager::LoadGraph("makimono.png");
	png_unrollScroll_ = TextureManager::LoadGraph("wasi_back2.png");
	png_topBeltBG_ = TextureManager::LoadGraph("wasi_frame_up2.png");
	png_bottomBeltBG_ = TextureManager::LoadGraph("wasi_frame_bottom2.png");
}

void SceneTransition::Reset(void)
{
	// �����Ă镔��
	pos_rolledScroll_.x = 1350;
	pos_rolledScroll_.y = 360;
	pos_unrollScroll_.x = 640 + 1280 + 70;
	pos_unrollScroll_.y = 360;
	pos_TBBeltBG_.x = 640 + 1280 + 70;
	pos_TBBeltBG_.y = 30;

	// �A���t�@�l������
	alpha_ = 255;

	// �t���[���J�E���g������
	frameCount_rolled_ = 0;
	frameCount_transBG_ = 0;
	frameCount_moveTBBG_ = 0;
}

void SceneTransition::Start(void)
{
	Reset();
	isExe_ = true;
	isBeginEnd_ = false;
}

void SceneTransition::End(void)
{
	isExe_ = false;
	//Reset();
}

void SceneTransition::Update(void)
{
	if (isExe_ && !isBeginEnd_)
	{
		// 1�Ԗڏ����̈ړ��̃��[�g
		float rate_move1 = (std::min)((float)frameCount_rolled_ / kMaxFrameRolled_, 1.f);
		// �����Ă��̍��W
		pos_rolledScroll_.x = Math::Ease::EaseInOutSine(rate_move1, 1350, -70); // �ړ��� 1420
		// �����ĂȂ���^�񒆂̍��W
		pos_unrollScroll_.x = Math::Ease::EaseInOutSine(rate_move1, 1920 + 35, 640); // �ړ��� 1420
		// �����ĂȂ���㉺�̍��W
		pos_TBBeltBG_.x = Math::Ease::EaseInOutSine(rate_move1, 1920 + 35, 640); // �ړ��� 1420

		if (rate_move1 >= 1.0f)
		{
			isBeginEnd_ = true;
		}

		frameCount_rolled_++;
	}
	else if (isExe_)
	{
		// 2�Ԗڏ����̉摜���߂̃��[�g
		float rate_trans = (std::min)((float)frameCount_transBG_ / kMaxFrameRolledTrans_, 1.f);
		alpha_ = (int32_t)Math::Ease::EaseInSine(rate_trans, 255, 0); // �ړ��� 1420
		//alpha_ = (1 - rate_trans) * 255;

		// �t���[���J�E���g���Z
		frameCount_transBG_++;
		if (rate_trans >= 1.f)
		{
			// 3�Ԗڏ����̉摜���߂̃��[�g
			float rate_move2 = (std::min)((float)frameCount_moveTBBG_ / kMaxFrameTBBelt_, 1.f);
			// �����ĂȂ���㉺�̍��W 
			pos_TBBeltBG_.y = Math::Ease::EaseOutCubic(rate_move2, 30, -40); // �ړ��� 70
			//pos_TBBeltBG_.y = (1 - rate_move2) * 70 - 40;

			// �t���[���J�E���g���Z
			frameCount_moveTBBG_++;
			if (rate_move2 >= 1.f)
			{
				End();
				return;
			}
		}
	}
}

void SceneTransition::Draw(void)
{
	// �w�i-�^�񒆂̕���
	wasiBack2_.SetScale({ 1.0f,1.0f,0 });
	wasiBack2_.SetTrans({ pos_unrollScroll_.x,pos_unrollScroll_.y,0 });
	wasiBack2_.SetRot({ 0,0,0 });
	wasiBack2_.DrawBoxSprite(png_unrollScroll_, { 1.0f,1.0f,1.0f,(float)alpha_ / 255.0f }, { 0.5f,0.5f });

	// �w�i-�㉺�̑�
	wasiFrameUp2_.SetScale({ 1.0f,1.0f,0 });
	wasiFrameUp2_.SetTrans({ pos_TBBeltBG_.x,pos_TBBeltBG_.y,0 });
	wasiFrameUp2_.SetRot({ 0,0,0 });
	wasiFrameUp2_.DrawBoxSprite(png_topBeltBG_, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f });

	wasiFrameBottom2_.SetScale({ 1.0f,1.0f,0 });
	wasiFrameBottom2_.SetTrans({ pos_TBBeltBG_.x,(720.f - pos_TBBeltBG_.y - 1),0 });
	wasiFrameBottom2_.SetRot({ 0,0,0 });
	wasiFrameBottom2_.DrawBoxSprite(png_bottomBeltBG_, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f });

	// ����
	makimono_.SetScale({ 1.0f,1.0f,0 });
	makimono_.SetTrans({ pos_rolledScroll_.x,(720.f - pos_rolledScroll_.y - 1),0 });
	makimono_.SetRot({ 0,0,0 });
	makimono_.DrawBoxSprite(png_rolledScroll_, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f });
}
