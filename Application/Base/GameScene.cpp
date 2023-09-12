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
#include "UI.h"

void GameScene::Finalize()
{
}

void GameScene::Initialize(void)
{
	PostEffectManager::GetInstance().GetPostEffect2()->effectFlags_.isGrayScale = false;

	//BGM再生
	Sound::GetInstance().PlayWave("play_BGM.wav", 1.0f, true);

	// (0,0) ~ (1280,720) よりちょい内側
	stage_->Initialize({ 0,0 }, { 1280,720 });

    player_->SetPos({ 640,360 });
    player_->SetRot(0);
    player_->SetRad({ 10,0 });
    player_->SetVecMove({ 0,-1 });
	player_->Initialize();

	EnemyManager::GetInstance().Initialize(player_.get(), stage_.get());

	ParticleManagerL::GetInstance()->Init();

	//ゲームスピード
	GameVelocityManager::GetInstance().Initialize();

	drawNum_.Initialize(TextureManager::LoadGraph("number.png"));

	Score::GetInstance()->Init();
    GameSceneUpdate();

	float opeUIPlusY = 260.0f;	//操作UIのY座標ずらす用
	UI::GetInstance()->SetPos(UIType::Makimono, { 0.f,0.f });
	UI::GetInstance()->SetColor(UIType::Makimono, { 0.7f,0.7f,0.7f,1.f });

	UI::GetInstance()->SetPos(UIType::Time, { 100.f,180.f });
	UI::GetInstance()->SetSize(UIType::Time, 0.2f);
	UI::GetInstance()->SetAncorPoint(UIType::Time, { 0.5f,0.5f });

	UI::GetInstance()->SetPos(UIType::Menubutton, { 100.f,0.f + opeUIPlusY });
	UI::GetInstance()->SetSize(UIType::Menubutton, 0.3f);
	UI::GetInstance()->SetAncorPoint(UIType::Menubutton, { 0.5f,0.5f });
	UI::GetInstance()->SetPos(UIType::Pause, { 100.f,50.f + opeUIPlusY });
	UI::GetInstance()->SetSize(UIType::Pause, 0.21f);
	UI::GetInstance()->SetAncorPoint(UIType::Pause, { 0.5f,0.5f });

	UI::GetInstance()->SetPos(UIType::Lstick, { 100.f,120.f + opeUIPlusY });
	UI::GetInstance()->SetSize(UIType::Lstick, 0.3f);
	UI::GetInstance()->SetAncorPoint(UIType::Lstick, { 0.5f,0.5f });
	UI::GetInstance()->SetPos(UIType::Move, { 100.f,170.f + opeUIPlusY });
	UI::GetInstance()->SetSize(UIType::Move, 0.2f);
	UI::GetInstance()->SetAncorPoint(UIType::Move, { 0.5f,0.5f });

	UI::GetInstance()->SetPos(UIType::Rbutton, { 100.f,240.f + opeUIPlusY });
	UI::GetInstance()->SetSize(UIType::Rbutton, 0.3f);
	UI::GetInstance()->SetAncorPoint(UIType::Rbutton, { 0.5f,0.5f });
	UI::GetInstance()->SetPos(UIType::Attack, { 100.f,290.f + opeUIPlusY });
	UI::GetInstance()->SetSize(UIType::Attack, 0.2f);
	UI::GetInstance()->SetAncorPoint(UIType::Attack, { 0.5f,0.5f });

	UI::GetInstance()->SetPos(UIType::Abutton, { 100.f,360.f + opeUIPlusY });
	UI::GetInstance()->SetSize(UIType::Abutton, 0.3f);
	UI::GetInstance()->SetAncorPoint(UIType::Abutton, { 0.5f,0.5f });
	UI::GetInstance()->SetPos(UIType::Skewer, { 100.f,410.f + opeUIPlusY });
	UI::GetInstance()->SetSize(UIType::Skewer, 0.2f);
	UI::GetInstance()->SetAncorPoint(UIType::Skewer, { 0.5f,0.5f });

	UI::GetInstance()->SetPos(UIType::Retry, { 640,300 });
	UI::GetInstance()->SetSize(UIType::Retry, 0.6f);
	UI::GetInstance()->SetAncorPoint(UIType::Retry, { 0.5f,0.5f });
	UI::GetInstance()->SetPos(UIType::ToTitle, { 640,500 });
	UI::GetInstance()->SetSize(UIType::ToTitle, 0.6f);
	UI::GetInstance()->SetAncorPoint(UIType::ToTitle, { 0.5f,0.5f });
}

void GameScene::Update(void)
{
    if (HitStopManager::GetInstance().GetIsStop())
    {
        nTimer_.Update(false,0.f);
    }
    else
    {
        nTimer_.Update(false, GameVelocityManager::GetInstance().GetVelocity());
    }

	//設定した (制限時間) - (経過時間)
    int32_t nTimer = uint32_t(1000.f - nTimer_.GetTimer() / 60);
	drawNum_.SetNum(nTimer, { 300,150 }, { 1.0f / 10.0f,1.0f }, { 100,160 }, 1.0f);	//残り秒数表示

	if (PadInput::GetInstance().GetTriggerButton(GAMEPAD_START))
	{
		if (isMenu_)
		{
			isMenu_ = false;
		}
		else
		{
			isMenu_ = true;
			// 串刺し用にボタン押してたら解除する用関数
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

	if (isMenu_ == false)
	{
		switch (progress_)
		{
		case GameScene::Progress::PRE:            // フレームカウントが規定値超えたら遷移
			if (frameCount_preGame_ > kMaxFrame_preGame_)
			{
				// 進行値をGAMEに
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

				if (nTimer_.GetisTimeOut())
				{
					Score::GetInstance()->HighScoreUpdate();

					Sound::GetInstance().PlayWave("sceneChange_SE.wav");
					//BGMストップ
					Sound::GetInstance().StopWave("play_BGM.wav");
					SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::RESULT);
				}

				ParticleManagerL::GetInstance()->Update(GameVelocityManager::GetInstance().GetVelocity());

				CollisionManger::GetInstance()->Update();

				//ゲームスピード
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
				//BGMストップ
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
				//BGMストップ
				Sound::GetInstance().StopWave("play_BGM.wav");
				SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::TITLE);
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
}

void GameScene::DrawSprite2()
{

	Score::GetInstance()->Draw();

	UI::GetInstance()->Draw(UIType::Makimono);
	drawNum_.Draw(CameraManager::GetInstance().GetCamera2D("UICamera"));
	UI::GetInstance()->Draw(UIType::Time);
	UI::GetInstance()->Draw(UIType::Menubutton);
	UI::GetInstance()->Draw(UIType::Pause);
	UI::GetInstance()->Draw(UIType::Lstick);
	UI::GetInstance()->Draw(UIType::Move);
	UI::GetInstance()->Draw(UIType::Rbutton);
	UI::GetInstance()->Draw(UIType::Attack);
	UI::GetInstance()->Draw(UIType::Abutton);
	UI::GetInstance()->Draw(UIType::Skewer);

	drawNum_.Draw(CameraManager::GetInstance().GetCamera2D("UICamera"));
	if (isMenu_)
	{
		UI::GetInstance()->Draw(UIType::Retry);
		UI::GetInstance()->Draw(UIType::ToTitle);
	}
}

void GameScene::DrawImgui()
{
	//ゲームスピード
	GameVelocityManager::GetInstance().UpdateImGui();
	//ヒットストップ
	HitStopManager::GetInstance().DrawImGui();
}
