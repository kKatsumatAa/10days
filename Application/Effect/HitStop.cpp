#include "HitStop.h"
#include "PostEffectManager.h"
#include "ImGuiManager.h"
#include "Util.h"


HitStopManager& HitStopManager::GetInstance()
{
	static HitStopManager sInst;
	return sInst;
}


//--------------------------------------------------
void HitStopManager::Initialize()
{
	timer_ = 0;
	timerMax_ = 0;
	PostEffectManager::GetInstance().GetPostEffect1()->effectFlags_.isBarrelCurve = true;
}

void HitStopManager::Update()
{
	if (timer_ <= timerMax_)
	{
		PostEffectManager::GetInstance().GetPostEffect1()->effectFlags_.barrelCurvePow = min(1.0f - EaseIn(GetTimeRate()), MAX_EFFECT_POW_);

		timer_++;
	}
	else
	{
		PostEffectManager::GetInstance().GetPostEffect1()->effectFlags_.isBarrelCurve = false;
	}
}

void HitStopManager::DrawImGui()
{
	ImGui::Begin("HitStop");

	ImGui::Text("timer: %f", timer_);
	ImGui::Text("timerMax: %f", timerMax_);
	ImGui::Text("timeRate: %f", (float)timer_ / (float)timerMax_);

	ImGui::End();
}

//--------------------------------------------------
void HitStopManager::BeginHitStop(uint32_t time)
{
	timer_ = 0;
	timerMax_ = time;
	PostEffectManager::GetInstance().GetPostEffect1()->effectFlags_.isBarrelCurve = true;
}
