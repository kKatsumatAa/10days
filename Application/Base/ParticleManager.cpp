#include "ParticleManager.h"

void ParticleManagerL::EnemyDeadEffect(const Vec2& pos)
{
	enemyDeadParticle_.SetPos(pos.x, pos.y);
	Vec2 s = { 1.f, 1.f };
	enemyDeadParticle_.SetScale(s);
	enemyDeadParticle_.Add(40, 0.5f, 20.0f, 30.0f, { -5.0f,-5.0f }, { 5.0f,5.0f }, { 0,0 }, 0, 0, {0.75f,0.3f,0.5f,1.f});
}

void ParticleManagerL::SkewerEffect(const Vec2& pos, const Vec2& velo)
{
	skewerParticle_.SetPos(pos.x, pos.y);
	Vec2 s = { 1.f, 1.f };
	skewerParticle_.SetScale(s);
	skewerParticle_.Add(4, 0.25f, 20.0f, 30.0f,
		{ velo.x - 2.0f,velo.y - 2.0f }, { velo.x + 2.0f,velo.y + 2.0f },
		{ 0,0 }, 0, 0, { 1.f,1.f,1.f,0.5f });
}

ParticleManagerL* ParticleManagerL::GetInstance()
{
	static ParticleManagerL instance;
	return &instance;
}

void ParticleManagerL::Init()
{
	enemyDeadParticle_.Init();
	skewerParticle_.Init();
}

void ParticleManagerL::Update(float velocity)
{
	enemyDeadParticle_.Update(false,velocity);
	skewerParticle_.Update(false,velocity);
}

void ParticleManagerL::Draw()
{
	skewerParticle_.DrawCircleParticle();
	enemyDeadParticle_.DrawCircleParticle();
}

void ParticleManagerL::Reset()
{
}
