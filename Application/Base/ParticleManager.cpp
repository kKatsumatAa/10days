#include "ParticleManager.h"

void ParticleManagerL::BossHitEnemyEffect(const Vec2& pos)
{
	enemyDeadParticle.SetPos(pos.x, pos.y);
	Vec2 s = { 1.f, 1.f };
	enemyDeadParticle.SetScale(s);
	enemyDeadParticle.Add(20, 0.25f, 20.0f, 30.0f, { -4.0f,-4.0f }, { 4.0f,4.0f });
}

ParticleManagerL* ParticleManagerL::GetInstance()
{
	static ParticleManagerL instance;
	return &instance;
}

void ParticleManagerL::Init()
{
	enemyDeadParticle.Init();
}

void ParticleManagerL::Update(float velocity)
{
	enemyDeadParticle.Update(velocity);
}

void ParticleManagerL::Draw()
{
	enemyDeadParticle.DrawCircleParticle();
}

void ParticleManagerL::Reset()
{
}
