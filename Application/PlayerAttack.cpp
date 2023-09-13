#include "PlayerAttack.h"
#include "UtilL.h"
#include "MathUtil.h"
#include "Vec3.h"

PlayerMowAttack::PlayerMowAttack(CollisionManger* colMPtr) : IEntity(nullptr), colMPtr_(colMPtr)
{
	// Õ“Ëƒ}ƒl[ƒWƒƒ‚Ö‚Ì“o˜^
	colMPtr->Register(this);

	// Œ`óÝ’è
	shape_ = Shape::CIRCLE;
	// –¼ÌÝ’è
	id_ = "player_mowAttack";

    // Õ“Ëcallback”½‰f
    onCollision_ = std::bind(&PlayerMowAttack::OnCollision, this);

	radius_ = kRadius_;
}

PlayerMowAttack::~PlayerMowAttack(void)
{
	// “o˜^‚Ì–•Á
	onCollision_ = nullptr;
	colMPtr_->UnRegister(this);
}

void PlayerMowAttack::Attack(const Vec2& vec_move, const Vec2& attackRangeCenter)
{
	// player‚Ìis•ûŒü‚ðŽæ“¾
	vec_playerMove_ = vec_move;

	// ’†S’n“_‚ðŽæ“¾
	position_ = attackRangeCenter;

	// UŒ‚’†‚Å‚È‚¢‚È‚ç
	if (frameCount_attack_ == 0)
	{
		// UŒ‚ŠJŽn
		frameCount_attack_++;
	}
}

void PlayerMowAttack::Update(void)
{
    // “ã‚¬•¥‚¢‚ª“G‚É“–‚½‚Á‚½”»’èitrigger)
    if (isHitMow_ && isHitMowOld_ == false)
    {
		Sound::GetInstance().PlayWave("attack_hit_SE.wav");
    }

	// UŒ‚’†‚È‚ç
	if (frameCount_attack_)
	{
		// UŒ‚ŽžŠÔƒJƒEƒ“ƒgƒtƒŒ[ƒ€‚ð‰ÁŽZ
		Math::Function::LoopIncrement(frameCount_attack_, 0, kMaxAttackFrame_);
	}

    isHitMowOld_ = isHitMow_;
    isHitMow_ = false;
}

void PlayerMowAttack::Draw(void)
{
	// UŒ‚”ÍˆÍ‚ð‰ÂŽ‹‰»B
	//DrawBox((int32_t)(position_.x - radius_.x), (int32_t)(position_.y - radius_.y), (int32_t)(position_.x + radius_.x), (int32_t)(position_.y + radius_.y), UtilL::Color::BLUE, true);
	//DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, UtilL::Color::BLUE, false, 1);
	////DrawFormatString((int32_t)position_.x - 5, (int32_t)position_.y - 70, UtilL::Color::YELLOW, state_dir_ == Direction::RIGHT ? "‰E" : "¶");
	//DrawFormatString((int32_t)position_.x - 5, (int32_t)position_.y - 90, UtilL::Color::YELLOW, "%f",rotation_);

	//DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_, rotation_, png_mowAttack_, true);

	Object::SetScale({ kPngScale_, kPngScale_,0 });
	Object::SetTrans({ position_.x, position_.y,0 });
	Object::SetRot({ 0,0, rotation_ - Math::Function::ToRadian(90) });
	DrawBoxSprite(nullptr, png_mowAttack_, { 1.0f,0.2f,0.3f,0.5f }, { 0.5f,0.5f });
}

void PlayerMowAttack::DrawArea(Vec4& color)
{
	Object::SetScale({ kPngScale_, kPngScale_,0 });
	Object::SetTrans({ position_.x, position_.y,0 });
	Object::SetRot({ 0,0, rotation_ - Math::Function::ToRadian(90) });
	DrawBoxSprite(nullptr, png_mowAttack_, color, { 0.5f,0.5f });
}

void PlayerMowAttack::OnCollision(void)
{
    // ÚG‘ÎÛ‚Ì–¼Ì‚ª enemy
    if (other_->GetId() == "enemy")
    {
        isHitMow_ = true;
    }
}
