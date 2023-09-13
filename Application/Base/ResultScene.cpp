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
    Score::GetInstance()->LoadTex();

    //BGM再生
    Sound::GetInstance().PlayWave("result_BGM.wav", 0.2f, true);

    Score::GetInstance()->SaveScore();

    UI::GetInstance()->SetPos(UIType::NowScore, { 250.f,350.f });
    UI::GetInstance()->SetSize(UIType::NowScore, 0.5f);
    Score::GetInstance()->SetNowPos({ 1000.f,350.f });
    Score::GetInstance()->SetNowSize(0.5f);
    Score::GetInstance()->SetNowNum();

    Score::GetInstance()->SetRank();
    Score::GetInstance()->SetRankPos({150.f, 640.f});
    Score::GetInstance()->SetRankSize(0.5f);

    UI::GetInstance()->SetPos(UIType::HighScore, { 250.f,250.f });
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
}

void ResultScene::Update(void)
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
            //BGMストップ
            Sound::GetInstance().StopWave("result_BGM.wav");
            SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::GAME);
        }
    }
    else if (destination_ == Destination::TITLE)
    {
        if (PadInput::GetInstance().GetTriggerButton(GAMEPAD_A))
        {
            Sound::GetInstance().PlayWave("decision_SE.wav", 0.2f);
            //BGMストップ
            Sound::GetInstance().StopWave("result_BGM.wav");
            SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::TITLE);
        }
    }

#ifdef _DEBUG
    if (KeyboardInput::GetInstance().KeyTrigger(DIK_R))
    {
        Sound::GetInstance().PlayWave("decision_SE.wav", 0.2f);
        //BGMストップ
        Sound::GetInstance().StopWave("result_BGM.wav");
        SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::TITLE);
    }
#endif // _DEBUG


    //if (progressNum_ == 1)
    //{
    //    // 透明化しはじめるまでのフレームを待つため、kMaxFrame4StartTrans_の値を超えるまでは、rateの計算が出来ないようにする
    //    float frame = (float)(std::max)(frameCount_sign_ - kMaxFrame4StartTrans_, 0);

    //    // 現在フレームと最大値の割合算出
    //    float rate = (std::min)(frame / kMaxFrameTransColor_, 1.f);
    //    // 割合から、α値を段々減算
    //    alpha_sign_ = (int32_t)((1 - rate) * 255);

    //    // いったん仮でRB押しで次へ進行
    //    if (PadInput::GetInstance().GetTriggerButton(GAMEPAD_RIGHT_SHOULDER))
    //    {
    //        // 看板用フレームカウンタ加算
    //        frameCount_sign_++;
    //    }

    //    // フレームカウンタが起動されてたら加算し続ける
    //    if (frameCount_sign_)
    //    {
    //        // 看板用フレームカウンタ加算
    //        frameCount_sign_++;

    //        // フレームカウンタの値が、(透明化し始めるまで + 透明になるまで + 次の間隔)を上回ったら
    //        if (frameCount_sign_ > kMaxFrame4StartTrans_ + kMaxFrameTransColor_ + kIntervalNextSign_)
    //        {
    //            // カウンタ初期化して
    //            frameCount_sign_ = 0;
    //            // α値初期化して
    //            alpha_sign_ = 255;
    //            // 表示する看板を次のものへ
    //            progressNum_++;
    //        }
    //    }
    //}
    //else if (progressNum_ == 2)
    //{
    //    // 透明化しはじめるまでのフレームを待つため、kMaxFrame4StartTrans_の値を超えるまでは、rateの計算が出来ないようにする
    //    float frame = (float)(std::max)(frameCount_sign_ - kMaxFrame4StartTrans_, 0);

    //    // 現在フレームと最大値の割合算出
    //    float rate = (std::min)(frame / kMaxFrameTransColor_, 1.f);
    //    // 割合から、α値を段々減算
    //    alpha_sign_ = (int32_t)((1 - rate) * 255);

    //    // いったん仮でA押し && スティック入力してたら
    //    if (PadInput::GetInstance().GetPushButton(GAMEPAD_A) && PadInput::GetInstance().GetLeftStickTilt().x || PadInput::GetInstance().GetLeftStickTilt().y) // ここスローモーションかかるせいで時間かかりすぎ、エンジンならこうはならないと思うけど
    //    {
    //        // ここの加算は最初の一回のみにしたい
    //        if (frameCount_sign_ == 0)
    //        {
    //            // 看板用フレームカウンタ加算
    //            frameCount_sign_++;
    //        }
    //    }

    //    // フレームカウンタが起動されてたら加算し続ける
    //    if (frameCount_sign_)
    //    {
    //        // 看板用フレームカウンタ加算
    //        frameCount_sign_++;

    //        // フレームカウンタの値が、(透明化し始めるまで + 透明になるまで + 次の間隔)を上回ったら
    //        if (frameCount_sign_ > kMaxFrame4StartTrans_ + kMaxFrameTransColor_ + kIntervalNextSign_)
    //        {
    //            // カウンタ初期化して
    //            frameCount_sign_ = 0;
    //            // α値初期化して
    //            alpha_sign_ = 255;
    //            // 表示する看板を次のものへ
    //            progressNum_++;
    //        }
    //    }
    //}
    //else if (progressNum_ == 3)
    //{
    //    // 透明化しはじめるまでのフレームを待つため、kMaxFrame4StartTrans_の値を超えるまでは、rateの計算が出来ないようにする
    //    float frame = (float)(std::max)(frameCount_sign_ - kMaxFrame4StartTrans_, 0);

    //    // 現在フレームと最大値の割合算出
    //    float rate = (std::min)(frame / kMaxFrameTransColor_, 1.f);
    //    // 割合から、α値を段々減算
    //    alpha_sign_ = (int32_t)((1 - rate) * 255);

    //    // いったん仮でpad-AをReleaseで
    //    if (PadInput::GetInstance().GetReleaseTrigger(GAMEPAD_A)) // ここの挙動ちょっと怪しい
    //    {
    //        // 看板用フレームカウンタ加算
    //        frameCount_sign_++;
    //    }

    //    // フレームカウンタが起動されてたら加算し続ける
    //    if (frameCount_sign_)
    //    {
    //        // 看板用フレームカウンタ加算
    //        frameCount_sign_++;

    //        // フレームカウンタの値が、(透明化し始めるまで + 透明になるまで + 次の間隔)を上回ったら
    //        if (frameCount_sign_ > kMaxFrame4StartTrans_ + kMaxFrameTransColor_ + kIntervalNextSign_)
    //        {
    //            // カウンタ初期化して
    //            frameCount_sign_ = 0;
    //            // α値初期化して
    //            alpha_sign_ = 255;
    //            // 表示する看板を次のものへ
    //            progressNum_++;
    //        }
    //    }
    //}
    //else if (progressNum_ == 4) // "8体くっつけてデカ団子に！"
    //{
    //    // 透明化しはじめるまでのフレームを待つため、kMaxFrame4StartTrans_の値を超えるまでは、rateの計算が出来ないようにする
    //    float frame = (float)(std::max)(frameCount_sign_ - kMaxFrame4StartTrans_, 0);

    //    // 現在フレームと最大値の割合算出
    //    float rate = (std::min)(frame / kMaxFrameTransColor_, 1.f);
    //    // 割合から、α値を段々減算
    //    alpha_sign_ = (int32_t)((1 - rate) * 255);

    //    // いったん仮でRB押しで次へ進行
    //    if (PadInput::GetInstance().GetReleaseTrigger(GAMEPAD_RIGHT_SHOULDER))
    //    {
    //        // 看板用フレームカウンタ加算
    //        frameCount_sign_++;
    //    }

    //    // フレームカウンタが起動されてたら加算し続ける
    //    if (frameCount_sign_)
    //    {
    //        // 看板用フレームカウンタ加算
    //        frameCount_sign_++;

    //        // フレームカウンタの値が、(透明化し始めるまで + 透明になるまで + 次の間隔)を上回ったら
    //        if (frameCount_sign_ > kMaxFrame4StartTrans_ + kMaxFrameTransColor_ + kIntervalNextSign_)
    //        {
    //            // カウンタ初期化して
    //            frameCount_sign_ = 0;
    //            // α値初期化して
    //            alpha_sign_ = 255;
    //            // 表示する看板を次のものへ
    //            progressNum_++;
    //        }
    //    }
    //}
    //else if (progressNum_ == 5) // "倒すと高得点"
    //{
    //    // 透明化しはじめるまでのフレームを待つため、kMaxFrame4StartTrans_の値を超えるまでは、rateの計算が出来ないようにする
    //    float frame = (float)(std::max)(frameCount_sign_ - kMaxFrame4StartTrans_, 0);

    //    // 現在フレームと最大値の割合算出
    //    float rate = (std::min)(frame / kMaxFrameTransColor_, 1.f);
    //    // 割合から、α値を段々減算
    //    alpha_sign_ = (int32_t)((1 - rate) * 255);

    //    // いったん仮でRB押しで次へ進行
    //    if (PadInput::GetInstance().GetReleaseTrigger(GAMEPAD_RIGHT_SHOULDER))
    //    {
    //        // 看板用フレームカウンタ加算
    //        frameCount_sign_++;
    //    }

    //    // フレームカウンタが起動されてたら加算し続ける
    //    if (frameCount_sign_)
    //    {
    //        // 看板用フレームカウンタ加算
    //        frameCount_sign_++;

    //        // フレームカウンタの値が、(透明化し始めるまで + 透明になるまで + 次の間隔)を上回ったら
    //        if (frameCount_sign_ > kMaxFrame4StartTrans_ + kMaxFrameTransColor_ + kIntervalNextSign_)
    //        {
    //            // カウンタ初期化して
    //            frameCount_sign_ = 0;
    //            // α値初期化して
    //            alpha_sign_ = 255;
    //            // 表示する看板を次のものへ
    //            progressNum_++;
    //        }
    //    }
    //}
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

    UI::GetInstance()->Draw(UIType::Retry);
    UI::GetInstance()->Draw(UIType::ToTitle);
    UI::GetInstance()->Draw(UIType::Abutton);

    /*if (progressNum_ == 1)
    {
        UI::GetInstance()->Draw(UIType::Tutorial1);
    }
    else if (progressNum_ == 2)
    {
        UI::GetInstance()->Draw(UIType::Tutorial2);
        UI::GetInstance()->Draw(UIType::Tutorial3);
    }
    else if (progressNum_ == 3)
    {
        UI::GetInstance()->Draw(UIType::Tutorial4);
    }
    else if (progressNum_ == 4)
    {
        UI::GetInstance()->Draw(UIType::Tutorial5);
    }
    else if (progressNum_ == 5)
    {
        UI::GetInstance()->Draw(UIType::Tutorial6);
    }*/

    Score::GetInstance()->DrawRank(0.f);
}

void ResultScene::DrawImgui()
{
}
