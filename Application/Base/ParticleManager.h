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

public:
	//ボスに敵を当てた時
	void EnemyDeadEffect(const Vec2& pos);
	//ボスに敵を当てた時
	void SkewerEffect(const Vec2& pos, const Vec2& velo);

public:
	static ParticleManagerL* GetInstance();

	void Init();
	void Update(float velocity);
	void Draw();
	void Reset();
};