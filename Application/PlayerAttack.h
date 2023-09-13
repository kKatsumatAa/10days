#pragma once
#include "IEntity.h"
#include "CollisionManger.h"
#include "TextureManager.h"

class PlayerMowAttack :
	public IEntity
{
public:
	// ’è”
	static const int32_t kMaxAttackFrame_{ 10 }; // UŒ‚”ÍˆÍ‚Ì—LŒøŠÔ(ƒtƒŒ[ƒ€)
	const float kBlewDist_{ 80.f }; // ‚«”ò‚Î‚·‹——£
	const Vec2 kRadius_{ 65,0 }; // UŒ‚”ÍˆÍ ¦Œ»İ”»’è‚ª‰~‚È‚Ì‚Åyg‚¢‚Ü‚¹‚ñ
	const float kPngScale_{ 0.08f }; // ‰æ‘œ‚ÌŠg‘å—¦

	// ŠÖ”
	PlayerMowAttack(CollisionManger* colMPtr);
	~PlayerMowAttack(void) override;

	void Attack(const Vec2& vec_move, const Vec2& attackRangeCenter);
	void Update(void) override;
	void Draw(void) override;

private:
    void OnCollision(void);

	// •Ï”
	CollisionManger* colMPtr_;

	int32_t frameCount_attack_; // UŒ‚”ÍˆÍ‚Ì”»’è—P—\(ƒtƒŒ[ƒ€)
	Vec2 vec_playerMove_;

	// resource
	uint64_t png_mowAttack_ = TextureManager::LoadGraph("attack_area.png");
	uint32_t attack_SE_ = 0;   //UŒ‚‰¹

    // “ã‚¬•¥‚¢‚ª“G‚É“–‚½‚Á‚½‚©‚Ç‚¤‚©
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

