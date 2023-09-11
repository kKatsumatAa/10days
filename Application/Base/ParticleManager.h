#pragma once
#include "Particle.h"
#include <list>

class ParticleManagerL final
{
private:
	ParticleManagerL() {};

private:
	// パーティクルのインスタンス
	Emitter enemyDeadParticle;		//敵死んだ時

public:
	//ボスに敵を当てた時
	void BossHitEnemyEffect(const Vec2& pos);

public:
	static ParticleManagerL* GetInstance();

	void Init();
	void Update(float velocity);
	void Draw();
	void Reset();
};