#include "SceneGame.h"
#include "SceneManager.h"
#include "GameVelocityManager.h"
#include "GameVelocityState.h"


void SceneGame::Finalize()
{

}

//---------------------------------------------------------------------------------------
void SceneGame::Initialize()
{
	GameVelocityManager::GetInstance().Initialize();
}

void SceneGame::Update()
{
	GameVelocityManager::GetInstance().Update();

	//リセット
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_R))
	{
		sceneM_->SetNextScene("GAME");
	}

	//シーン遷移
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_ESCAPE))
	{
		ParticleManager::GetInstance()->ClearParticles();
		sceneM_->SetNextScene("STAGESELECT");
	}
}

void SceneGame::Draw()
{
}

void SceneGame::DrawSprite()
{
}

void SceneGame::DrawImgui()
{
	GameVelocityManager::GetInstance().UpdateImGui();
}
