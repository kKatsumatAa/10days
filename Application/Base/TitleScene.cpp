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
	//画像読み込み
	titleImageHandle_ = TextureManager::LoadGraph("title.png");
	//BGM再生
	Sound::GetInstance().PlayWave("title_BGM.wav", 1.0f, true);

	Score::GetInstance()->LoadScore();
	UI::GetInstance()->SetPos(UIType::Abutton, { 640.f,600.f });
	UI::GetInstance()->SetSize(UIType::Abutton, 0.8f);
	UI::GetInstance()->SetAncorPoint(UIType::Abutton, { 0.5f,0.5f });
}

void TitleScene::Update(void)
{
	// pad-Aで画面切替
	if (PadInput::GetInstance().GetTriggerButton(GAMEPAD_A))
	{
		Sound::GetInstance().PlayWave("sceneChange_SE.wav");
		//BGMストップ
		Sound::GetInstance().StopWave("title_BGM.wav");
		SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::TUTORIAL);
	}

	// debag用key-Rで切替
#ifdef _DEBUG
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE))
	{
		Sound::GetInstance().PlayWave("sceneChange_SE.wav");
		//BGMストップ
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
	//タイトル
	titleObj_.DrawBoxSprite(CameraManager::GetInstance().GetCamera2D("UICamera"), titleImageHandle_);

	UI::GetInstance()->Draw(UIType::Abutton);
}

void TitleScene::DrawImgui()
{
}
