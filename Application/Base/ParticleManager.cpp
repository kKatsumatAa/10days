#include "ParticleManager.h"

void ParticleManagerL::BossHitEnemyEffect(const Vec2& pos)
{
	enemyDeadParticle.SetPos(pos.x, pos.y);
	Vec2 s = { 1.f, 1.f };
	enemyDeadParticle.SetScale(s);
	enemyDeadParticle.Add(40, 0.5f, 20.0f, 30.0f, { -5.0f,-5.0f }, { 5.0f,5.0f }, { 0,0 }, 0, 0, {0.75f,0.3f,0.5f,1.f});
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
	enemyDeadParticle.Update(false,velocity);
}

void ParticleManagerL::Draw()
{
	enemyDeadParticle.DrawCircleParticle();
}

void ParticleManagerL::Reset()
{
}
