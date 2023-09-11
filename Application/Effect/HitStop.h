#pragma once
#include<memory>

//ヒットストップ
class HitStopManager final
{
private:
	uint32_t timer_ = 0;
	uint32_t timerMax_ = 0;

private:
	HitStopManager() { ; }
	~HitStopManager() { ; }

public:
	HitStopManager& operator=(const HitStopManager& inst) = delete;
	HitStopManager(const HitStopManager& inst) = delete;

	static HitStopManager& GetInstance();

public:
	void Initialize();
	void Update();
	void DrawImGui();

public:
	void BeginHitStop(uint32_t time);

public:
	//止まっているかどうか
	bool GetIsStop() { return timer_ < timerMax_; }
	float GetTimeRate() { return (float)timer_ / (float)timerMax_; }
};