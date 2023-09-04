#include "GameVelocityManager.h"
#include "Util.h"
#include "ImGuiManager.h"
#include "PostEffectManager.h"
#include "GameVelocityState.h"


GameVelocityManager& GameVelocityManager::GetInstance()
{
	static GameVelocityManager sInst;
	return sInst;
}

void GameVelocityManager::Initialize()
{
	ChangeSlowState("NONE");
	gameVelocity_ = 1.0f;
}


//-------------------------------------------------------------------------
void GameVelocityManager::SetSlowMotionData(int32_t lerpFrame, float velocity, bool isSlowMotion)
{
	//���`�⊮�p�ɕۑ�
	slowData_.slowVelBegin = gameVelocity_;
	slowData_.slowVelEnd = velocity;

	slowData_.slowTimeMax = lerpFrame;

	slowData_.isSlowMotion = isSlowMotion;
}

void GameVelocityManager::BeginSlowMotion(int32_t lerpFrame, float endVelocity)
{
	//���`�⊮�p�ɕۑ�
	SetSlowMotionData(lerpFrame, endVelocity, true);

	ChangeSlowState("BEGIN");
}

void GameVelocityManager::EndSlowMotion(int32_t lerpFrame, float endVelocity)
{
	//���`�⊮�p�ɕۑ�
	SetSlowMotionData(lerpFrame, endVelocity, false);

	ChangeSlowState("END");
}

void GameVelocityManager::EndSlowMotion()
{
	//���`�⊮�p�ɕۑ�
	SetSlowMotionData(slowData_.slowTimeMax, slowData_.slowVelBegin, false);

	ChangeSlowState("END");
}

//-----------------------------------------------------------------------------------------
float GameVelocityManager::GetVelocity()
{
	return gameVelocity_;
}

//-----------------------------------------------------------------------------------------
void GameVelocityManager::Update()
{
	//�O�ڂ̐F��
	Vec3 col = LerpVec3(VEL_COL_MIN_, { 1.0f,1.0f,1.0f }, gameVelocity_);
	PostEffectManager::GetInstance().GetPostEffect3()->effectFlags_.color = { col.x_,col.y_,col.z_,1.0f };

	//
	slowState_->Update();
}


void GameVelocityManager::UpdateImGui()
{
	ImGui::Begin("GameSpeed");
	ImGui::SliderFloat("gameVelocity", &gameVelocity_, GAME_VELOCITY_MIN_, GAME_VELOCITY_MAX_);

	ImGui::Checkbox("isSlowMotion", &slowData_.isSlowMotion);
	ImGui::SliderFloat("slowVelBegin", &slowData_.slowVelBegin, GAME_VELOCITY_MIN_, GAME_VELOCITY_MAX_);
	ImGui::SliderFloat("slowVelEnd", &slowData_.slowVelEnd, GAME_VELOCITY_MIN_, GAME_VELOCITY_MAX_);
	ImGui::InputInt("slowTimeMax", &slowData_.slowTimeMax);

	if (ImGui::Button("SlowMotionBegin"))
	{
		BeginSlowMotion(slowData_.slowTimeMax, slowData_.slowVelEnd);
	}
	else if (ImGui::Button("SlowMotionEnd"))
	{
		EndSlowMotion();
	}
	ImGui::End();
}


//--------------------------------------------------------------------------------------
void GameVelocityManager::ChangeSlowState(const std::string& name)
{
	std::string nameL = name;
	if (slowState_)
	{
		slowState_.reset();
	}
	slowState_ = GameVelocityState::GetState(nameL);
	slowState_->Initialize(slowData_.slowVelBegin, slowData_.slowVelEnd, slowData_.slowTimeMax);
}
