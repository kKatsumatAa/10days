#include "Player.h"

#include "UtilL.h"
#include "MathUtil.h"
#include "KeyboardInput.h"
#include "PadInput.h"
#include "Vec3.h"
#include "Enemy.h"
#include "SceneManager.h"
#include "EnemyManager.h"
#include "GameVelocityManager.h"
#include "GameVelocityState.h"
#include "HitStop.h"
#include "CameraManager.h"
#include "ParticleManager.h"
#include "Score.h"


const float Player::kMowDist_{ 15.f };// �ガ�����Ő�����΂����� �������ύX����Ȃ�enemy.h�̊������M��Ȃ��Əu�Ԉړ��ɂȂ����܂�
bool Player::isSkewerScreenBlack4SceneM_{};

Player::Player(CollisionManger* colMPtr, Stage* stagePtr) : IEntity(stagePtr), mow_(colMPtr), skewer_(colMPtr), colMPtr_(colMPtr)
{
	// �Փ˃}�l�[�W���ւ̓o�^
	colMPtr->Register(this);

	// �`��ݒ�
	shape_ = Shape::CIRCLE;
	// ���̐ݒ�
	id_ = "player";

	// �Փ�callback���f
	onCollision_ = std::bind(&Player::OnCollision, this);

	// ���肪���̏�Ɏc�葱�����Ⴄ����A��΂Ɉ���������Ȃ����W�ɓ]�����邲�艟���B pos(-10��,-10��)
	skewer_.SetPos({ -100000.f, -100000.f });

	areaColor_ = { 1.0f,0.2f,0.3f,0.5f };
}

Player::~Player(void)
{
	// �o�^�̖���
	onCollision_ = nullptr;
	colMPtr_->UnRegister(this);
}

void Player::Initialize()
{
	PostEffectManager::GetInstance().GetPostEffect1()->effectFlags_.isRadialBlur = false;
	CameraManager::GetInstance().GetCamera2D()->EndFollow();
	CameraManager::GetInstance().GetCamera2D()->SetZoom({ 1.0f,1.0f });
}

void Player::Update(void)
{
	// ���G���Ԓ��Ȃ�
	if (frameCount_invincible_ != 0)
	{
		// ���G���Ԃ̃t���[���J�E���g�����Z
		Math::Function::LoopIncrement<int32_t>(frameCount_invincible_, 0, kMaxInvincibleFrame_);
	}

	if (frameCount_postSkewerInvincible_)
	{
		frameCount_postSkewerInvincible_++;
		if (frameCount_postSkewerInvincible_ > kMaxFrame_postSkewerInvincible_)
		{
			frameCount_postSkewerInvincible_ = 0;
		}
	}

	void (Player:: * FuncTbl[])() =
	{
		&Player::MoveUpdate,
		&Player::MowAttackUpdate,
		&Player::SkewerAttackUpdate,
	};

	(this->*FuncTbl[(size_t)state_])();

	// ������΂����t���O�I����������
	if (isKnockback_)
	{
		// �C�[�W���O�p�̃^�C�����[�g
		float rate = (std::min)((float)frameCount_knockback_ / kMaxKnockbackFrame_, 1.f);

		// ������΂���鑬�� �� �C�[�W���O�Œ���
		const float mowSpeed = (1 - Math::Ease::EaseInSine(rate)) * Player::kKnockbackDist_;
		// ���W�ɉ��Z
		position_ += vec_contactE2P_ * mowSpeed;

		if (rate >= 1.f)
		{
			frameCount_knockback_ = 0;
			isKnockback_ = false;
			return;
		}

		// �t���[���J�E���g���Z
		Math::Function::LoopIncrement<int32_t>(frameCount_knockback_, 0, kMaxKnockbackFrame_);
	}

	// �f�o�b�O���C���p�L�^
	pos4Line_ = position_ + vec_move_ * 30;

    // ���������]�p���擾
    rotation_ = std::acos(Vec2(0, -1).Dot(vec_move_));
    //���]���Ȃ��悤��
    if (vec_move_.x < 0)
    {
        rotation_ = -rotation_;
    }
    // �E�����������擾
    isRight_ = Vec2(0, -1).Cross(vec_move_.GetNormalize()) > 0.f;

    uint32_t num = 0;
    if (EnemyManager::GetInstance().GetDefeatedEnemiesNum(num))
    {
        //�ːi�ŉ҂����X�R�A�̉��o
        Score::GetInstance()->BeginOneSkewerEffect(0.4f, { 1250.f, 120.f }, 150);
    }
}

void Player::Draw(void)
{
	Object::SetScale({ radius_.x,radius_.x,0 });
	Object::SetTrans({ position_.x,position_.y,0 });
	Object::SetRot({ 0,0,rotation_ });

	// �`��
	//DrawLineAA(position_.x, position_.y, pos4Line_.x, pos4Line_.y, UtilL::Color::WHITE, 3);

	// ���݂̃v���C���[�̏�ԁi�����̂݁j
	//DrawFormatString(0, 140, 0xffffff, "state ;%d", static_cast<int32_t>(state_));

	// �v���C���[�̉E�������o��
	Vec3 vec3_move = { vec_move_.x,vec_move_.y,0 };
	Vec3 vec3_right = Vec3(0, 0, 1).Cross(vec3_move.GetNormalized());
	Vec2 vec2_right = { vec3_right.x,vec3_right.y };

	// skewer�ׂ̈Ƀ{�^�����������Ă�Ȃ�
	if (frameCount_4Skewer_ > 0)
	{
		// box�p�ϐ�
		//Vec2 frame_pos4PredictionBox_front = position_ - vec2_right * radius_.x;
		//Vec2 frame_pos4PredictionBox_Back = position_ + vec_move_ * 1000 + vec2_right * radius_.x;
		//Vec2 contents_pos4PredictionBox_front = position_ - vec2_right * (radius_.x - 1);
		//Vec2 contents_pos4PredictionBox_Back = position_ + vec_move_ * 1000 + vec2_right * (radius_.x - 1);

		// Dxlib��DrawBox()�͉�]�@�\�Ȃ����烀���ł�
		//DrawBox((int32_t)frame_pos4PredictionBox_front.x, (int32_t)frame_pos4PredictionBox_front.y, (int32_t)frame_pos4PredictionBox_Back.x, (int32_t)frame_pos4PredictionBox_Back.y, Color::RED,false);
		//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 30);
		//DrawBox((int32_t)contents_pos4PredictionBox_front.x, (int32_t)contents_pos4PredictionBox_front.y, (int32_t)contents_pos4PredictionBox_Back.x, (int32_t)contents_pos4PredictionBox_Back.y, Color::RED,true);
		//SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

		// line�p�ϐ�
		Vec2 pos_predictionLine_LB = position_ - vec2_right * radius_.x;
		Vec2 pos_predictionLine_LT = position_ + vec_move_ * 1500 - vec2_right * radius_.x;
		Vec2 pos_predictionLine_RB = position_ + vec2_right * radius_.x;
		Vec2 pos_predictionLine_RT = position_ + vec_move_ * 1500 + vec2_right * radius_.x;

		// DrawLine�Ŗ�������܂�
		//DrawLine((int32_t)pos_predictionLine_LB.x, (int32_t)pos_predictionLine_LB.y, (int32_t)pos_predictionLine_LT.x, (int32_t)pos_predictionLine_LT.y, Color::RED, 2); // ���c
		//DrawLine((int32_t)pos_predictionLine_RB.x, (int32_t)pos_predictionLine_RB.y, (int32_t)pos_predictionLine_RT.x, (int32_t)pos_predictionLine_RT.y, Color::RED, 2); // �E�c
		//DrawLine((int32_t)pos_predictionLine_LB.x, (int32_t)pos_predictionLine_LB.y, (int32_t)pos_predictionLine_RB.x, (int32_t)pos_predictionLine_RB.y, Color::RED, 2); // ����
		//DrawLine((int32_t)pos_predictionLine_LT.x, (int32_t)pos_predictionLine_LT.y, (int32_t)pos_predictionLine_RT.x, (int32_t)pos_predictionLine_RT.y, Color::RED, 2); // �㉡

		//���`��
		/*arrow_.Object::SetScale({ kPngScale_ * 3.0f, kPngScale_ * 5.0f,0 });
		arrow_.Object::SetTrans({ position_.x,position_.y,0 });
		arrow_.Object::SetRot({ 0,0,rotation_ });
		arrow_.DrawBoxSprite(nullptr, png_arrow_, { 1.0f,0.2f,0.3f,0.5f }, { 0.5f,0.5f });*/

		skewerArea_.Object::SetTrans({ position_.x,position_.y,0 });
		skewerArea_.Object::SetRot({ 0,0,rotation_ });
		skewerArea_.DrawBoxSprite(nullptr, png_skewerArea_, areaColor_, { 0.5f,1.f });
		//DrawFormatString(1000, 60, UtilL::Color::GREEN, "���ߏ��");
		//DrawFormatString(1000, 80, UtilL::Color::GREEN, "frame: %d/%d", frameCount_4Skewer_, kChargeFrame4Skewer_);
	}
	else if (state_ != State::ATTACK_SKEWER) // ���h���U���̂��߂ɗ��߂Ă�Ԃ�A���h���U�����͔��~��\�����Ȃ� ������ȊO�̎��ɕ\��
	{
		// �U���͈͂�debug�̕\��
#pragma region �ガ�����U���͈̔͂��ړ������Ă�
		mow_.SetPos(position_);
		mow_.SetRot(rotation_);
#pragma endregion
		mow_.DrawArea(areaColor_);

		// ���̍��W
		Vec2 pos_arrow = position_ + vec_move_ * kMowArrowDist2Self_;

		arrow_.Object::SetScale({ kMowArrowXRate_, kMowArrowYRate_,0 });
		arrow_.Object::SetTrans({ pos_arrow.x, pos_arrow.y,0 });
		arrow_.Object::SetRot({ 0,0,rotation_ });

		// �v���C���[�̈ړ��x�N�g����0��������(�X�^�[�g���΍�j
		if (vec_move_.GetLength() == 0)
		{
			// ���̍��W�����ߑł��Ō��߂�B�i���͏�����Ԃ��ƉE�����Ă�̂ŁA�E�����ɂ��߂����j
			pos_arrow = position_ + Vec2(1, 0) * kMowArrowDist2Self_;
			arrow_.Object::SetTrans({ pos_arrow.x, pos_arrow.y,0 });
			arrow_.DrawBoxSprite(nullptr, png_arrow_, areaColor_, { 0.5f,0.5f });
		}
		else // �ړ��x�N�g����0����Ȃ�������B
		{
			arrow_.DrawBoxSprite(nullptr, png_arrow_, areaColor_, { 0.5f,0.5f });
		}
	}

	// skewer�ׂ̈Ƀ{�^�����������ĂȂ� && ���h���U�����łȂ� && �ガ�����U�����Ȃ�
	// �ガ������� && �ガ�����t���[���J�E���g��0�ȊO
	if (state_ == State::ATTACK_MOW && mow_.GetFrameCountAttack() > 1)
	{
		// ����`��
		sword_.Object::SetScale({ 0.18f, 0.18f,0 });
		sword_.Object::SetTrans({ pos4Sword_.x, pos4Sword_.y,0 });
		sword_.Object::SetRot({ 0,0,rot4Sword2_ });
		sword_.DrawBoxSprite(nullptr, png_sword_, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f });
		//DrawCircle((int32_t)pos4Sword_.x, (int32_t)pos4Sword_.y, 1, UtilL::Color::BLUE, true, 1);
	}
	//DrawFormatString(1000, 120, UtilL::Color::GREEN, "rot4s: %f", rot4RotationSword_);

	if (state_ == State::ATTACK_SKEWER) // ���h���U�����A���h���̕`��֐����Ăяo��
	{
		// �q�b�g�X�g�b�v�|���ĂȂ����Ȃ�A���W���Z����
		if (frameCount_SkewerEndHitStop_ == 0)
		{
			pos4Sword_ += vec_move_ * 5;
			pos4SwordUp_ += vec_move_ * (6 + EnemyManager::GetInstance().GetSkewerEnemiesLength());
			pos4SwordBottom_ -= vec_move_ * 12;
			//�ːi�p�[�e�B�N��
			ParticleManagerL::GetInstance()->SkewerEffect(position_, -vec_move_);

			dist_p4su2p4sb_ = EnemyManager::GetInstance().GetSkewerEnemiesLength() - 18;
		}

		// ��
		sword_.Object::SetScale({ 0.18f, 0.18f,0 });
		sword_.Object::SetTrans({ pos4Sword_.x, pos4Sword_.y,0 });
		sword_.Object::SetRot({ 0,0,rotation_ });
		sword_.DrawBoxSprite(nullptr, png_sword_, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f });


		swordUp_.Object::SetScale({ 0.18f, 0.18f,0 });
		swordUp_.Object::SetTrans({ pos4SwordUp_.x, pos4SwordUp_.y,0 });
		swordUp_.Object::SetRot({ 0,0,rotation_ });
		swordUp_.DrawBoxSprite(nullptr, png_swordUp_, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f });

		swordBottom_.Object::SetScale({ 0.18f, 0.18f,0 });
		swordBottom_.Object::SetTrans({ pos4SwordBottom_.x, pos4SwordBottom_.y,0 });
		swordBottom_.Object::SetRot({ 0,0,rotation_ });
		swordBottom_.DrawBoxSprite(nullptr, png_swordBottom_, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f });
		skewer_.Draw();
	}

	// ���G���Ԓ��Ȃ�
	if (((frameCount_invincible_) / 5) % 2 == 1)
	{
		//DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Color::YELLOW, true, 1);

		Object::SetScale({ kPngScale_, kPngScale_,0 });
		Object::SetTrans({ position_.x, position_.y,0 });
		Object::SetRot({ 0,0,rotation_ });
		DrawBoxSprite(nullptr, png_player_, { 0.0f,0.0f,0.0f,1.0f }, { 0.5f,0.5f });
		areaColor_ = { 0.f,0.f,0.f,0.5f };
		//DrawFormatString(1000, 20, UtilL::Color::YELLOW, "���G���");
		//DrawFormatString(1000, 40, UtilL::Color::YELLOW, "frame: %d", kMaxInvincibleFrame_ - frameCount_invincible_);
	}
	else // ���G���Ԃ���Ȃ��Ȃ�
	{
		//DrawFormatString(1000, 20, UtilL::Color::WHITE, "�ʏ���");
		//DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Color::WHITE, true, 1);

		// player�̕`��
		Object::SetScale({ kPngScale_, kPngScale_,0 });
		Object::SetTrans({ position_.x, position_.y,0 });
		Object::SetRot({ 0,0,rotation_ });
		DrawBoxSprite(nullptr, png_player_, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f });
		areaColor_ = { 1.0f,0.2f,0.3f,0.5f };
	}

	// ���h���U�����̔�����W
	//DrawFormatString(1000, 100, UtilL::Color::GREEN, "pos(%f,%f)", skewer_.GetPos().x, skewer_.GetPos().y);
	//DrawFormatString(0, 500, UtilL::Color::WHITE, "pos(%f,%f)", position_.x, position_.y);
}

void Player::ResetSkewerInfo4Pause(void)
{
	// �������u�Ԃɏ�����
	frameCount_4Skewer_ = 0;
	isSkewerScreenBlack4SceneM_ = false;

	// �X���[���[�V��������
	// �X���[���[�V�����؂�z
	//SceneManager::GetInstance()->EndSlowMotion();
}

void Player::MoveUpdate(void)
{

	// ���h���I����̏k�݃t���O�I���Ȃ�
	if (isSkewerEndShrink_)
	{
		// �t���[���J�E���^���K��l�ȏ�Ȃ�I��
		if (frameCount_SkewerEndShrink_ > kMaxFrameSkewerEndShrink_)
		{
			isSkewerEndShrink_ = false;
			frameCount_SkewerEndShrink_ = 0;
		}
		else // �K��l�����Ȃ���Z
		{
			float rate = (std::min)((float)frameCount_SkewerEndShrink_ / kMaxFrameSkewerEndShrink_, 1.f);
			position_ = position_ + vec_move_ * Math::Ease::EaseInSine(rate) * dist_p4su2p4sb_;

			// �t���[�����Z
			frameCount_SkewerEndShrink_++;
		}

	}
	else // �k�ݏ������Ă�Ԃ͓����Ȃ�
	{
		// ����
		Vec2 input{};
		input += PadInput::GetInstance().GetLeftStickTilt();
#ifdef _DEBUG
		input.x += KeyboardInput::GetInstance().KeyPush(DIK_D) - KeyboardInput::GetInstance().KeyPush(DIK_A);
		input.y += KeyboardInput::GetInstance().KeyPush(DIK_S) - KeyboardInput::GetInstance().KeyPush(DIK_W);
#endif // _DEBUG

		// ���͂����������̂݁A�x�N�g�����L�^
		if (input.GetLength())
		{
			// �ړ������x�N�g�����L�^
			vec_move_ = input.GetNormalize();
		}

		//�@pad-A�������Ă��Ȃ����͈ړ��ł���B�i���h���ׂ̈ɗ��߂Ă鎞�͓����Ȃ��j
		if (frameCount_4Skewer_ == 0) // ���h���J�E���g��0�Ȃ�i=���߂ĂȂ�)
		{
			// ���W += (���K�����ꂽ���͒l * ���x)
			position_ += input.GetNormalize() * kMoveSpeed_;

			// �����߂����[���A����ȏア���Ȃ��悤�ɂ��Ă���
			position_.x = Math::Function::Clamp<float>(position_.x, stagePtr_->GetLT().x + radius_.x * 2, stagePtr_->GetRB().x - radius_.x * 2);
			position_.y = Math::Function::Clamp<float>(position_.y, stagePtr_->GetLT().y + radius_.x * 2, stagePtr_->GetRB().y - radius_.x * 2);
		}

#pragma region �ガ�����U���͈̔͂��ړ������Ă�
		mow_.SetPos(position_);
		mow_.SetRot(rotation_);
#pragma endregion


		// ���G������Ȃ���΍U���ł���
		if (frameCount_invincible_ == 0)
		{
			PostEffectManager::GetInstance().GetPostEffect1()->effectFlags_.isRGBShift = false;

			// pad-A�����ĂȂ��� && pad-R||RB ��Attack_MOW��ԂɑJ��
			if (PadInput::GetInstance().GetPushButton(GAMEPAD_A) == false && PadInput::GetInstance().GetReleaseTrigger(GAMEPAD_RIGHT_SHOULDER))
			{
				Sound::GetInstance().PlayWave("attack_SE.wav", 0.2f);
				mow_.Attack(vec_move_, position_);
				state_ = State::ATTACK_MOW;
			}

			// pad-A��������ATTACK_SKEWER��ԂɑJ��
			if (PadInput::GetInstance().GetPushButton(GAMEPAD_A))
			{
				if (!GameVelocityManager::GetInstance().GetIsSlowMotion())
				{
					// �X���[���[�V�����J�n
					GameVelocityManager::GetInstance().BeginSlowMotion(30, 0.1f);
				}

				// ATTACK_SKEWER��Ԃɓ��邽�߂̗��ߌv���t���[�������Z
				//frameCount_4Skewer_++;
				frameCount_4Skewer_ += 5; // �X���[���[�V��������̂��ߗ͋Z������5�t���[�������J�E���g���܂��B

				// ���d�l�㉟���Ă���X���[���[�V�����J�n�ɂȂ�̂ŁA�ŏ��̃t���[�����J�E���g�� +n ����Ă��܂��̂��ȒP�ɉ��������@�v�����܂���B

				isSkewerScreenBlack4SceneM_ = true;
			}
			else if (PadInput::GetInstance().GetReleaseTrigger(GAMEPAD_A))
			{
				GameVelocityManager::GetInstance().EndSlowMotion(30, 1.0f);
				Sound::GetInstance().PlayWave("skewer_SE.wav", 0.2f);
			}
			else
			{
				// �K��t���[���ȏ�G��Ă���J��
				if (frameCount_4Skewer_ >= kChargeFrame4Skewer_)
				{
					// �J�ڂ��ď�����
					skewer_.Attack();
					state_ = State::ATTACK_SKEWER;
					frameCount_4Skewer_ = 0;
					pos4Sword_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;
					pos4SwordUp_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;
					pos4SwordBottom_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;
				}
				// �������u�Ԃɏ�����
				frameCount_4Skewer_ = 0;
				isSkewerScreenBlack4SceneM_ = false;

			}
		}
		else
		{
			PostEffectManager::GetInstance().GetPostEffect1()->effectFlags_.RGBShiftPow = GetRand(0.005f, 0.015f)
				* (1.0f - (float)frameCount_invincible_ / (float)kMaxInvincibleFrame_);
		}
	}
	//// ���h���I����̃q�b�g�X�g�b�v
	//if (frameCount_SkewerEndHitStop_)
	//{
	//    // �t���[�����Z
	//    SceneManager::GetInstance()->EndSlowMotion();
	//    frameCount_SkewerEndHitStop_ = 0;
	//}

#ifdef _DEBUG
	// key-SPACE��Attack_MOW��ԂɑJ��
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE))
	{
		Sound::GetInstance().PlayWave("attack_SE.wav", 0.2f);
		mow_.Attack(vec_move_, position_);
		state_ = State::ATTACK_MOW;
	}
#endif // _DEBUG

	//�J������Ǐ]������
	CameraManager::GetInstance().GetCamera2D()->SetPos(position_);
}

void Player::MowAttackUpdate(void)
{
	//// ���h���I����̃q�b�g�X�g�b�v
	//if (frameCount_SkewerEndHitStop_)
	//{
	//    SceneManager::GetInstance()->EndSlowMotion();
	//}

	//�v���C���[�̑O�����~���ɂ���G�𐁂���΂��d�l
	// �����̂��߁A�v���C���[�̑O���ɒ����`�̓����蔻����o���āA���~��̓����蔻��ɂ������������Ă��𐁂���΂�

	// �U������̃t���[����0�ɂȂ�����
	if (mow_.GetFrameCountAttack() == 0)
	{
		// ��ԑJ��
		state_ = State::MOVE;
		// �֐��I��
		return;
	}

	// �t���[�����Z�Ői�s�������Z�o
	float rate = (std::min)((float)(mow_.GetFrameCountAttack() - 1) / PlayerMowAttack::kMaxAttackFrame_, 1.f);
	// �p�x�ō��ǂ̂��炢�����Ă͂߂� rad = ToRad(���� * 180��)
	rot4RotationSword_ = Math::Function::ToRadian(kMaxRangeSwordDegree_ * rate);

	// �v���C���[�̉E�������o��
	Vec3 vec3_move = { vec_move_.x,vec_move_.y,0 };
	Vec3 vec3_right = Vec3(0, 0, 1).Cross(vec3_move.GetNormalized());
	Vec2 vec2_right = { vec3_right.x,vec3_right.y };
	// �������W = �E���� * �K�苗��
	const Vec2 initPos = vec2_right * kMowSwordCenterDist_; // ��]���̏������W
	// ��]�ړ��̍��W�v�Z
	pos4Sword_.x = -(initPos.x * std::cos(rot4RotationSword_) - initPos.y * std::sin(rot4RotationSword_));
	pos4Sword_.y = -(initPos.x * std::sin(rot4RotationSword_) + initPos.y * std::cos(rot4RotationSword_));
	// ��]��v���C���[�̈ʒu�܂ňړ�������B
	pos4Sword_.x += position_.x;
	pos4Sword_.y += position_.y;

	// ���̊G���̂̉�]�p���v�Z����
	rot4Sword2_ = (rotation_ - Math::Function::ToRadian(90)) + rate * Math::Function::ToRadian(kMaxRangeSwordDegree_); // �p�x���傢�[�߂�

	// �ガ�����U���{�̂�Update()
	mow_.Update();
}

void Player::SkewerAttackUpdate(void)
{
	CameraManager::GetInstance().GetCamera2D()->BeginFollow(50.0f);
	PostEffectManager::GetInstance().GetPostEffect1()->effectFlags_.isRadialBlur = true;

	// isSkewer��false�Ȃ�MOVE��Ԃ֑J��
	if (skewer_.GetIsSkewer() == false)
	{
		if (PadInput::GetInstance().GetPushButton(GAMEPAD_A)
			&& !GameVelocityManager::GetInstance().GetIsSlowMotion())
		{
			// �X���[���[�V�����J�n
			GameVelocityManager::GetInstance().BeginSlowMotion(30, 0.1f);
		}
		PostEffectManager::GetInstance().GetPostEffect1()->effectFlags_.isRadialBlur = false;
		CameraManager::GetInstance().GetCamera2D()->EndFollow();
		state_ = State::MOVE;
		// ���肪���̏�Ɏc�葱�����Ⴄ����A��΂Ɉ���������Ȃ����W�ɓ]�����邲�艟���B pos(-10��,-10��)
		skewer_.SetPos({ -100000.f, -100000.f });
		// �����A�قڊm���ɒʂ�Ǝv������K��t���[����(�q�b�g�X�g�b�v��j�ɒʂ�Ǝv����ŁA�X���[���[�V�����I�������ăq�b�g�X�g�b�v���I��
		if (frameCount_SkewerEndHitStop_)
		{
			frameCount_SkewerEndHitStop_ = 0;
			isSkewerEndShrink_ = true;
		}
		// �֐��I��
		return;
	}

	// ���h���̏㔼����1�t���[����̍��W = 1�t���[���O�̏㔼���̍��W + (���K�����ꂽ�v���C���[�̌��� * ���x)
	Vec2 skewerd_swordUp_pos = pos4SwordUp_ /*+ vec_move_ * skewer_.GetKMoveSpeed()*/;

	// ���h��1�t���[����̍��W = ���W + (���K�����ꂽ�v���C���[�̌��� * ���x)
	Vec2 skewered_pos = position_ + vec_move_ * skewer_.GetKMoveSpeed();

	// ���h���̏㔼����1�t���[����̍��W (+ ���a)���A�X�e�[�W�̓����Ȃ�v���C���[�̍��W���X�V
	if (skewerd_swordUp_pos.x - skewer_.GetRad().x > stagePtr_->GetLT().x && skewerd_swordUp_pos.y - skewer_.GetRad().y > stagePtr_->GetLT().y && // ���݁A���a�͉~�Ƃ���x�����g���Ă��Ȃ��̂ŁAy���g���Ă��Ȃ��͈̂Ӑ}�I
		skewerd_swordUp_pos.x + skewer_.GetRad().x < stagePtr_->GetRB().x && skewerd_swordUp_pos.y + skewer_.GetRad().y < stagePtr_->GetRB().y)   // [2023/09/10]�d�l���ς��A���̔���ʒu�Ŏ~�܂邩���f����̂ŁA��`�̔���������́Ay���g��
	{
		// ���f
		position_ = skewered_pos;
	}
	else // ���h��1�t���[����̍��W (+ ���a)���A�X�e�[�W�O�Ȃ���h����ԏI��
	{
		skewer_.End(); // isSkewer��false�ɂ���B

		CameraManager::GetInstance().GetCamera2D()->EndFollow();
		if (EnemyManager::GetInstance().GetSkewerEnemiesNum())
		{
			uint32_t hsTime = 20 + (uint32_t)(EnemyManager::GetInstance().GetSkewerEnemiesNum() * 0.2f);
			hsTime = Math::Function::Clamp<uint32_t>(hsTime, 20, 60);	//�����ɑ����Ȃ��悤����
			HitStopManager::GetInstance().BeginHitStop(hsTime);
		}

		if (EnemyManager::GetInstance().GetSkewerEnemiesLength())
		{
			//GameVelocityManager::GetInstance().BeginSlowMotion(30, 0.1f);
			frameCount_SkewerEndHitStop_++;
		}
		frameCount_postSkewerInvincible_++;

		//���j��
		Sound::GetInstance().PlayWave("wall_hit_SE.wav", 1.0f);
	}

	const float eRange = EnemyManager::GetInstance().GetSkewerEnemiesLength();

	// ���h��1�t���[����̍��W + (���K�����ꂽ�v���C���[�̌��� * (�K�苗�� + ���h���Ă�G��"���a"))
	skewer_.SetPos(skewered_pos + vec_move_ * (kSkewerAttackCenterDist_ + eRange));
	skewer_.Update();
	// ���h���G�̍��W = �v���C���[�̍��W + ���K�����ꂽ�v���C���[�̌��� * �K�苗��)
	pos4Sword_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;
	pos4SwordUp_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;
	pos4SwordBottom_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;


	//�J������x��ĒǏ]
	CameraManager::GetInstance().GetCamera2D()->SetPos(position_);
	CameraManager::GetInstance().GetCamera2D()->Update();
}

void Player::OnCollision(void)
{
	// �ڐG�Ώۂ̖��̂� enemy
	if (other_->GetId() == "enemy")
	{
		// �L���X�g����enemy�Ƃ��Ď擾
		Enemy* enemyPtr = static_cast<Enemy*>(other_);

		// �G����ł�Ȃ牟���߂��v���
		if (enemyPtr->GetIsAlive() == false) return;
		// ���h���U�����������߂��v���
		if (skewer_.GetIsSkewer()) return;
		// ���h���I����̓��ʂȖ��G���ԂȂ��Ȃ�X���[
		if (frameCount_postSkewerInvincible_) return;

		// �G���玩���܂ł̕����x�N�g��
		vec_contactE2P_ = (position_ - other_->GetPos()).GetNormalize();

		// ���G���Ԓ��łȂ��Ȃ�
		if (frameCount_invincible_ == 0)
		{
			PostEffectManager::GetInstance().GetPostEffect1()->effectFlags_.isRGBShift = true;

			CameraManager::GetInstance().GetCamera2D()->BeginShake(50, 8.0f);

			// �m�N�o���܁`��
			isKnockback_ = true;

			// ���G���Ԃɓ���
			frameCount_invincible_++;

			// ���������ɔ�e�����ꍇ���Z�b�g(?)
			if (frameCount_4Skewer_)
			{
				frameCount_4Skewer_ = 0;
				GameVelocityManager::GetInstance().EndSlowMotion(30, 1.0f);
			}

			Sound::GetInstance().PlayWave("enemy_hit_SE.wav", 0.5f);
			ParticleManagerL::GetInstance()->PlayerKnokEffect(position_, vec_contactE2P_);
		}
		else // ���G���Ԓ��Ȃ牟���߂�
		{
			// �����߂���̍��W = ���W + (���K�����ꂽ�����߂����� * ���x)
			Vec2 pushBacked_pos = position_ + vec_contactE2P_ * kPushBackDist_;

			// �m�b�N�o�b�N��̍��W (+ ���a)���A�X�e�[�W�̓����Ȃ���W���f
			if (pushBacked_pos.x - radius_.x > stagePtr_->GetLT().x && pushBacked_pos.y - radius_.x > stagePtr_->GetLT().y && // ���݁A���a�͉~�Ƃ���x�����g���Ă��Ȃ��̂�
				pushBacked_pos.x + radius_.x < stagePtr_->GetRB().x && pushBacked_pos.y + radius_.x < stagePtr_->GetRB().y)   // y���g���Ă��Ȃ��͈̂Ӑ}�I
			{
				// ���f
				position_ = pushBacked_pos;
			}
		}
	}
}
