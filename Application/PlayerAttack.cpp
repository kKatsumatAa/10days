#include "PlayerAttack.h"
#include "UtilL.h"
#include "MathUtil.h"
#include "Vec3.h"

PlayerMowAttack::PlayerMowAttack(CollisionManger* colMPtr) : IEntity(nullptr), colMPtr_(colMPtr)
{
	// 衝突マネージャへの登録
	colMPtr->Register(this);

	// 形状設定
	shape_ = Shape::CIRCLE;
	// 名称設定
	id_ = "player_mowAttack";

	radius_ = kRadius_;
}

PlayerMowAttack::~PlayerMowAttack(void)
{
	// 登録の抹消
	onCollision_ = nullptr;
	colMPtr_->UnRegister(this);
}

void PlayerMowAttack::Attack(const Vec2& vec_move, const Vec2& attackRangeCenter)
{
	// playerの進行方向を取得
	vec_playerMove_ = vec_move;

	// 中心地点を取得
	position_ = attackRangeCenter;

	// 攻撃中でないなら
	if (frameCount_attack_ == 0)
	{
		// 攻撃開始
		frameCount_attack_++;
	}
}

void PlayerMowAttack::Update(void)
{
	// 攻撃中なら
	if (frameCount_attack_)
	{
		// 攻撃時間カウントフレームを加算
		Math::Function::LoopIncrement(frameCount_attack_, 0, kMaxAttackFrame_);
	}
}

void PlayerMowAttack::Draw(void)
{
	// 攻撃範囲を可視化。
	//DrawBox((int32_t)(position_.x - radius_.x), (int32_t)(position_.y - radius_.y), (int32_t)(position_.x + radius_.x), (int32_t)(position_.y + radius_.y), UtilL::Color::BLUE, true);
	//DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, UtilL::Color::BLUE, false, 1);
	////DrawFormatString((int32_t)position_.x - 5, (int32_t)position_.y - 70, UtilL::Color::YELLOW, state_dir_ == Direction::RIGHT ? "右" : "左");
	//DrawFormatString((int32_t)position_.x - 5, (int32_t)position_.y - 90, UtilL::Color::YELLOW, "%f",rotation_);

	//DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_, rotation_, png_mowAttack_, true);

	Object::SetScale({ kPngScale_, kPngScale_,0 });
	Object::SetTrans({ position_.x, position_.y,0 });
	Object::SetRot({ 0,0, rotation_ - Math::Function::ToRadian(90) });
	DrawBoxSprite(nullptr, png_mowAttack_, { 1.0f,1.0f,1.0f,0.4f }, { 0.5f,0.5f });
}
