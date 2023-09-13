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
    //�摜�ǂݍ���
    png_result_ = TextureManager::LoadGraph("result.png");
    Score::GetInstance()->LoadTex();

    //BGM�Đ�
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
            //BGM�X�g�b�v
            Sound::GetInstance().StopWave("result_BGM.wav");
            SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::GAME);
        }
    }
    else if (destination_ == Destination::TITLE)
    {
        if (PadInput::GetInstance().GetTriggerButton(GAMEPAD_A))
        {
            Sound::GetInstance().PlayWave("decision_SE.wav", 0.2f);
            //BGM�X�g�b�v
            Sound::GetInstance().StopWave("result_BGM.wav");
            SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::TITLE);
        }
    }

#ifdef _DEBUG
    if (KeyboardInput::GetInstance().KeyTrigger(DIK_R))
    {
        Sound::GetInstance().PlayWave("decision_SE.wav", 0.2f);
        //BGM�X�g�b�v
        Sound::GetInstance().StopWave("result_BGM.wav");
        SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::TITLE);
    }
#endif // _DEBUG


    //if (progressNum_ == 1)
    //{
    //    // ���������͂��߂�܂ł̃t���[����҂��߁AkMaxFrame4StartTrans_�̒l�𒴂���܂ł́Arate�̌v�Z���o���Ȃ��悤�ɂ���
    //    float frame = (float)(std::max)(frameCount_sign_ - kMaxFrame4StartTrans_, 0);

    //    // ���݃t���[���ƍő�l�̊����Z�o
    //    float rate = (std::min)(frame / kMaxFrameTransColor_, 1.f);
    //    // ��������A���l��i�X���Z
    //    alpha_sign_ = (int32_t)((1 - rate) * 255);

    //    // �������񉼂�RB�����Ŏ��֐i�s
    //    if (PadInput::GetInstance().GetTriggerButton(GAMEPAD_RIGHT_SHOULDER))
    //    {
    //        // �Ŕp�t���[���J�E���^���Z
    //        frameCount_sign_++;
    //    }

    //    // �t���[���J�E���^���N������Ă�����Z��������
    //    if (frameCount_sign_)
    //    {
    //        // �Ŕp�t���[���J�E���^���Z
    //        frameCount_sign_++;

    //        // �t���[���J�E���^�̒l���A(���������n�߂�܂� + �����ɂȂ�܂� + ���̊Ԋu)����������
    //        if (frameCount_sign_ > kMaxFrame4StartTrans_ + kMaxFrameTransColor_ + kIntervalNextSign_)
    //        {
    //            // �J�E���^����������
    //            frameCount_sign_ = 0;
    //            // ���l����������
    //            alpha_sign_ = 255;
    //            // �\������Ŕ����̂��̂�
    //            progressNum_++;
    //        }
    //    }
    //}
    //else if (progressNum_ == 2)
    //{
    //    // ���������͂��߂�܂ł̃t���[����҂��߁AkMaxFrame4StartTrans_�̒l�𒴂���܂ł́Arate�̌v�Z���o���Ȃ��悤�ɂ���
    //    float frame = (float)(std::max)(frameCount_sign_ - kMaxFrame4StartTrans_, 0);

    //    // ���݃t���[���ƍő�l�̊����Z�o
    //    float rate = (std::min)(frame / kMaxFrameTransColor_, 1.f);
    //    // ��������A���l��i�X���Z
    //    alpha_sign_ = (int32_t)((1 - rate) * 255);

    //    // �������񉼂�A���� && �X�e�B�b�N���͂��Ă���
    //    if (PadInput::GetInstance().GetPushButton(GAMEPAD_A) && PadInput::GetInstance().GetLeftStickTilt().x || PadInput::GetInstance().GetLeftStickTilt().y) // �����X���[���[�V���������邹���Ŏ��Ԃ����肷���A�G���W���Ȃ炱���͂Ȃ�Ȃ��Ǝv������
    //    {
    //        // �����̉��Z�͍ŏ��̈��݂̂ɂ�����
    //        if (frameCount_sign_ == 0)
    //        {
    //            // �Ŕp�t���[���J�E���^���Z
    //            frameCount_sign_++;
    //        }
    //    }

    //    // �t���[���J�E���^���N������Ă�����Z��������
    //    if (frameCount_sign_)
    //    {
    //        // �Ŕp�t���[���J�E���^���Z
    //        frameCount_sign_++;

    //        // �t���[���J�E���^�̒l���A(���������n�߂�܂� + �����ɂȂ�܂� + ���̊Ԋu)����������
    //        if (frameCount_sign_ > kMaxFrame4StartTrans_ + kMaxFrameTransColor_ + kIntervalNextSign_)
    //        {
    //            // �J�E���^����������
    //            frameCount_sign_ = 0;
    //            // ���l����������
    //            alpha_sign_ = 255;
    //            // �\������Ŕ����̂��̂�
    //            progressNum_++;
    //        }
    //    }
    //}
    //else if (progressNum_ == 3)
    //{
    //    // ���������͂��߂�܂ł̃t���[����҂��߁AkMaxFrame4StartTrans_�̒l�𒴂���܂ł́Arate�̌v�Z���o���Ȃ��悤�ɂ���
    //    float frame = (float)(std::max)(frameCount_sign_ - kMaxFrame4StartTrans_, 0);

    //    // ���݃t���[���ƍő�l�̊����Z�o
    //    float rate = (std::min)(frame / kMaxFrameTransColor_, 1.f);
    //    // ��������A���l��i�X���Z
    //    alpha_sign_ = (int32_t)((1 - rate) * 255);

    //    // �������񉼂�pad-A��Release��
    //    if (PadInput::GetInstance().GetReleaseTrigger(GAMEPAD_A)) // �����̋���������Ɖ�����
    //    {
    //        // �Ŕp�t���[���J�E���^���Z
    //        frameCount_sign_++;
    //    }

    //    // �t���[���J�E���^���N������Ă�����Z��������
    //    if (frameCount_sign_)
    //    {
    //        // �Ŕp�t���[���J�E���^���Z
    //        frameCount_sign_++;

    //        // �t���[���J�E���^�̒l���A(���������n�߂�܂� + �����ɂȂ�܂� + ���̊Ԋu)����������
    //        if (frameCount_sign_ > kMaxFrame4StartTrans_ + kMaxFrameTransColor_ + kIntervalNextSign_)
    //        {
    //            // �J�E���^����������
    //            frameCount_sign_ = 0;
    //            // ���l����������
    //            alpha_sign_ = 255;
    //            // �\������Ŕ����̂��̂�
    //            progressNum_++;
    //        }
    //    }
    //}
    //else if (progressNum_ == 4) // "8�̂������ăf�J�c�q�ɁI"
    //{
    //    // ���������͂��߂�܂ł̃t���[����҂��߁AkMaxFrame4StartTrans_�̒l�𒴂���܂ł́Arate�̌v�Z���o���Ȃ��悤�ɂ���
    //    float frame = (float)(std::max)(frameCount_sign_ - kMaxFrame4StartTrans_, 0);

    //    // ���݃t���[���ƍő�l�̊����Z�o
    //    float rate = (std::min)(frame / kMaxFrameTransColor_, 1.f);
    //    // ��������A���l��i�X���Z
    //    alpha_sign_ = (int32_t)((1 - rate) * 255);

    //    // �������񉼂�RB�����Ŏ��֐i�s
    //    if (PadInput::GetInstance().GetReleaseTrigger(GAMEPAD_RIGHT_SHOULDER))
    //    {
    //        // �Ŕp�t���[���J�E���^���Z
    //        frameCount_sign_++;
    //    }

    //    // �t���[���J�E���^���N������Ă�����Z��������
    //    if (frameCount_sign_)
    //    {
    //        // �Ŕp�t���[���J�E���^���Z
    //        frameCount_sign_++;

    //        // �t���[���J�E���^�̒l���A(���������n�߂�܂� + �����ɂȂ�܂� + ���̊Ԋu)����������
    //        if (frameCount_sign_ > kMaxFrame4StartTrans_ + kMaxFrameTransColor_ + kIntervalNextSign_)
    //        {
    //            // �J�E���^����������
    //            frameCount_sign_ = 0;
    //            // ���l����������
    //            alpha_sign_ = 255;
    //            // �\������Ŕ����̂��̂�
    //            progressNum_++;
    //        }
    //    }
    //}
    //else if (progressNum_ == 5) // "�|���ƍ����_"
    //{
    //    // ���������͂��߂�܂ł̃t���[����҂��߁AkMaxFrame4StartTrans_�̒l�𒴂���܂ł́Arate�̌v�Z���o���Ȃ��悤�ɂ���
    //    float frame = (float)(std::max)(frameCount_sign_ - kMaxFrame4StartTrans_, 0);

    //    // ���݃t���[���ƍő�l�̊����Z�o
    //    float rate = (std::min)(frame / kMaxFrameTransColor_, 1.f);
    //    // ��������A���l��i�X���Z
    //    alpha_sign_ = (int32_t)((1 - rate) * 255);

    //    // �������񉼂�RB�����Ŏ��֐i�s
    //    if (PadInput::GetInstance().GetReleaseTrigger(GAMEPAD_RIGHT_SHOULDER))
    //    {
    //        // �Ŕp�t���[���J�E���^���Z
    //        frameCount_sign_++;
    //    }

    //    // �t���[���J�E���^���N������Ă�����Z��������
    //    if (frameCount_sign_)
    //    {
    //        // �Ŕp�t���[���J�E���^���Z
    //        frameCount_sign_++;

    //        // �t���[���J�E���^�̒l���A(���������n�߂�܂� + �����ɂȂ�܂� + ���̊Ԋu)����������
    //        if (frameCount_sign_ > kMaxFrame4StartTrans_ + kMaxFrameTransColor_ + kIntervalNextSign_)
    //        {
    //            // �J�E���^����������
    //            frameCount_sign_ = 0;
    //            // ���l����������
    //            alpha_sign_ = 255;
    //            // �\������Ŕ����̂��̂�
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
    //���U���g�摜
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
