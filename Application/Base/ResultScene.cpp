#include "ResultScene.h"
#include "CameraManager.h"
#include "Util.h"
#include "KeyboardInput.h"
#include "PadInput.h"
#include "SceneManager.h"
#include "Score.h"
#include "UI.h"
#include "MathUtil.h"

void ResultScene::Finalize()
{
    UI::GetInstance()->SetColor(UIType::Abutton, { 1,1,1,1 });

    timer_wait4Start_.Reset();
    timer_currentScore_.Reset();
    timer_highScore_.Reset();
    timer_newRecord_.Reset();
    timer_DisplayRank_.Reset();
    timer_other_.Reset();
}

void ResultScene::Initialize(void)
{
    //æÇÝÝ
    png_result_ = TextureManager::LoadGraph("result.png");
    Score::GetInstance()->LoadTex();

    //BGMÄ¶
    Sound::GetInstance().PlayWave("result_BGM.wav", 0.2f, true);

    Score::GetInstance()->SaveScore();

    UI::GetInstance()->SetPos(UIType::NowScore, { 250.f,400.f });
    UI::GetInstance()->SetSize(UIType::NowScore, 0.5f);
    Score::GetInstance()->SetNowPos({ 1000.f,350.f });
    Score::GetInstance()->SetNowSize(0.5f);
    Score::GetInstance()->SetNowNum();

    Score::GetInstance()->SetRank();
    Score::GetInstance()->SetRankPos({ 640.f, 300.f });
    Score::GetInstance()->SetRankSize(0.8f);

    UI::GetInstance()->SetPos(UIType::HighScore, { 250.f,300.f });
    UI::GetInstance()->SetSize(UIType::HighScore, 0.5f);
    Score::GetInstance()->SetHighPos({ 1000.f,250.f });
    Score::GetInstance()->SetHighSize(0.5f);
    Score::GetInstance()->SetHighNum();

    UI::GetInstance()->SetPos(UIType::Retry, { 420,600 });
    UI::GetInstance()->SetAncorPoint(UIType::Retry, { 0.5f,0.5f });
    UI::GetInstance()->SetPos(UIType::ToTitle, { 940,600 });
    UI::GetInstance()->SetAncorPoint(UIType::ToTitle, { 0.5f,0.5f });
    UI::GetInstance()->SetPos(UIType::Abutton, { 610,550 });
    UI::GetInstance()->SetSize(UIType::Abutton, 0.3f);
    UI::GetInstance()->SetAncorPoint(UIType::Abutton, { 0.5f,0.5f });
    if (destination_ == Destination::RETRY)
    {
        UI::GetInstance()->SetColor(UIType::ToTitle, { 1,1,1,0.3f });
        UI::GetInstance()->SetSize(UIType::ToTitle, 0.4f);
        UI::GetInstance()->SetColor(UIType::Retry, { 1,1,1,1 });
        UI::GetInstance()->SetSize(UIType::Retry, 0.7f);
    }
    else if (destination_ == Destination::TITLE)
    {
        UI::GetInstance()->SetColor(UIType::Retry, { 1,1,1,0.3f });
        UI::GetInstance()->SetSize(UIType::Retry, 0.4f);
        UI::GetInstance()->SetColor(UIType::ToTitle, { 1,1,1,1 });
        UI::GetInstance()->SetSize(UIType::ToTitle, 0.7f);
    }

    UI::GetInstance()->SetPos(UIType::NowScore, { 250,200 });
    UI::GetInstance()->SetPos(UIType::HighScore, { 250,300 });

}

void ResultScene::Update(void)
{
    // ÁZ1
    timer_wait4Start_.Update(false, 1.f);
    // ªò1
    if (timer_wait4Start_.GetisTimeOut())
    {
        // ÁZ2
        timer_currentScore_.Update(false, 1.f);
        // ªò2
        if (timer_currentScore_.GetisTimeOut())
        {
            // ÁZ3
            timer_highScore_.Update(false, 1.f);
            // ªò3
            if (timer_highScore_.GetisTimeOut())
            {
                if (Score::GetInstance()->GetIsNewRecord())
                {
                    // ÁZ4
                    timer_newRecord_.Update(false, 1.f);
                    // ªò4
                    if (timer_newRecord_.GetisTimeOut())
                    {
                        // ÁZ5
                        timer_DisplayRank_.Update(false, 1.f);
                        // ªò5
                        if (timer_DisplayRank_.GetisTimeOut())
                        {
                            isEndAllDisplay_ = true;
                            // ÁZ6
                            timer_other_.Update(false, 1.f);
                        }
                    }
                }
                else
                {
                    // ÁZ5
                    timer_DisplayRank_.Update(false, 1.f);
                    // ªò5
                    if (timer_DisplayRank_.GetisTimeOut())
                    {
                        isEndAllDisplay_ = true;
                        // ÁZ6
                        timer_other_.Update(false, 1.f);

                    }
                }
            }
        }
    }

    if (isEndAllDisplay_)
    {
        if (PadInput::GetInstance().GetLeftStickTilt().x >= 0.3f)
        {
            if (destination_ == 0)
            {
                Sound::GetInstance().PlayWave("select_SE.wav", 0.5f);
            }
            destination_++;
            destination_ = (std::min)(destination_, 1);
            UI::GetInstance()->SetColor(UIType::Retry, { 1,1,1,0.3f });
            UI::GetInstance()->SetSize(UIType::Retry, 0.4f);
            UI::GetInstance()->SetColor(UIType::ToTitle, { 1,1,1,1 });
            UI::GetInstance()->SetSize(UIType::ToTitle, 0.7f);
            UI::GetInstance()->SetPos(UIType::Abutton, { 1100,550 });
        }
        else if (PadInput::GetInstance().GetLeftStickTilt().x <= -0.3f)
        {
            if (destination_ == 1)
            {
                Sound::GetInstance().PlayWave("select_SE.wav", 0.5f);
            }
            destination_--;
            destination_ = (std::max)(destination_, 0);
            UI::GetInstance()->SetColor(UIType::ToTitle, { 1,1,1,0.3f });
            UI::GetInstance()->SetSize(UIType::ToTitle, 0.4f);
            UI::GetInstance()->SetColor(UIType::Retry, { 1,1,1,1 });
            UI::GetInstance()->SetSize(UIType::Retry, 0.7f);
            UI::GetInstance()->SetPos(UIType::Abutton, { 610,550 });
        }

        if (destination_ == Destination::RETRY)
        {
            if (PadInput::GetInstance().GetTriggerButton(GAMEPAD_A))
            {
                Sound::GetInstance().PlayWave("decision_SE.wav", 0.2f);
                //BGMXgbv
                Sound::GetInstance().StopWave("result_BGM.wav");
                SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::GAME);
            }
        }
        else if (destination_ == Destination::TITLE)
        {
            if (PadInput::GetInstance().GetTriggerButton(GAMEPAD_A))
            {
                Sound::GetInstance().PlayWave("decision_SE.wav", 0.2f);
                //BGMXgbv
                Sound::GetInstance().StopWave("result_BGM.wav");
                SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::TITLE);
            }
        }
    }
#ifdef _DEBUG
    if (KeyboardInput::GetInstance().KeyTrigger(DIK_R))
    {
        Sound::GetInstance().PlayWave("decision_SE.wav", 0.2f);
        //BGMXgbv
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
    //Ugæ
    result_.DrawBoxSprite(CameraManager::GetInstance().GetCamera2D("UICamera"), png_result_);

    Score::GetInstance()->DrawHighScore();

    float rate = (std::min)(timer_currentScore_.GetTimer() / timer_currentScore_.GetMaxTimer(), 1.f);
    UI::GetInstance()->SetColor(UIType::NowScore, { 1.f,1.f,1.f,Math::Ease::EaseInOutSine(rate,0.f,1.f) });
    UI::GetInstance()->SetSize(UIType::NowScore, Math::Ease::EaseInOutSine(rate, 5.f, 0.5f));
    UI::GetInstance()->Draw(UIType::NowScore);
    Score::GetInstance()->SetNowColor({ 1.f,1.f,1.f,Math::Ease::EaseInOutSine(rate,0.f,1.f) });
    Score::GetInstance()->SetNowPos({ Math::Ease::EaseOutSine(rate,1500,1000),210 });
    Score::GetInstance()->SetNowNum();
    Score::GetInstance()->Draw();
    float rate2 = (std::min)(timer_highScore_.GetTimer() / timer_highScore_.GetMaxTimer(), 1.f);
    UI::GetInstance()->SetColor(UIType::HighScore, { 1.f,1.f,1.f,Math::Ease::EaseInOutSine(rate2,0.f,1.f) });
    UI::GetInstance()->SetSize(UIType::HighScore, Math::Ease::EaseInOutSine(rate2, 5.f, 0.5f));
    UI::GetInstance()->Draw(UIType::HighScore);
    Score::GetInstance()->SetHighColor({ 1.f,1.f,1.f,Math::Ease::EaseInOutSine(rate2,0.f,1.f) });
    Score::GetInstance()->SetHighPos({ Math::Ease::EaseOutSine(rate2,1500,1000),310 });
    Score::GetInstance()->SetHighNum();
    Score::GetInstance()->Draw();
    float rate3 = (std::min)(timer_newRecord_.GetTimer() / timer_newRecord_.GetMaxTimer(), 1.f);
    UI::GetInstance()->SetColor(UIType::NewRecord, { 1.f,0.f,0.f,Math::Ease::EaseInOutSine(rate3,0.f,1.f) });
    UI::GetInstance()->SetSize(UIType::NewRecord, Math::Ease::EaseInOutSine(rate3, 5.f, 0.5f));
    UI::GetInstance()->Draw(UIType::NewRecord);

    float rate4 = (std::min)(timer_DisplayRank_.GetTimer() / timer_DisplayRank_.GetMaxTimer(), 1.f);
    Score::GetInstance()->SetRankPos({ 640.f,Math::Ease::EaseOutSine(rate4,-200,300) });
    Score::GetInstance()->DrawRank(0.1f);

    if (isEndAllDisplay_)
    {
        float rate5 = (std::min)(timer_other_.GetTimer() / timer_other_.GetMaxTimer(), 1.f);
        UI::GetInstance()->SetColor(UIType::Retry, { 1.f,1.f,1.f,Math::Ease::EaseInOutSine(rate5,0.f,1.f) });
        UI::GetInstance()->SetColor(UIType::ToTitle, { 1.f,1.f,1.f,Math::Ease::EaseInOutSine(rate5,0.f,1.f) });
        UI::GetInstance()->SetColor(UIType::Abutton, { 1.f,1.f,1.f,Math::Ease::EaseInOutSine(rate5,0.f,1.f) });
        UI::GetInstance()->Draw(UIType::Retry);
        UI::GetInstance()->Draw(UIType::ToTitle);
        UI::GetInstance()->Draw(UIType::Abutton);
    }
}

void ResultScene::DrawImgui()
{
}
