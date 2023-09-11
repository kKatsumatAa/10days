#include "ResultScene.h"

#include "Util.h"
#include "KeyboardInput.h"
#include "PadInput.h"
#include "SceneManager.h"
#include "Score.h"

void ResultScene::Finalize()
{
}

void ResultScene::Initialize(void)
{
	//画像読み込み
	TextureManager::LoadGraph("result.png");
	//BGM読み込み、再生
	Sound::GetInstance().LoadWave("result_BGM.wav", false);
	Sound::GetInstance().LoadWave("sceneChange_SE.wav", false);
	Sound::GetInstance().PlayWave("result_BGM.wav", 1.0f, true);

	Score::SaveScore();
}

void ResultScene::Update(void)
{
	if (PadInput::GetInstance().GetTriggerButton(GAMEPAD_A))
	{
		Sound::GetInstance().PlayWave("sceneChange_SE.wav");
		//BGMストップ
		Sound::GetInstance().StopWave("result_BGM.wav");
		SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::TITLE);
	}

#ifdef _DEBUG
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_R))
	{
		Sound::GetInstance().PlayWave("sceneChange_SE.wav");
		//BGMストップ
		Sound::GetInstance().StopWave("result_BGM.wav");
		SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::TITLE);
	}
#endif // _DEBUG
}

void ResultScene::Draw(void)
{

}

void ResultScene::DrawSprite()
{
	//リザルト画像
	result_.DrawBoxSprite(png_result_);

	//DrawFormatString(0, 380, UtilL::Color::RED, "Scene: RESULT");
	//DrawFormatString(0, 0, UtilL::Color::WHITE, "pad-A or key-R[DEBUG]でtitle");

	Score::Draw();
}

void ResultScene::DrawImgui()
{
}
