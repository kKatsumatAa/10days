#include "ResultScene.h"
#include "CameraManager.h"
#include "Util.h"
#include "KeyboardInput.h"
#include "PadInput.h"
#include "SceneManager.h"
#include "Score.h"
#include "UI.h"

void ResultScene::Finalize()
{
}

void ResultScene::Initialize(void)
{
	//画像読み込み
	png_result_ = TextureManager::LoadGraph("result.png");
	//BGM再生
	Sound::GetInstance().PlayWave("result_BGM.wav", 1.0f, true);

	Score::GetInstance()->SaveScore();

    UI::GetInstance()->SetPos(UIType::NowScore, { 250.f,350.f });
    UI::GetInstance()->SetSize(UIType::NowScore, 0.5f);
    Score::GetInstance()->SetNowPos({1000.f,350.f});
	Score::GetInstance()->SetNowSize(0.5f);
    Score::GetInstance()->SetNowNum();

    UI::GetInstance()->SetPos(UIType::HighScore, { 250.f,250.f });
    UI::GetInstance()->SetSize(UIType::HighScore, 0.5f);
    Score::GetInstance()->SetHighPos({ 1000.f,250.f });
    Score::GetInstance()->SetHighSize(0.5f);
    Score::GetInstance()->SetHighNum();

	UI::GetInstance()->SetPos(UIType::Abutton, { 640.f,600.f });
	UI::GetInstance()->SetSize(UIType::Abutton, 0.8f);
	UI::GetInstance()->SetAncorPoint(UIType::Abutton, {0.5f,0.5f});
}

void ResultScene::Update(void)
{
    if (PadInput::GetInstance().GetLeftStickTilt().x >= 0.3f)
    {
        destination_++;
        destination_ = (std::min)(destination_, 1);
    }
    else if (PadInput::GetInstance().GetLeftStickTilt().x <= -0.3f)
    {
        destination_--;
        destination_ = (std::max)(destination_, 0);
    }

    if (destination_ == Destination::RETRY)
    {
        if (PadInput::GetInstance().GetTriggerButton(GAMEPAD_A))
        {
            Sound::GetInstance().PlayWave("sceneChange_SE.wav");
            //BGMストップ
            Sound::GetInstance().StopWave("result_BGM.wav");
            SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::GAME);
        }
    }
    else if (destination_ == Destination::TITLE)
    {
        if (PadInput::GetInstance().GetTriggerButton(GAMEPAD_A))
        {
            Sound::GetInstance().PlayWave("sceneChange_SE.wav");
            //BGMストップ
            Sound::GetInstance().StopWave("result_BGM.wav");
            SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::TITLE);
        }
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

}

void ResultScene::DrawSprite2()
{
    //リザルト画像
    result_.DrawBoxSprite(CameraManager::GetInstance().GetCamera2D("UICamera"), png_result_);

    Score::GetInstance()->Draw();
    Score::GetInstance()->DrawHighScore();
    UI::GetInstance()->Draw(UIType::NowScore);
    UI::GetInstance()->Draw(UIType::HighScore);
    UI::GetInstance()->Draw(UIType::Abutton);
}

void ResultScene::DrawImgui()
{
}
