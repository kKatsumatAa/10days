#include "TitleScene.h"

#include "Util.h"
#include "PadInput.h"
#include "SceneManager.h"
#include "KeyboardInput.h"
#include "Score.h"
#include "CameraManager.h"
#include "UI.h"


void TitleScene::Finalize()
{
}

void TitleScene::Initialize(void)
{
	//�摜�ǂݍ���
	titleImageHandle_ = TextureManager::LoadGraph("title.png");
	//BGM�Đ�
	Sound::GetInstance().PlayWave("title_BGM.wav", 1.0f, true);

	Score::GetInstance()->LoadScore();
	UI::GetInstance()->SetPos(UIType::Abutton, { 640.f,600.f });
	UI::GetInstance()->SetSize(UIType::Abutton, 0.8f);
	UI::GetInstance()->SetAncorPoint(UIType::Abutton, { 0.5f,0.5f });
}

void TitleScene::Update(void)
{
	// pad-A�ŉ�ʐؑ�
	if (PadInput::GetInstance().GetTriggerButton(GAMEPAD_A))
	{
		Sound::GetInstance().PlayWave("sceneChange_SE.wav");
		//BGM�X�g�b�v
		Sound::GetInstance().StopWave("title_BGM.wav");
		SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::TUTORIAL);
	}

	// debag�pkey-R�Őؑ�
#ifdef _DEBUG
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE))
	{
		Sound::GetInstance().PlayWave("sceneChange_SE.wav");
		//BGM�X�g�b�v
		Sound::GetInstance().StopWave("title_BGM.wav");
		SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::TUTORIAL);
	}
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_R))
	{
		Sound::GetInstance().PlayWave("decision_SE.wav");
	}
#endif // _DEBUG
}

void TitleScene::Draw(void)
{

}

void TitleScene::DrawSprite()
{

}

void TitleScene::DrawSprite2()
{
	//�^�C�g��
	titleObj_.DrawBoxSprite(CameraManager::GetInstance().GetCamera2D("UICamera"), titleImageHandle_);

	UI::GetInstance()->Draw(UIType::Abutton);
}

void TitleScene::DrawImgui()
{
}
