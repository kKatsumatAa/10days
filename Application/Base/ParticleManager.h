#pragma once
#include "Particle.h"
#include <list>

class ParticleManagerL final
{
private:
	ParticleManagerL() {};

private:
	// �p�[�e�B�N���̃C���X�^���X
	Emitter enemyDeadParticle_;		//�G���񂾎�
	Emitter skewerParticle_;		//�ːi��

public:
	//�{�X�ɓG�𓖂Ă���
	void EnemyDeadEffect(const Vec2& pos);
	//�{�X�ɓG�𓖂Ă���
	void SkewerEffect(const Vec2& pos, const Vec2& velo);

public:
	static ParticleManagerL* GetInstance();

	void Init();
	void Update(float velocity);
	void Draw();
	void Reset();
};