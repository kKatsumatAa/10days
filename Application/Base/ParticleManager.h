#pragma once
#include "Particle.h"
#include <list>

class ParticleManagerL final
{
private:
	ParticleManagerL() {};

private:
	// パーティクルのインスタンス
	Emitter enemyDeadParticle_;		//敵死んだ時
	Emitter skewerParticle_;		//突進時
	Emitter enemyHitParticle_;		//敵がなぎ払いに当たった時

	uint64_t texKey_;

public:
	//敵死んだ時
	void EnemyDeadEffect(const Vec2& pos);
	//突進時
	void SkewerEffect(const Vec2& pos, const Vec2& velo);
	//敵がなぎ払いに当たった時
	void EnemyHitEffect(const Vec2& pos, const Vec2& velo);

public:
	static ParticleManagerL* GetInstance();

	void Init();
	void Update(float velocity);
	void Draw();
	void Reset();
};