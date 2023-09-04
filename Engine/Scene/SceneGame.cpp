#include "SceneGame.h"
#include "SceneManager.h"


void SceneGame::Finalize()
{

}

//---------------------------------------------------------------------------------------
void SceneGame::Initialize()
{
	ParticleManager::GetInstance()->SetBlendNum(ParticleManager::TRIANGLE);

	//�|�X�g�G�t�F�N�g
	PostEffectManager::GetInstance().GetPostEffect1()->effectFlags_.isVignette = true;

	PostEffectManager::GetInstance().GetPostEffect1()->effectFlags_.isDepthField = true;
	PostEffectManager::GetInstance().GetPostEffect1()->effectFlags_.focusDepth = 0.797f;//0.742
	PostEffectManager::GetInstance().GetPostEffect1()->effectFlags_.nFocusWidth = 0.862f;//1.316
	PostEffectManager::GetInstance().GetPostEffect1()->effectFlags_.focusDiffPow = 1.272f;//1.142
}

void SceneGame::Update()
{
	//���Z�b�g
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_R))
	{
		sceneM_->SetNextScene("GAME");
	}


	//�V�[���J��
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
}
