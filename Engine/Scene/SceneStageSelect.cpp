#include "SceneStageSelect.h"
#include "SceneManager.h"


void SceneStageSelect::Finalize()
{
}

//---------------------------------------------------------------------------------------
void SceneStageSelect::Initialize()
{
	//ObjectManager::GetInstance().ClearAllObj();
	ParticleManager::GetInstance()->ClearParticles();

	sceneM_->StopWaveAllScene();

	//�|�X�g�G�t�F�N�g
	PostEffectManager::GetInstance().GetPostEffect2()->effectFlags_.isScanningLine = true;
	PostEffectManager::GetInstance().GetPostEffect3()->effectFlags_.isBarrelCurve = true;
	PostEffectManager::GetInstance().GetPostEffect1()->effectFlags_.isDepthField = false;
}

void SceneStageSelect::Update()
{
	//�V�[���J��
	if (MouseInput::GetInstance().GetTriggerClick(CLICK_LEFT))
	{
		PostEffectManager::GetInstance().GetPostEffect2()->effectFlags_.isScanningLine = false;

		sceneM_->SetNextScene("GAME");
	}
	//�^�C�g����escape�ŏI��
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_ESCAPE))
	{
		sceneM_->SetNextScene("TITLE");
	}
}

void SceneStageSelect::Draw()
{

}

void SceneStageSelect::DrawSprite()
{
}

void SceneStageSelect::DrawImgui()
{
}

