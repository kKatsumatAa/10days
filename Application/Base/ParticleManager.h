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
	Emitter enemyHitParticle_;		//�G���Ȃ������ɓ���������

	uint64_t texKey_;

public:
	//�G���񂾎�
	void EnemyDeadEffect(const Vec2& pos);
	//�ːi��
	void SkewerEffect(const Vec2& pos, const Vec2& velo);
	//�G���Ȃ������ɓ���������
	void EnemyHitEffect(const Vec2& pos, const Vec2& velo);

public:
	static ParticleManagerL* GetInstance();

	void Init();
	void Update(float velocity);
	void Draw();
	void Reset();
};