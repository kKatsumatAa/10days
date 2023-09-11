#include "TitleScene.h"

#include "Util.h"
#include "PadInput.h"
#include "SceneManager.h"
#include "KeyboardInput.h"
#include "Score.h"

void TitleScene::Finalize()
{
}

void TitleScene::Initialize(void)
{
	//�摜�ǂݍ���
	titleImageHandle_ = TextureManager::LoadGraph("title.png");
	//BGM�Đ�
	Sound::GetInstance().PlayWave("title_BGM.wav", 1.0f, true);

	Score::LoadScore();
}

void TitleScene::Update(void)
{
	// pad-A�ŉ�ʐؑ�
	if (PadInput::GetInstance().GetTriggerButton(GAMEPAD_A))
	{
		Sound::GetInstance().PlayWave("sceneChange_SE.wav");
		//BGM�X�g�b�v
		Sound::GetInstance().StopWave("title_BGM.wav");
		SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::GAME);
	}

	// debag�pkey-R�Őؑ�
#ifdef _DEBUG
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE))
	{
		Sound::GetInstance().PlayWave("sceneChange_SE.wav");
		//BGM�X�g�b�v
		Sound::GetInstance().StopWave("title_BGM.wav");
		SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::GAME);
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
	//�^�C�g��
	titleObj_.DrawBoxSprite(titleImageHandle_);

	//DrawFormatString(0, 380, UtilL::Color::RED, "Scene: TITLE");
	//DrawFormatString(0, 0, UtilL::Color::RED, "press pad-A or key-SPACE");
	//DrawFormatString(0, 20, UtilL::Color::RED, "key-R��pad�ڑ��Ċm�F");
	//DrawFormatString(0, 40, UtilL::Color::WHITE, GetPadConnect() ? "pad connected" : "pad isnt connected");
}

void TitleScene::DrawImgui()
{
}
