#include "PlayerAttack.h"
#include "UtilL.h"
#include "MathUtil.h"
#include "Vec3.h"

PlayerMowAttack::PlayerMowAttack(CollisionManger* colMPtr) : IEntity(nullptr), colMPtr_(colMPtr)
{
	// �Փ˃}�l�[�W���ւ̓o�^
	colMPtr->Register(this);

	// �`��ݒ�
	shape_ = Shape::CIRCLE;
	// ���̐ݒ�
	id_ = "player_mowAttack";

	radius_ = kRadius_;
}

PlayerMowAttack::~PlayerMowAttack(void)
{
	// �o�^�̖���
	onCollision_ = nullptr;
	colMPtr_->UnRegister(this);
}

void PlayerMowAttack::Attack(const Vec2& vec_move, const Vec2& attackRangeCenter)
{
	// player�̐i�s�������擾
	vec_playerMove_ = vec_move;

	// ���S�n�_���擾
	position_ = attackRangeCenter;

	// �U�����łȂ��Ȃ�
	if (frameCount_attack_ == 0)
	{
		// �U���J�n
		frameCount_attack_++;
	}
}

void PlayerMowAttack::Update(void)
{
	// �U�����Ȃ�
	if (frameCount_attack_)
	{
		// �U�����ԃJ�E���g�t���[�������Z
		Math::Function::LoopIncrement(frameCount_attack_, 0, kMaxAttackFrame_);
	}
}

void PlayerMowAttack::Draw(void)
{
	// �U���͈͂������B
	//DrawBox((int32_t)(position_.x - radius_.x), (int32_t)(position_.y - radius_.y), (int32_t)(position_.x + radius_.x), (int32_t)(position_.y + radius_.y), UtilL::Color::BLUE, true);
	//DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, UtilL::Color::BLUE, false, 1);
	////DrawFormatString((int32_t)position_.x - 5, (int32_t)position_.y - 70, UtilL::Color::YELLOW, state_dir_ == Direction::RIGHT ? "�E" : "��");
	//DrawFormatString((int32_t)position_.x - 5, (int32_t)position_.y - 90, UtilL::Color::YELLOW, "%f",rotation_);

	//DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_, rotation_, png_mowAttack_, true);

	Object::SetScale({ kPngScale_, kPngScale_,0 });
	Object::SetTrans({ position_.x, position_.y,0 });
	Object::SetRot({ 0,0, rotation_ - Math::Function::ToRadian(90) });
	DrawBoxSprite(nullptr, png_mowAttack_, { 1.0f,1.0f,1.0f,0.4f }, { 0.5f,0.5f });
}
