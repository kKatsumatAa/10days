#include "GameScene.h"
#include "CollisionManger.h"
#include "SceneManager.h"
#include "KeyboardInput.h"
#include "PadInput.h"
#include "MathUtil.h"
#include "EnemyManager.h"
#include "ParticleManager.h"
#include "Score.h"
#include "GameVelocityManager.h"
#include "GameVelocityState.h"
#include "HitStop.h"
#include "CameraManager.h"

void GameScene::Finalize()
{
}

void GameScene::Initialize(void)
{
	//�S�̂̃J����
	CameraManager::GetInstance().AddCamera2D("GameCamera");
	CameraManager::GetInstance().SetUsingCamera2D("GameCamera");

	//BGM�Đ�
	Sound::GetInstance().PlayWave("play_BGM.wav", 1.0f, true);

	// (0,0) ~ (1280,720) ��肿�傢����
	stage_->Initialize({ 0,0 }, { 1280,720 });

	player_->SetPos({ 300,300 });
	player_->SetRot(0);
	player_->SetRad({ 10,0 });

	EnemyManager::GetInstance().Initialize(player_.get(), stage_.get());

	//timer_.Start(kMaxGameTimer_);
	timer_.Start(50000);	//��������50000�b��

	ParticleManagerL::GetInstance()->Init();

	//�Q�[���X�s�[�h
	GameVelocityManager::GetInstance().Initialize();

	drawNum_.Initialize(TextureManager::LoadGraph("number.png"));

	Score::GetInstance()->Init();
    GameSceneUpdate();
}

void GameScene::Update(void)
{
	//�ݒ肵�� (��������) - (�o�ߎ���)
	uint32_t timer = (uint32_t)timer_.GetEndTime() - (uint32_t)timer_.GetElapsedTime();
	drawNum_.SetNum(timer, {0,150}, {1.0f / 10.0f,1.0f}, {100,160}, 1.0f);	//�c��b���\��

	if (PadInput::GetInstance().GetTriggerButton(VK_GAMEPAD_MENU))
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
	}

	if (isMenu_ == false)
	{
		switch (progress_)
		{
		case GameScene::Progress::PRE:            // �t���[���J�E���g���K��l��������J��
			if (frameCount_preGame_ > kMaxFrame_preGame_)
			{
				// �i�s�l��GAME��
				progress_ = Progress::GAME;
			}
			break;
		case GameScene::Progress::GAME:
			break;
		case GameScene::Progress::POST:
			break;
		default:
			break;
		}

		if (progress_ == Progress::PRE)
		{
			frameCount_preGame_++;
		}
		else if (progress_ == Progress::GAME)
		{
			if (!HitStopManager::GetInstance().GetIsStop())
			{
				stage_->Update();

				player_->Update();
				EnemyManager::GetInstance().Update();

				if (KeyboardInput::GetInstance().KeyTrigger(DIK_0))
				{
					timer_.SetEndTime(10.f);
				}

				if (timer_.GetIsEnd())
				{
					Score::GetInstance()->HighScoreUpdate();

					Sound::GetInstance().PlayWave("sceneChange_SE.wav");
					//BGM�X�g�b�v
					Sound::GetInstance().StopWave("play_BGM.wav");
					SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::RESULT);
				}

				ParticleManagerL::GetInstance()->Update(GameVelocityManager::GetInstance().GetVelocity());

				CollisionManger::GetInstance()->Update();

				//�Q�[���X�s�[�h
				GameVelocityManager::GetInstance().Update();
			}

			HitStopManager::GetInstance().Update();
		}
	}
	else
	{
		if (PadInput::GetInstance().GetLeftStickTilt().y >= 0.3f)
		{
			destination_++;
			destination_ = (std::min)(destination_, 1);
		}
		else if (PadInput::GetInstance().GetLeftStickTilt().y <= -0.3f)
		{
			destination_--;
			destination_ = (std::max)(destination_, 0);
		}

		if (destination_ == Destination::RETRY)
		{
			if (PadInput::GetInstance().GetTriggerButton(GAMEPAD_A))
			{
				//PlaySoundMem(sceneChange_SE_, DX_PLAYTYPE_NORMAL);
				////BGM�X�g�b�v
				//StopSoundMem(game_BGM_);
				//SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::GAME);
			}
		}
		else if (destination_ == Destination::TITLE)
		{
			if (PadInput::GetInstance().GetTriggerButton(GAMEPAD_A))
			{
				//PlaySoundMem(sceneChange_SE_, DX_PLAYTYPE_NORMAL);
				////BGM�X�g�b�v
				//StopSoundMem(game_BGM_);
				//SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::TITLE);
			}
		}
	}


}

void GameScene::GameSceneUpdate(void)
{
    stage_->Update();

    player_->Update();
    EnemyManager::GetInstance().Update();

    ParticleManagerL::GetInstance()->Update(GameVelocityManager::GetInstance().GetVelocity());
}

void GameScene::Draw(void)
{

}

void GameScene::DrawSprite()
{
	stage_->Draw();

	ParticleManagerL::GetInstance()->Draw();

	player_->Draw();
	EnemyManager::GetInstance().Draw();

	drawNum_.Draw(CameraManager::GetInstance().GetCamera2D("UICamera"));

	////DrawFormatString(0, 380, UtilL::Color::RED, "Scene: GAME");
	////DrawFormatString(0, 0, UtilL::Color::WHITE, "[DEBUG]key-0�ŏI�����Ԃ�10�b�ɕύX�B���Ɍo�߂��Ă�ꍇ��GameScene�I��");
	////DrawFormatString(0, 20, UtilL::Color::WHITE, "time: %f", timer_.GetElapsedTime());
	////DrawPad();

	Score::GetInstance()->Draw();
}

void GameScene::DrawImgui()
{
	//�Q�[���X�s�[�h
	GameVelocityManager::GetInstance().UpdateImGui();
	//�q�b�g�X�g�b�v
	HitStopManager::GetInstance().DrawImGui();
}
