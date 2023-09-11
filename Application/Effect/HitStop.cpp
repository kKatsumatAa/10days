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
	timer_ = 0;
}

void HitStopManager::Update()
{
	if (timer_ <= timerMax_)
	{
		PostEffectManager::GetInstance().GetPostEffect2()->effectFlags_.barrelCurvePow = 1.0f - EaseIn(GetTimeRate());

		timer_++;
	}
	else
	{
		PostEffectManager::GetInstance().GetPostEffect2()->effectFlags_.isBarrelCurve = false;
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
	PostEffectManager::GetInstance().GetPostEffect2()->effectFlags_.isBarrelCurve = true;
}
