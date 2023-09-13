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


const float Player::kMowDist_{ 15.f };// 薙ぎ払いで吹き飛ばす距離 こっち変更するならenemy.hの割合も弄らないと瞬間移動になっちまう
bool Player::isSkewerScreenBlack4SceneM_{};

Player::Player(CollisionManger* colMPtr, Stage* stagePtr) : IEntity(stagePtr), mow_(colMPtr), skewer_(colMPtr), colMPtr_(colMPtr)
{
	// 衝突マネージャへの登録
	colMPtr->Register(this);

	// 形状設定
	shape_ = Shape::CIRCLE;
	// 名称設定
	id_ = "player";

	// 衝突callback反映
	onCollision_ = std::bind(&Player::OnCollision, this);

	// 判定がその場に残り続けちゃうから、絶対に引っかからない座標に転送するごり押し。 pos(-10万,-10万)
	skewer_.SetPos({ -100000.f, -100000.f });

	areaColor_ = { 1.0f,0.2f,0.3f,0.5f };
}

Player::~Player(void)
{
	// 登録の抹消
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
	// 無敵時間中なら
	if (frameCount_invincible_ != 0)
	{
		// 無敵時間のフレームカウントを加算
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

	// 吹き飛ばされるフラグオンだったら
	if (isKnockback_)
	{
		// イージング用のタイムレート
		float rate = (std::min)((float)frameCount_knockback_ / kMaxKnockbackFrame_, 1.f);

		// 吹き飛ばされる速さ を イージングで調整
		const float mowSpeed = (1 - Math::Ease::EaseInSine(rate)) * Player::kKnockbackDist_;
		// 座標に加算
		position_ += vec_contactE2P_ * mowSpeed;

		if (rate >= 1.f)
		{
			frameCount_knockback_ = 0;
			isKnockback_ = false;
			return;
		}

		// フレームカウント加算
		Math::Function::LoopIncrement<int32_t>(frameCount_knockback_, 0, kMaxKnockbackFrame_);
	}

	// デバッグライン用記録
	pos4Line_ = position_ + vec_move_ * 30;

    // 向きから回転角を取得
    rotation_ = std::acos(Vec2(0, -1).Dot(vec_move_));
    //反転しないように
    if (vec_move_.x < 0)
    {
        rotation_ = -rotation_;
    }
    // 右向き方向を取得
    isRight_ = Vec2(0, -1).Cross(vec_move_.GetNormalize()) > 0.f;

    uint32_t num = 0;
    if (EnemyManager::GetInstance().GetDefeatedEnemiesNum(num))
    {
        //突進で稼いだスコアの演出
        Score::GetInstance()->BeginOneSkewerEffect(0.4f, { 1250.f, 120.f }, 150);
    }
}

void Player::Draw(void)
{
	Object::SetScale({ radius_.x,radius_.x,0 });
	Object::SetTrans({ position_.x,position_.y,0 });
	Object::SetRot({ 0,0,rotation_ });

	// 描画
	//DrawLineAA(position_.x, position_.y, pos4Line_.x, pos4Line_.y, UtilL::Color::WHITE, 3);

	// 現在のプレイヤーの状態（数字のみ）
	//DrawFormatString(0, 140, 0xffffff, "state ;%d", static_cast<int32_t>(state_));

	// プレイヤーの右方向を出す
	Vec3 vec3_move = { vec_move_.x,vec_move_.y,0 };
	Vec3 vec3_right = Vec3(0, 0, 1).Cross(vec3_move.GetNormalized());
	Vec2 vec2_right = { vec3_right.x,vec3_right.y };

	// skewerの為にボタン長押ししてるなら
	if (frameCount_4Skewer_ > 0)
	{
		// box用変数
		//Vec2 frame_pos4PredictionBox_front = position_ - vec2_right * radius_.x;
		//Vec2 frame_pos4PredictionBox_Back = position_ + vec_move_ * 1000 + vec2_right * radius_.x;
		//Vec2 contents_pos4PredictionBox_front = position_ - vec2_right * (radius_.x - 1);
		//Vec2 contents_pos4PredictionBox_Back = position_ + vec_move_ * 1000 + vec2_right * (radius_.x - 1);

		// DxlibのDrawBox()は回転機能ないからムリです
		//DrawBox((int32_t)frame_pos4PredictionBox_front.x, (int32_t)frame_pos4PredictionBox_front.y, (int32_t)frame_pos4PredictionBox_Back.x, (int32_t)frame_pos4PredictionBox_Back.y, Color::RED,false);
		//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 30);
		//DrawBox((int32_t)contents_pos4PredictionBox_front.x, (int32_t)contents_pos4PredictionBox_front.y, (int32_t)contents_pos4PredictionBox_Back.x, (int32_t)contents_pos4PredictionBox_Back.y, Color::RED,true);
		//SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

		// line用変数
		Vec2 pos_predictionLine_LB = position_ - vec2_right * radius_.x;
		Vec2 pos_predictionLine_LT = position_ + vec_move_ * 1500 - vec2_right * radius_.x;
		Vec2 pos_predictionLine_RB = position_ + vec2_right * radius_.x;
		Vec2 pos_predictionLine_RT = position_ + vec_move_ * 1500 + vec2_right * radius_.x;

		// DrawLineで無理矢理やります
		//DrawLine((int32_t)pos_predictionLine_LB.x, (int32_t)pos_predictionLine_LB.y, (int32_t)pos_predictionLine_LT.x, (int32_t)pos_predictionLine_LT.y, Color::RED, 2); // 左縦
		//DrawLine((int32_t)pos_predictionLine_RB.x, (int32_t)pos_predictionLine_RB.y, (int32_t)pos_predictionLine_RT.x, (int32_t)pos_predictionLine_RT.y, Color::RED, 2); // 右縦
		//DrawLine((int32_t)pos_predictionLine_LB.x, (int32_t)pos_predictionLine_LB.y, (int32_t)pos_predictionLine_RB.x, (int32_t)pos_predictionLine_RB.y, Color::RED, 2); // 下横
		//DrawLine((int32_t)pos_predictionLine_LT.x, (int32_t)pos_predictionLine_LT.y, (int32_t)pos_predictionLine_RT.x, (int32_t)pos_predictionLine_RT.y, Color::RED, 2); // 上横

		//矢印描画
		/*arrow_.Object::SetScale({ kPngScale_ * 3.0f, kPngScale_ * 5.0f,0 });
		arrow_.Object::SetTrans({ position_.x,position_.y,0 });
		arrow_.Object::SetRot({ 0,0,rotation_ });
		arrow_.DrawBoxSprite(nullptr, png_arrow_, { 1.0f,0.2f,0.3f,0.5f }, { 0.5f,0.5f });*/

		skewerArea_.Object::SetTrans({ position_.x,position_.y,0 });
		skewerArea_.Object::SetRot({ 0,0,rotation_ });
		skewerArea_.DrawBoxSprite(nullptr, png_skewerArea_, areaColor_, { 0.5f,1.f });
		//DrawFormatString(1000, 60, UtilL::Color::GREEN, "溜め状態");
		//DrawFormatString(1000, 80, UtilL::Color::GREEN, "frame: %d/%d", frameCount_4Skewer_, kChargeFrame4Skewer_);
	}
	else if (state_ != State::ATTACK_SKEWER) // 串刺し攻撃のために溜めてる間や、串刺し攻撃中は半円を表示しない ※それ以外の時に表示
	{
		// 攻撃範囲とdebugの表示
#pragma region 薙ぎ払い攻撃の範囲を移動させてる
		mow_.SetPos(position_);
		mow_.SetRot(rotation_);
#pragma endregion
		mow_.DrawArea(areaColor_);

		// 矢印の座標
		Vec2 pos_arrow = position_ + vec_move_ * kMowArrowDist2Self_;

		arrow_.Object::SetScale({ kMowArrowXRate_, kMowArrowYRate_,0 });
		arrow_.Object::SetTrans({ pos_arrow.x, pos_arrow.y,0 });
		arrow_.Object::SetRot({ 0,0,rotation_ });

		// プレイヤーの移動ベクトルが0だったら(スタート時対策）
		if (vec_move_.GetLength() == 0)
		{
			// 矢印の座標を決め打ちで決める。（今は初期状態だと右向いてるので、右方向にきめうち）
			pos_arrow = position_ + Vec2(1, 0) * kMowArrowDist2Self_;
			arrow_.Object::SetTrans({ pos_arrow.x, pos_arrow.y,0 });
			arrow_.DrawBoxSprite(nullptr, png_arrow_, areaColor_, { 0.5f,0.5f });
		}
		else // 移動ベクトルが0じゃなかったら。
		{
			arrow_.DrawBoxSprite(nullptr, png_arrow_, areaColor_, { 0.5f,0.5f });
		}
	}

	// skewerの為にボタン長押ししてない && 串刺し攻撃中でない && 薙ぎ払い攻撃中なら
	// 薙ぎ払い状態 && 薙ぎ払いフレームカウントが0以外
	if (state_ == State::ATTACK_MOW && mow_.GetFrameCountAttack() > 1)
	{
		// 串を描画
		sword_.Object::SetScale({ 0.18f, 0.18f,0 });
		sword_.Object::SetTrans({ pos4Sword_.x, pos4Sword_.y,0 });
		sword_.Object::SetRot({ 0,0,rot4Sword2_ });
		sword_.DrawBoxSprite(nullptr, png_sword_, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f });
		//DrawCircle((int32_t)pos4Sword_.x, (int32_t)pos4Sword_.y, 1, UtilL::Color::BLUE, true, 1);
	}
	//DrawFormatString(1000, 120, UtilL::Color::GREEN, "rot4s: %f", rot4RotationSword_);

	if (state_ == State::ATTACK_SKEWER) // 串刺し攻撃中、串刺しの描画関数を呼び出す
	{
		// ヒットストップ掛けてない時なら、座標加算して
		if (frameCount_SkewerEndHitStop_ == 0)
		{
			pos4Sword_ += vec_move_ * 5;
			pos4SwordUp_ += vec_move_ * (6 + EnemyManager::GetInstance().GetSkewerEnemiesLength());
			pos4SwordBottom_ -= vec_move_ * 12;
			//突進パーティクル
			ParticleManagerL::GetInstance()->SkewerEffect(position_, -vec_move_);

			dist_p4su2p4sb_ = EnemyManager::GetInstance().GetSkewerEnemiesLength() - 18;
		}

		// 串
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

	// 無敵時間中なら
	if (((frameCount_invincible_) / 5) % 2 == 1)
	{
		//DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Color::YELLOW, true, 1);

		Object::SetScale({ kPngScale_, kPngScale_,0 });
		Object::SetTrans({ position_.x, position_.y,0 });
		Object::SetRot({ 0,0,rotation_ });
		DrawBoxSprite(nullptr, png_player_, { 0.0f,0.0f,0.0f,1.0f }, { 0.5f,0.5f });
		areaColor_ = { 0.f,0.f,0.f,0.5f };
		//DrawFormatString(1000, 20, UtilL::Color::YELLOW, "無敵状態");
		//DrawFormatString(1000, 40, UtilL::Color::YELLOW, "frame: %d", kMaxInvincibleFrame_ - frameCount_invincible_);
	}
	else // 無敵時間じゃないなら
	{
		//DrawFormatString(1000, 20, UtilL::Color::WHITE, "通常状態");
		//DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Color::WHITE, true, 1);

		// playerの描画
		Object::SetScale({ kPngScale_, kPngScale_,0 });
		Object::SetTrans({ position_.x, position_.y,0 });
		Object::SetRot({ 0,0,rotation_ });
		DrawBoxSprite(nullptr, png_player_, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f });
		areaColor_ = { 1.0f,0.2f,0.3f,0.5f };
	}

	// 串刺し攻撃時の判定座標
	//DrawFormatString(1000, 100, UtilL::Color::GREEN, "pos(%f,%f)", skewer_.GetPos().x, skewer_.GetPos().y);
	//DrawFormatString(0, 500, UtilL::Color::WHITE, "pos(%f,%f)", position_.x, position_.y);
}

void Player::ResetSkewerInfo4Pause(void)
{
	// 離した瞬間に初期化
	frameCount_4Skewer_ = 0;
	isSkewerScreenBlack4SceneM_ = false;

	// スローモーション解除
	// スローモーション切る奴
	//SceneManager::GetInstance()->EndSlowMotion();
}

void Player::MoveUpdate(void)
{

	// 串刺し終了後の縮みフラグオンなら
	if (isSkewerEndShrink_)
	{
		// フレームカウンタが規定値以上なら終了
		if (frameCount_SkewerEndShrink_ > kMaxFrameSkewerEndShrink_)
		{
			isSkewerEndShrink_ = false;
			frameCount_SkewerEndShrink_ = 0;
		}
		else // 規定値未満なら加算
		{
			float rate = (std::min)((float)frameCount_SkewerEndShrink_ / kMaxFrameSkewerEndShrink_, 1.f);
			position_ = position_ + vec_move_ * Math::Ease::EaseInSine(rate) * dist_p4su2p4sb_;

			// フレーム加算
			frameCount_SkewerEndShrink_++;
		}

	}
	else // 縮み処理してる間は動けない
	{
		// 入力
		Vec2 input{};
		input += PadInput::GetInstance().GetLeftStickTilt();
#ifdef _DEBUG
		input.x += KeyboardInput::GetInstance().KeyPush(DIK_D) - KeyboardInput::GetInstance().KeyPush(DIK_A);
		input.y += KeyboardInput::GetInstance().KeyPush(DIK_S) - KeyboardInput::GetInstance().KeyPush(DIK_W);
#endif // _DEBUG

		// 入力があった時のみ、ベクトルを記録
		if (input.GetLength())
		{
			// 移動方向ベクトルを記録
			vec_move_ = input.GetNormalize();
		}

		//　pad-Aを押していない時は移動できる。（串刺しの為に溜めてる時は動けない）
		if (frameCount_4Skewer_ == 0) // 串刺しカウントが0なら（=溜めてない)
		{
			// 座標 += (正規化された入力値 * 速度)
			position_ += input.GetNormalize() * kMoveSpeed_;

			// 押し戻しっつーか、それ以上いかないようにってだけ
			position_.x = Math::Function::Clamp<float>(position_.x, stagePtr_->GetLT().x + radius_.x * 2, stagePtr_->GetRB().x - radius_.x * 2);
			position_.y = Math::Function::Clamp<float>(position_.y, stagePtr_->GetLT().y + radius_.x * 2, stagePtr_->GetRB().y - radius_.x * 2);
		}

#pragma region 薙ぎ払い攻撃の範囲を移動させてる
		mow_.SetPos(position_);
		mow_.SetRot(rotation_);
#pragma endregion


		// 無敵中じゃなければ攻撃できる
		if (frameCount_invincible_ == 0)
		{
			PostEffectManager::GetInstance().GetPostEffect1()->effectFlags_.isRGBShift = false;

			// pad-A押してない時 && pad-R||RB でAttack_MOW状態に遷移
			if (PadInput::GetInstance().GetPushButton(GAMEPAD_A) == false && PadInput::GetInstance().GetReleaseTrigger(GAMEPAD_RIGHT_SHOULDER))
			{
				Sound::GetInstance().PlayWave("attack_SE.wav", 0.2f);
				mow_.Attack(vec_move_, position_);
				state_ = State::ATTACK_MOW;
			}

			// pad-A長押しでATTACK_SKEWER状態に遷移
			if (PadInput::GetInstance().GetPushButton(GAMEPAD_A))
			{
				if (!GameVelocityManager::GetInstance().GetIsSlowMotion())
				{
					// スローモーション開始
					GameVelocityManager::GetInstance().BeginSlowMotion(30, 0.1f);
				}

				// ATTACK_SKEWER状態に入るための溜め計測フレームを加算
				//frameCount_4Skewer_++;
				frameCount_4Skewer_ += 5; // スローモーション回避のため力技だけど5フレーム分ずつカウントします。

				// ↑仕様上押してからスローモーション開始になるので、最初のフレーム分カウントが +n されてしまうのを簡単に回避する方法思いつきません。

				isSkewerScreenBlack4SceneM_ = true;
			}
			else if (PadInput::GetInstance().GetReleaseTrigger(GAMEPAD_A))
			{
				GameVelocityManager::GetInstance().EndSlowMotion(30, 1.0f);
				Sound::GetInstance().PlayWave("skewer_SE.wav", 0.2f);
			}
			else
			{
				// 規定フレーム以上触れてたら遷移
				if (frameCount_4Skewer_ >= kChargeFrame4Skewer_)
				{
					// 遷移して初期化
					skewer_.Attack();
					state_ = State::ATTACK_SKEWER;
					frameCount_4Skewer_ = 0;
					pos4Sword_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;
					pos4SwordUp_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;
					pos4SwordBottom_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;
				}
				// 離した瞬間に初期化
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
	//// 串刺し終了後のヒットストップ
	//if (frameCount_SkewerEndHitStop_)
	//{
	//    // フレーム加算
	//    SceneManager::GetInstance()->EndSlowMotion();
	//    frameCount_SkewerEndHitStop_ = 0;
	//}

#ifdef _DEBUG
	// key-SPACEでAttack_MOW状態に遷移
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE))
	{
		Sound::GetInstance().PlayWave("attack_SE.wav", 0.2f);
		mow_.Attack(vec_move_, position_);
		state_ = State::ATTACK_MOW;
	}
#endif // _DEBUG

	//カメラを追従させる
	CameraManager::GetInstance().GetCamera2D()->SetPos(position_);
}

void Player::MowAttackUpdate(void)
{
	//// 串刺し終了後のヒットストップ
	//if (frameCount_SkewerEndHitStop_)
	//{
	//    SceneManager::GetInstance()->EndSlowMotion();
	//}

	//プレイヤーの前方半円分にいる敵を吹き飛ばす仕様
	// 実現のため、プレイヤーの前方に長方形の当たり判定を出して、かつ円状の当たり判定にも引っかかってるやつを吹っ飛ばす

	// 攻撃判定のフレームが0になったら
	if (mow_.GetFrameCountAttack() == 0)
	{
		// 状態遷移
		state_ = State::MOVE;
		// 関数終了
		return;
	}

	// フレーム換算で進行割合を算出
	float rate = (std::min)((float)(mow_.GetFrameCountAttack() - 1) / PlayerMowAttack::kMaxAttackFrame_, 1.f);
	// 角度で今どのくらいか当てはめる rad = ToRad(割合 * 180°)
	rot4RotationSword_ = Math::Function::ToRadian(kMaxRangeSwordDegree_ * rate);

	// プレイヤーの右方向を出す
	Vec3 vec3_move = { vec_move_.x,vec_move_.y,0 };
	Vec3 vec3_right = Vec3(0, 0, 1).Cross(vec3_move.GetNormalized());
	Vec2 vec2_right = { vec3_right.x,vec3_right.y };
	// 初期座標 = 右方向 * 規定距離
	const Vec2 initPos = vec2_right * kMowSwordCenterDist_; // 回転時の初期座標
	// 回転移動の座標計算
	pos4Sword_.x = -(initPos.x * std::cos(rot4RotationSword_) - initPos.y * std::sin(rot4RotationSword_));
	pos4Sword_.y = -(initPos.x * std::sin(rot4RotationSword_) + initPos.y * std::cos(rot4RotationSword_));
	// 回転後プレイヤーの位置まで移動させる。
	pos4Sword_.x += position_.x;
	pos4Sword_.y += position_.y;

	// 串の絵自体の回転角を計算する
	rot4Sword2_ = (rotation_ - Math::Function::ToRadian(90)) + rate * Math::Function::ToRadian(kMaxRangeSwordDegree_); // 角度ちょい深めに

	// 薙ぎ払い攻撃本体のUpdate()
	mow_.Update();
}

void Player::SkewerAttackUpdate(void)
{
	CameraManager::GetInstance().GetCamera2D()->BeginFollow(50.0f);
	PostEffectManager::GetInstance().GetPostEffect1()->effectFlags_.isRadialBlur = true;

	// isSkewerがfalseならMOVE状態へ遷移
	if (skewer_.GetIsSkewer() == false)
	{
		if (PadInput::GetInstance().GetPushButton(GAMEPAD_A)
			&& !GameVelocityManager::GetInstance().GetIsSlowMotion())
		{
			// スローモーション開始
			GameVelocityManager::GetInstance().BeginSlowMotion(30, 0.1f);
		}
		PostEffectManager::GetInstance().GetPostEffect1()->effectFlags_.isRadialBlur = false;
		CameraManager::GetInstance().GetCamera2D()->EndFollow();
		state_ = State::MOVE;
		// 判定がその場に残り続けちゃうから、絶対に引っかからない座標に転送するごり押し。 pos(-10万,-10万)
		skewer_.SetPos({ -100000.f, -100000.f });
		// 多分、ほぼ確実に通ると思うから規定フレーム後(ヒットストップ後）に通ると思うんで、スローモーション終了させてヒットストップも終了
		if (frameCount_SkewerEndHitStop_)
		{
			frameCount_SkewerEndHitStop_ = 0;
			isSkewerEndShrink_ = true;
		}
		// 関数終了
		return;
	}

	// 串刺しの上半分の1フレーム後の座標 = 1フレーム前の上半分の座標 + (正規化されたプレイヤーの向き * 速度)
	Vec2 skewerd_swordUp_pos = pos4SwordUp_ /*+ vec_move_ * skewer_.GetKMoveSpeed()*/;

	// 串刺し1フレーム後の座標 = 座標 + (正規化されたプレイヤーの向き * 速度)
	Vec2 skewered_pos = position_ + vec_move_ * skewer_.GetKMoveSpeed();

	// 串刺しの上半分の1フレーム後の座標 (+ 半径)が、ステージの内側ならプレイヤーの座標を更新
	if (skewerd_swordUp_pos.x - skewer_.GetRad().x > stagePtr_->GetLT().x && skewerd_swordUp_pos.y - skewer_.GetRad().y > stagePtr_->GetLT().y && // 現在、半径は円としてxしか使っていないので、yが使われていないのは意図的
		skewerd_swordUp_pos.x + skewer_.GetRad().x < stagePtr_->GetRB().x && skewerd_swordUp_pos.y + skewer_.GetRad().y < stagePtr_->GetRB().y)   // [2023/09/10]仕様が変わり、串の判定位置で止まるか判断するので、矩形の判定をもつ串は、yも使う
	{
		// 反映
		position_ = skewered_pos;
	}
	else // 串刺し1フレーム後の座標 (+ 半径)が、ステージ外なら串刺し状態終了
	{
		skewer_.End(); // isSkewerをfalseにする。

		CameraManager::GetInstance().GetCamera2D()->EndFollow();
		if (EnemyManager::GetInstance().GetSkewerEnemiesNum())
		{
			uint32_t hsTime = 20 + (uint32_t)(EnemyManager::GetInstance().GetSkewerEnemiesNum() * 0.2f);
			hsTime = Math::Function::Clamp<uint32_t>(hsTime, 20, 60);	//無限に増えないよう制限
			HitStopManager::GetInstance().BeginHitStop(hsTime);
		}

		if (EnemyManager::GetInstance().GetSkewerEnemiesLength())
		{
			//GameVelocityManager::GetInstance().BeginSlowMotion(30, 0.1f);
			frameCount_SkewerEndHitStop_++;
		}
		frameCount_postSkewerInvincible_++;

		//撃破音
		Sound::GetInstance().PlayWave("wall_hit_SE.wav", 1.0f);
	}

	const float eRange = EnemyManager::GetInstance().GetSkewerEnemiesLength();

	// 串刺し1フレーム後の座標 + (正規化されたプレイヤーの向き * (規定距離 + 串刺してる敵の"直径"))
	skewer_.SetPos(skewered_pos + vec_move_ * (kSkewerAttackCenterDist_ + eRange));
	skewer_.Update();
	// 串刺し絵の座標 = プレイヤーの座標 + 正規化されたプレイヤーの向き * 規定距離)
	pos4Sword_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;
	pos4SwordUp_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;
	pos4SwordBottom_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;


	//カメラを遅れて追従
	CameraManager::GetInstance().GetCamera2D()->SetPos(position_);
	CameraManager::GetInstance().GetCamera2D()->Update();
}

void Player::OnCollision(void)
{
	// 接触対象の名称が enemy
	if (other_->GetId() == "enemy")
	{
		// キャストしてenemyとして取得
		Enemy* enemyPtr = static_cast<Enemy*>(other_);

		// 敵死んでんなら押し戻し要らん
		if (enemyPtr->GetIsAlive() == false) return;
		// 串刺し攻撃中も押し戻し要らん
		if (skewer_.GetIsSkewer()) return;
		// 串刺し終了後の特別な無敵時間ないならスルー
		if (frameCount_postSkewerInvincible_) return;

		// 敵から自分までの方向ベクトル
		vec_contactE2P_ = (position_ - other_->GetPos()).GetNormalize();

		// 無敵時間中でないなら
		if (frameCount_invincible_ == 0)
		{
			PostEffectManager::GetInstance().GetPostEffect1()->effectFlags_.isRGBShift = true;

			CameraManager::GetInstance().GetCamera2D()->BeginShake(50, 8.0f);

			// ノクバしま～す
			isKnockback_ = true;

			// 無敵時間に入る
			frameCount_invincible_++;

			// 長押し中に被弾した場合リセット(?)
			if (frameCount_4Skewer_)
			{
				frameCount_4Skewer_ = 0;
				GameVelocityManager::GetInstance().EndSlowMotion(30, 1.0f);
			}

			Sound::GetInstance().PlayWave("enemy_hit_SE.wav", 0.5f);
			ParticleManagerL::GetInstance()->PlayerKnokEffect(position_, vec_contactE2P_);
		}
		else // 無敵時間中なら押し戻し
		{
			// 押し戻し後の座標 = 座標 + (正規化された押し戻し方向 * 速度)
			Vec2 pushBacked_pos = position_ + vec_contactE2P_ * kPushBackDist_;

			// ノックバック後の座標 (+ 半径)が、ステージの内側なら座標反映
			if (pushBacked_pos.x - radius_.x > stagePtr_->GetLT().x && pushBacked_pos.y - radius_.x > stagePtr_->GetLT().y && // 現在、半径は円としてxしか使っていないので
				pushBacked_pos.x + radius_.x < stagePtr_->GetRB().x && pushBacked_pos.y + radius_.x < stagePtr_->GetRB().y)   // yが使われていないのは意図的
			{
				// 反映
				position_ = pushBacked_pos;
			}
		}
	}
}
