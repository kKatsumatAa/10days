#pragma once
#include "Particle.h"
#include <list>

class ParticleManagerL final
{
private:
	ParticleManagerL() {};

private:
	// �p�[�e�B�N���̃C���X�^���X
	Emitter enemyDeadParticle;		//�G���񂾎�

public:
	//�{�X�ɓG�𓖂Ă���
	void BossHitEnemyEffect(const Vec2& pos);

public:
	static ParticleManagerL* GetInstance();

	void Init();
	void Update(float velocity);
	void Draw();
	void Reset();
};