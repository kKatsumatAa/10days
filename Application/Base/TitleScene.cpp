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
	//画像読み込み
	titleImageHandle_ = TextureManager::LoadGraph("title.png");
	//BGM読み込み、再生
	Sound::GetInstance().LoadWave("title_BGM.wav", false);
	Sound::GetInstance().PlayWave("title_BGM.wav", 1.0f, true);
	//SE読み込み
	Sound::GetInstance().LoadWave("decision_SE.wav", false);
	Sound::GetInstance().LoadWave("sceneChange_SE.wav", false);

	Score::LoadScore();
}

void TitleScene::Update(void)
{
	// pad-Aで画面切替
	if (PadInput::GetInstance().GetTriggerButton(GAMEPAD_A))
	{
		Sound::GetInstance().PlayWave("sceneChange_SE.wav");
		//BGMストップ
		Sound::GetInstance().StopWave("title_BGM.wav");
		SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::GAME);
	}

	// debag用key-Rで切替
#ifdef _DEBUG
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE))
	{
		Sound::GetInstance().PlayWave("sceneChange_SE.wav");
		//BGMストップ
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
	//タイトル
	titleObj_.DrawBoxSprite(titleImageHandle_);

	//DrawFormatString(0, 380, UtilL::Color::RED, "Scene: TITLE");
	//DrawFormatString(0, 0, UtilL::Color::RED, "press pad-A or key-SPACE");
	//DrawFormatString(0, 20, UtilL::Color::RED, "key-Rでpad接続再確認");
	//DrawFormatString(0, 40, UtilL::Color::WHITE, GetPadConnect() ? "pad connected" : "pad isnt connected");
}

void TitleScene::DrawImgui()
{
}
