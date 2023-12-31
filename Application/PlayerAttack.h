#pragma once
#include "IEntity.h"
#include "CollisionManger.h"
#include "TextureManager.h"

class PlayerMowAttack :
	public IEntity
{
public:
	// 定数
	static const int32_t kMaxAttackFrame_{ 10 }; // 攻撃範囲の有効時間(フレーム)
	const float kBlewDist_{ 80.f }; // 吹き飛ばす距離
	const Vec2 kRadius_{ 70,0 }; // 攻撃範囲 ※現在判定が円なのでy使いません
	const float kPngScale_{ 0.09f }; // 画像の拡大率

	// 関数
	PlayerMowAttack(CollisionManger* colMPtr);
	~PlayerMowAttack(void) override;

	void Attack(const Vec2& vec_move, const Vec2& attackRangeCenter);
	void Update(void) override;
	void Draw(void) override;
	void DrawArea(Vec4& color);

private:
    void OnCollision(void);

	// 変数
	CollisionManger* colMPtr_;

	int32_t frameCount_attack_; // 攻撃範囲の判定猶予(フレーム)
	Vec2 vec_playerMove_;

	// resource
	uint64_t png_mowAttack_ = TextureManager::LoadGraph("attack_area.png");
	uint32_t attack_SE_ = 0;   //攻撃音

    // 薙ぎ払いが敵に当たったかどうか
    bool isHitMow_;
    bool isHitMowOld_;

public:
	// setter

	//getter
	int32_t GetFrameCountAttack(void) { return frameCount_attack_; }
	const Vec2& GetVecMove(void) { return vec_playerMove_; }
	const Vec2& GetKRadius(void) { return kRadius_; }
	float GetKBlewDist(void) { return kBlewDist_; }

};

