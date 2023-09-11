#pragma once
#include "IEntity.h"
#include "CollisionManger.h"
#include "PlayerMowAttackSupport.h"
#include "TextureManager.h"

class PlayerMowAttack :
	public IEntity
{
public:
	// �萔
	static const int32_t kMaxAttackFrame_{ 10 }; // �U���͈̗͂L������(�t���[��)
	const float kBlewDist_{ 80.f }; // ������΂�����
	const Vec2 kRadius_{ 50,0 }; // �U���͈� �����ݔ��肪�~�Ȃ̂�y�g���܂���
	const float kPngScale_{ 0.06f }; // �摜�̊g�嗦

	// �֐�
	PlayerMowAttack(CollisionManger* colMPtr);
	~PlayerMowAttack(void) override;

	void Attack(const Vec2& vec_move, const Vec2& attackRangeCenter);
	void Update(void) override;
	void Draw(void) override;

private:

	// �ϐ�
	CollisionManger* colMPtr_;

	int32_t frameCount_attack_; // �U���͈͂̔���P�\(�t���[��)
	Vec2 vec_playerMove_;

	// resource
	uint64_t png_mowAttack_ = TextureManager::LoadGraph("attack_area.png");
	uint32_t attack_SE_ = 0;   //�U����

public:
	// setter

	//getter
	int32_t GetFrameCountAttack(void) { return frameCount_attack_; }
	const Vec2& GetVecMove(void) { return vec_playerMove_; }
	const Vec2& GetKRadius(void) { return kRadius_; }
	float GetKBlewDist(void) { return kBlewDist_; }

};

