#include "TutorialScene.h"
#include "PadInput.h"
#include "SceneManager.h"
#include "EnemyManager.h"
#include "KeyboardInput.h"
#include "Score.h"
#include "ParticleManager.h"
#include "UI.h"
#include "GameVelocityState.h"
#include "GameVelocityManager.h"
#include "HitStop.h"
#include "CameraManager.h"

void TutorialScene::Initialize(void)
{
    //BGM�Đ�
    Sound::GetInstance().PlayWave("play_BGM.wav", 1.0f, true);

    // (0,0) ~ (1280,720) ��肿�傢����
    stage_->Initialize({ 10,10 }, { 1270,710 });

    player_->SetPos({ 640,360 });
    player_->SetRot(0);
    player_->SetRad({ 10,0 });
    player_->SetVecMove({ 0,-1 });

    EnemyManager::GetInstance().Initialize(player_.get(), stage_.get());

    //timer_.Start(kMaxGameTimer_);
    timer_.Start(1000000000);

    ParticleManagerL::GetInstance()->Init();

    Score::GetInstance()->Init();
    TutorialSceneUpdate();

    UI::GetInstance()->SetPos(UIType::Lstick, { 30.f,30.f });
    UI::GetInstance()->SetSize(UIType::Lstick, 0.2f);
    UI::GetInstance()->SetPos(UIType::Move, { 100.f,30.f });
    UI::GetInstance()->SetSize(UIType::Move, 0.2f);

    UI::GetInstance()->SetPos(UIType::Rbutton, { 30.f,80.f });
    UI::GetInstance()->SetSize(UIType::Rbutton, 0.2f);
    UI::GetInstance()->SetPos(UIType::Attack, { 100.f,80.f });
    UI::GetInstance()->SetSize(UIType::Attack, 0.2f);

    UI::GetInstance()->SetPos(UIType::Abutton, { 30.f,130.f });
    UI::GetInstance()->SetSize(UIType::Abutton, 0.2f);
    UI::GetInstance()->SetAncorPoint(UIType::Abutton, { 0.f,0.f });
    UI::GetInstance()->SetPos(UIType::Skewer, { 100.f,130.f });
    UI::GetInstance()->SetSize(UIType::Skewer, 0.2f);

    UI::GetInstance()->SetPos(UIType::Retry, { 640,300 });
    UI::GetInstance()->SetSize(UIType::Retry, 0.6f);
    UI::GetInstance()->SetAncorPoint(UIType::Retry, { 0.5f,0.5f });
    UI::GetInstance()->SetPos(UIType::ToTitle, { 640,500 });
    UI::GetInstance()->SetSize(UIType::ToTitle, 0.6f);
    UI::GetInstance()->SetAncorPoint(UIType::ToTitle, { 0.5f,0.5f });

    UI::GetInstance()->SetPos(UIType::Tutorial1, { 1000,500 });
    UI::GetInstance()->SetSize(UIType::Tutorial1, 0.6f);
    UI::GetInstance()->SetAncorPoint(UIType::Tutorial1, { 0.5f,0.5f });
    UI::GetInstance()->SetPos(UIType::Tutorial2, { 1000,300 });
    UI::GetInstance()->SetSize(UIType::Tutorial2, 0.6f);
    UI::GetInstance()->SetAncorPoint(UIType::Tutorial2, { 0.5f,0.5f });
    UI::GetInstance()->SetPos(UIType::Tutorial3, { 1000,500 });
    UI::GetInstance()->SetSize(UIType::Tutorial3, 0.6f);
    UI::GetInstance()->SetAncorPoint(UIType::Tutorial3, { 0.5f,0.5f });
    UI::GetInstance()->SetPos(UIType::Tutorial4, { 1000,500 });
    UI::GetInstance()->SetSize(UIType::Tutorial4, 0.6f);
    UI::GetInstance()->SetAncorPoint(UIType::Tutorial4, { 0.5f,0.5f });
    UI::GetInstance()->SetPos(UIType::Tutorial5, { 1000,500 });
    UI::GetInstance()->SetSize(UIType::Tutorial5, 0.6f);
    UI::GetInstance()->SetAncorPoint(UIType::Tutorial5, { 0.5f,0.5f });
    UI::GetInstance()->SetPos(UIType::Tutorial6, { 1000,500 });
    UI::GetInstance()->SetSize(UIType::Tutorial6, 0.6f);
    UI::GetInstance()->SetAncorPoint(UIType::Tutorial6, { 0.5f,0.5f });
    UI::GetInstance()->SetPos(UIType::Tutorial7, { 1000,500 });
    UI::GetInstance()->SetSize(UIType::Tutorial7, 0.6f);
    UI::GetInstance()->SetAncorPoint(UIType::Tutorial7, { 0.5f,0.5f });
}

void TutorialScene::Update(void)
{
    if (PadInput::GetInstance().GetTriggerButton(GAMEPAD_START))
    {
        if (isMenu_)
        {
            isMenu_ = false;
        }
        else
        {
            isMenu_ = true;
            // ���h���p�Ƀ{�^�������Ă����������p�֐�
            player_->ResetSkewerInfo4Pause();
        }

        if (destination_ == Destination::RETRY)
        {
            UI::GetInstance()->SetColor(UIType::ToTitle, { 1,1,1,0.3f });
            UI::GetInstance()->SetSize(UIType::ToTitle, 0.4f);
            UI::GetInstance()->SetColor(UIType::Retry, { 1,1,1,1 });
            UI::GetInstance()->SetSize(UIType::Retry, 1);
        }
        else if (destination_ == Destination::TITLE)
        {
            UI::GetInstance()->SetColor(UIType::Retry, { 1,1,1,0.3f });
            UI::GetInstance()->SetSize(UIType::Retry, 0.4f);
            UI::GetInstance()->SetColor(UIType::ToTitle, { 1,1,1,1 });
            UI::GetInstance()->SetSize(UIType::ToTitle, 1);
        }
    }

    if (PadInput::GetInstance().GetPushButton(GAMEPAD_B)) // pad-B�������Ń`���[�g���A���X�L�b�v�p�̃J�E���^���Z
    {
        frameCount_skipTutorial_++;
        if (frameCount_skipTutorial_ > kMaxFrameSkipTutorial_)
        {
            Sound::GetInstance().PlayWave("sceneChange_SE.wav");
            //BGM�X�g�b�v
            Sound::GetInstance().StopWave("play_BGM.wav");
            SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::GAME);
        }
    }
    else // �����񂭂Ȃ����珉����
    {
        // ������
        frameCount_skipTutorial_ = 0;
    }

    if (isMenu_ == false)
    {
        stage_->Update();

        player_->Update();
        EnemyManager::GetInstance().Update();

#ifdef _DEBUG
        if (KeyboardInput::GetInstance().KeyTrigger(DIK_0))
        {
            timer_.SetEndTime(10.f);
        }
#endif // _DEBUG

        if (timer_.GetIsEnd())
        {
            Score::GetInstance()->HighScoreUpdate();

            Sound::GetInstance().PlayWave("sceneChange_SE.wav");
            //BGM�X�g�b�v
            Sound::GetInstance().StopWave("play_BGM.wav");
            SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::GAME);
        }

        ParticleManagerL::GetInstance()->Update(GameVelocityManager::GetInstance().GetVelocity());

        if (progressNum_ == 1)
        {
            // ���������͂��߂�܂ł̃t���[����҂��߁AkMaxFrame4StartTrans_�̒l�𒴂���܂ł́Arate�̌v�Z���o���Ȃ��悤�ɂ���
            float frame = (float)(std::max)(frameCount_sign_ - kMaxFrame4StartTrans_, 0);

            // ���݃t���[���ƍő�l�̊����Z�o
            float rate = (std::min)(frame / kMaxFrameTransColor_, 1.f);
            // ��������A���l��i�X���Z
            alpha_sign_ = (int32_t)((1 - rate) * 255);

            // �������񉼂�RB�����Ŏ��֐i�s
            if (PadInput::GetInstance().GetTriggerButton(GAMEPAD_RIGHT_SHOULDER))
            {
                // �Ŕp�t���[���J�E���^���Z
                frameCount_sign_++;
            }

            // �t���[���J�E���^���N������Ă�����Z��������
            if (frameCount_sign_)
            {
                // �Ŕp�t���[���J�E���^���Z
                frameCount_sign_++;

                // �t���[���J�E���^�̒l���A(���������n�߂�܂� + �����ɂȂ�܂� + ���̊Ԋu)����������
                if (frameCount_sign_ > kMaxFrame4StartTrans_ + kMaxFrameTransColor_ + kIntervalNextSign_)
                {
                    // �J�E���^����������
                    frameCount_sign_ = 0;
                    // ���l����������
                    alpha_sign_ = 255;
                    // �\������Ŕ����̂��̂�
                    progressNum_++;
                }
            }
        }
        else if (progressNum_ == 2)
        {
            // ���������͂��߂�܂ł̃t���[����҂��߁AkMaxFrame4StartTrans_�̒l�𒴂���܂ł́Arate�̌v�Z���o���Ȃ��悤�ɂ���
            float frame = (float)(std::max)(frameCount_sign_ - kMaxFrame4StartTrans_, 0);

            // ���݃t���[���ƍő�l�̊����Z�o
            float rate = (std::min)(frame / kMaxFrameTransColor_, 1.f);
            // ��������A���l��i�X���Z
            alpha_sign_ = (int32_t)((1 - rate) * 255);

            // �������񉼂�A���� && �X�e�B�b�N���͂��Ă���
            if (PadInput::GetInstance().GetPushButton(GAMEPAD_A) && PadInput::GetInstance().GetLeftStickTilt().x || PadInput::GetInstance().GetLeftStickTilt().y) // �����X���[���[�V���������邹���Ŏ��Ԃ����肷���A�G���W���Ȃ炱���͂Ȃ�Ȃ��Ǝv������
            {
                // �����̉��Z�͍ŏ��̈��݂̂ɂ�����
                if (frameCount_sign_ == 0)
                {
                    // �Ŕp�t���[���J�E���^���Z
                    frameCount_sign_++;
                }
            }

            // �t���[���J�E���^���N������Ă�����Z��������
            if (frameCount_sign_)
            {
                // �Ŕp�t���[���J�E���^���Z
                frameCount_sign_++;

                // �t���[���J�E���^�̒l���A(���������n�߂�܂� + �����ɂȂ�܂� + ���̊Ԋu)����������
                if (frameCount_sign_ > kMaxFrame4StartTrans_ + kMaxFrameTransColor_ + kIntervalNextSign_)
                {
                    // �J�E���^����������
                    frameCount_sign_ = 0;
                    // ���l����������
                    alpha_sign_ = 255;
                    // �\������Ŕ����̂��̂�
                    progressNum_++;
                }
            }
        }
        else if (progressNum_ == 3)
        {
            // ���������͂��߂�܂ł̃t���[����҂��߁AkMaxFrame4StartTrans_�̒l�𒴂���܂ł́Arate�̌v�Z���o���Ȃ��悤�ɂ���
            float frame = (float)(std::max)(frameCount_sign_ - kMaxFrame4StartTrans_, 0);

            // ���݃t���[���ƍő�l�̊����Z�o
            float rate = (std::min)(frame / kMaxFrameTransColor_, 1.f);
            // ��������A���l��i�X���Z
            alpha_sign_ = (int32_t)((1 - rate) * 255);

            // �������񉼂�pad-A��Release��
            if (PadInput::GetInstance().GetReleaseTrigger(GAMEPAD_A)) // �����̋���������Ɖ�����
            {
                // �Ŕp�t���[���J�E���^���Z
                frameCount_sign_++;
            }

            // �t���[���J�E���^���N������Ă�����Z��������
            if (frameCount_sign_)
            {
                // �Ŕp�t���[���J�E���^���Z
                frameCount_sign_++;

                // �t���[���J�E���^�̒l���A(���������n�߂�܂� + �����ɂȂ�܂� + ���̊Ԋu)����������
                if (frameCount_sign_ > kMaxFrame4StartTrans_ + kMaxFrameTransColor_ + kIntervalNextSign_)
                {
                    // �J�E���^����������
                    frameCount_sign_ = 0;
                    // ���l����������
                    alpha_sign_ = 255;
                    // �\������Ŕ����̂��̂�
                    progressNum_++;
                }
            }
        }
        else if (progressNum_ == 4) // "8�̂������ăf�J�c�q�ɁI"
        {
            // ���������͂��߂�܂ł̃t���[����҂��߁AkMaxFrame4StartTrans_�̒l�𒴂���܂ł́Arate�̌v�Z���o���Ȃ��悤�ɂ���
            float frame = (float)(std::max)(frameCount_sign_ - kMaxFrame4StartTrans_, 0);

            // ���݃t���[���ƍő�l�̊����Z�o
            float rate = (std::min)(frame / kMaxFrameTransColor_, 1.f);
            // ��������A���l��i�X���Z
            alpha_sign_ = (int32_t)((1 - rate) * 255);

            // �������񉼂�RB�����Ŏ��֐i�s
            if (PadInput::GetInstance().GetReleaseTrigger(GAMEPAD_RIGHT_SHOULDER))
            {
                // �Ŕp�t���[���J�E���^���Z
                frameCount_sign_++;
            }

            // �t���[���J�E���^���N������Ă�����Z��������
            if (frameCount_sign_)
            {
                // �Ŕp�t���[���J�E���^���Z
                frameCount_sign_++;

                // �t���[���J�E���^�̒l���A(���������n�߂�܂� + �����ɂȂ�܂� + ���̊Ԋu)����������
                if (frameCount_sign_ > kMaxFrame4StartTrans_ + kMaxFrameTransColor_ + kIntervalNextSign_)
                {
                    // �J�E���^����������
                    frameCount_sign_ = 0;
                    // ���l����������
                    alpha_sign_ = 255;
                    // �\������Ŕ����̂��̂�
                    progressNum_++;
                }
            }
        }
        else if (progressNum_ == 5) // "�|���ƍ����_"
        {
            // ���������͂��߂�܂ł̃t���[����҂��߁AkMaxFrame4StartTrans_�̒l�𒴂���܂ł́Arate�̌v�Z���o���Ȃ��悤�ɂ���
            float frame = (float)(std::max)(frameCount_sign_ - kMaxFrame4StartTrans_, 0);

            // ���݃t���[���ƍő�l�̊����Z�o
            float rate = (std::min)(frame / kMaxFrameTransColor_, 1.f);
            // ��������A���l��i�X���Z
            alpha_sign_ = (int32_t)((1 - rate) * 255);

            // �������񉼂�RB�����Ŏ��֐i�s
            if (PadInput::GetInstance().GetReleaseTrigger(GAMEPAD_RIGHT_SHOULDER))
            {
                // �Ŕp�t���[���J�E���^���Z
                frameCount_sign_++;
            }

            // �t���[���J�E���^���N������Ă�����Z��������
            if (frameCount_sign_)
            {
                // �Ŕp�t���[���J�E���^���Z
                frameCount_sign_++;

                // �t���[���J�E���^�̒l���A(���������n�߂�܂� + �����ɂȂ�܂� + ���̊Ԋu)����������
                if (frameCount_sign_ > kMaxFrame4StartTrans_ + kMaxFrameTransColor_ + kIntervalNextSign_)
                {
                    // �J�E���^����������
                    frameCount_sign_ = 0;
                    // ���l����������
                    alpha_sign_ = 255;
                    // �\������Ŕ����̂��̂�
                    progressNum_++;
                }
            }
        }
        else if (progressNum_ == 6) // "����ł͂����^��"
        {
            // ���������͂��߂�܂ł̃t���[����҂��߁AkMaxFrame4StartTrans_�̒l�𒴂���܂ł́Arate�̌v�Z���o���Ȃ��悤�ɂ���
            float frame = (float)(std::max)(frameCount_sign_ - kMaxFrame4StartTrans_, 0);

            // ���݃t���[���ƍő�l�̊����Z�o
            float rate = (std::min)(frame / kMaxFrameTransColor_, 1.f);
            // ��������A���l��i�X���Z
            alpha_sign_ = (int32_t)((1 - rate) * 255);

            // �������񉼂�RB�����Ŏ��֐i�s
            if (PadInput::GetInstance().GetReleaseTrigger(GAMEPAD_RIGHT_SHOULDER))
            {
                // �Ŕp�t���[���J�E���^���Z
                frameCount_sign_++;
            }

            // �t���[���J�E���^���N������Ă�����Z��������
            if (frameCount_sign_)
            {
                // �Ŕp�t���[���J�E���^���Z
                frameCount_sign_++;

                // �t���[���J�E���^�̒l���A(���������n�߂�܂� + �����ɂȂ�܂� + ���̊Ԋu)����������
                if (frameCount_sign_ > kMaxFrame4StartTrans_ + kMaxFrameTransColor_ + kIntervalNextSign_)
                {
                    // �J�E���^����������
                    frameCount_sign_ = 0;
                    // ���l����������
                    alpha_sign_ = 255;
                    // �\������Ŕ����̂��̂�
                    progressNum_++;

                    Sound::GetInstance().PlayWave("sceneChange_SE.wav");
                    //BGM�X�g�b�v
                    Sound::GetInstance().StopWave("play_BGM.wav");
                    SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::GAME);
                }
            }
        }
    }
    else
    {
        if (PadInput::GetInstance().GetLeftStickTilt().y >= 0.3f)
        {
            destination_++;
            destination_ = (std::min)(destination_, 1);
            UI::GetInstance()->SetColor(UIType::Retry, { 1,1,1,0.3f });
            UI::GetInstance()->SetSize(UIType::Retry, 0.4f);
            UI::GetInstance()->SetColor(UIType::ToTitle, { 1,1,1,1 });
            UI::GetInstance()->SetSize(UIType::ToTitle, 1);
        }
        else if (PadInput::GetInstance().GetLeftStickTilt().y <= -0.3f)
        {
            destination_--;
            destination_ = (std::max)(destination_, 0);
            UI::GetInstance()->SetColor(UIType::ToTitle, { 1,1,1,0.3f });
            UI::GetInstance()->SetSize(UIType::ToTitle, 0.4f);
            UI::GetInstance()->SetColor(UIType::Retry, { 1,1,1,1 });
            UI::GetInstance()->SetSize(UIType::Retry, 1);
        }

        if (destination_ == Destination::RETRY)
        {
            if (PadInput::GetInstance().GetTriggerButton(GAMEPAD_A))
            {
                Score::GetInstance()->HighScoreUpdate();

                Sound::GetInstance().PlayWave("sceneChange_SE.wav");
                //BGM�X�g�b�v
                Sound::GetInstance().StopWave("play_BGM.wav");
                SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::GAME);
            }
        }
        else if (destination_ == Destination::TITLE)
        {
            if (PadInput::GetInstance().GetTriggerButton(GAMEPAD_A))
            {
                Score::GetInstance()->HighScoreUpdate();

                Sound::GetInstance().PlayWave("sceneChange_SE.wav");
                //BGM�X�g�b�v
                Sound::GetInstance().StopWave("play_BGM.wav");
                SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::TITLE);
            }
        }
    }
}

void TutorialScene::TutorialSceneUpdate(void)
{
    stage_->Update();

    player_->Update();
    EnemyManager::GetInstance().Update();

    ParticleManagerL::GetInstance()->Update(GameVelocityManager::GetInstance().GetVelocity());
}

void TutorialScene::DrawSprite(void)
{
    stage_->Draw();

    player_->Draw();
    EnemyManager::GetInstance().Draw();

    ParticleManagerL::GetInstance()->Draw();

    //DrawPad();

    Score::GetInstance()->Draw();

    UI::GetInstance()->Draw(UIType::Lstick);
    UI::GetInstance()->Draw(UIType::Move);
    UI::GetInstance()->Draw(UIType::Rbutton);
    UI::GetInstance()->Draw(UIType::Attack);
    UI::GetInstance()->Draw(UIType::Abutton);
    UI::GetInstance()->Draw(UIType::Skewer);
    //SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
    //DrawBox(0, 0, 1280, 720, Util::Color::BLACK, true);
    //SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void TutorialScene::DrawSprite2(void)
{
    if (progressNum_ == 1)
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
    }
    else if (progressNum_ == 6)
    {
        UI::GetInstance()->Draw(UIType::Tutorial7);
    }

    if (isMenu_)
    {
        UI::GetInstance()->Draw(UIType::Retry);
        UI::GetInstance()->Draw(UIType::ToTitle);
    }
}
