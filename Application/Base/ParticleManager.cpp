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
	Vec2 v = velo.GetNormalize();
	skewerParticle_.SetPos(pos.x, pos.y);
	Vec2 s = { 1.f, 1.f };
	skewerParticle_.SetScale(s);
	skewerParticle_.Add(4, 0.25f, 20.0f, 30.0f,
		{ v.x - 2.0f,v.y - 2.0f }, { v.x + 2.0f,v.y + 2.0f },
		{ 0,0 }, 0, 0, { 1.f,1.f,1.f,0.5f });
}

void ParticleManagerL::EnemyHitEffect(const Vec2& pos, const Vec2& velo)
{
	Vec2 v = velo.GetNormalize();
	enemyHitParticle_.SetPos(pos.x, pos.y);
	Vec2 s = { 1.f, 1.f };
	enemyHitParticle_.SetScale(s);
	enemyHitParticle_.Add(20, 0.25f, 20.0f, 30.0f,
		{ v.x - 4.0f,v.y - 4.0f }, { v.x + 4.0f,v.y + 4.0f },
		v, 0, 0, { 0.75f,0.3f,0.5f,0.5f });
}

void ParticleManagerL::PlayerKnokEffect(const Vec2& pos, const Vec2& velo)
{
	Vec2 v = velo.GetNormalize();
	enemyHitParticle_.SetPos(pos.x, pos.y);
	Vec2 s = { 1.f, 1.f };
	enemyHitParticle_.SetScale(s);
	enemyHitParticle_.Add(30, 0.3f, 30.0f, 40.0f,
		{ v.x - 5.0f,v.y - 5.0f }, { v.x + 5.0f,v.y + 5.0f },
		v*1.2f, 0, 0, { 1.f,0.2f,0.3f,0.5f });
}

ParticleManagerL* ParticleManagerL::GetInstance()
{
	static ParticleManagerL instance;
	return &instance;
}

void ParticleManagerL::Init()
{
	texKey_ = TextureManager::LoadGraph("circle.png");

	enemyDeadParticle_.Init();
	skewerParticle_.Init();
	enemyHitParticle_.Init();

	enemyDeadParticle_.SetTextureKey(texKey_);
	skewerParticle_.SetTextureKey(texKey_);
	enemyHitParticle_.SetTextureKey(texKey_);
}

void ParticleManagerL::Update(float velocity)
{
	enemyDeadParticle_.Update(false,velocity);
	skewerParticle_.Update(false,velocity);
	enemyHitParticle_.Update(false,velocity);
}

void ParticleManagerL::Draw()
{
	skewerParticle_.DrawCircleParticle();
	enemyDeadParticle_.DrawCircleParticle();
	enemyHitParticle_.DrawCircleParticle();
}

void ParticleManagerL::Reset()
{
}
