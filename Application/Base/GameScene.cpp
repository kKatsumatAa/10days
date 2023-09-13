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
	Sound::GetInstance().PlayWave("play_BGM.wav", 0.2f, true);

	// (0,0) ~ (1280,720) よりちょい内側
	stage_->Initialize({ 0,0 }, { 1280,720 });

	player_->SetPos({ 640,360 });
	player_->SetRot(0);
	player_->SetRad({ 10,0 });
	player_->SetVecMove({ 0,-1 });
	player_->Initialize();

	EnemyManager::GetInstance().Initialize(player_.get(), stage_.get());
	//敵のデータ読み込み
	EnemyManager::GetInstance().LoadEnemiesDataCSV("gameEnemies.csv");

	ParticleManagerL::GetInstance()->Init();

	//ゲームスピード
	GameVelocityManager::GetInstance().Initialize();

	drawNum_.Initialize(TextureManager::LoadGraph("number.png"));

	Score::GetInstance()->Init();
	Score::GetInstance()->SetNowPos({ 1250.f,20.f });
	Score::GetInstance()->SetNowSize(0.6f);
	Score::GetInstance()->SetNowNum();
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

	UI::GetInstance()->SetPos(UIType::Retry, { 740,300 });
	UI::GetInstance()->SetSize(UIType::Retry, 0.6f);
	UI::GetInstance()->SetAncorPoint(UIType::Retry, { 0.5f,0.5f });
	UI::GetInstance()->SetColor(UIType::Retry, { 1.f,1.f,1.f,0.7f });
	UI::GetInstance()->SetPos(UIType::ToTitle, { 740,500 });
	UI::GetInstance()->SetSize(UIType::ToTitle, 0.6f);
	UI::GetInstance()->SetAncorPoint(UIType::ToTitle, { 0.5f,0.5f });
	UI::GetInstance()->SetColor(UIType::ToTitle, { 1.f,1.f,1.f,0.7f });

	UI::GetInstance()->SetPos(UIType::ReadyGo, { 740,360 });
	UI::GetInstance()->SetSize(UIType::ReadyGo, 2.4f);
	UI::GetInstance()->SetAncorPoint(UIType::ReadyGo, { 0.5f,0.5f });

	UI::GetInstance()->SetPos(UIType::Number1, { 680,500 });
	UI::GetInstance()->SetSize(UIType::Number1, 0.6f);
	UI::GetInstance()->SetColor(UIType::Number1, { 1.f,1.f,1.f,0.7f });
	UI::GetInstance()->SetAncorPoint(UIType::Number1, { 0.5f,0.5f });
	UI::GetInstance()->SetPos(UIType::Number2, { 680,500 });
	UI::GetInstance()->SetSize(UIType::Number2, 0.6f);
	UI::GetInstance()->SetColor(UIType::Number2, { 1.f,1.f,1.f,0.7f });
	UI::GetInstance()->SetAncorPoint(UIType::Number2, { 0.5f,0.5f });
	UI::GetInstance()->SetPos(UIType::Number3, { 680,500 });
	UI::GetInstance()->SetSize(UIType::Number3, 0.6f);
	UI::GetInstance()->SetColor(UIType::Number3, { 1.f,1.f,1.f,0.7f });
	UI::GetInstance()->SetAncorPoint(UIType::Number3, { 0.5f,0.5f });
}

void GameScene::Update(void)
{
	//設定した (制限時間) - (経過時間)
	int32_t timer = int32_t(3.f - countdownTimer_.GetTimer() / 60);

	int32_t nTimer = uint32_t(nTimer_.GetMaxTimer() / 60 - nTimer_.GetTimer() / 60);
	drawNum_.SetNum(nTimer, { 200,10 }, { 1.0f / 10.0f,1.0f }, { 100,160 }, 0.85f);	//残り秒数表示

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

		Sound::GetInstance().PlayWave("decision_SE.wav", 0.5f);
	}

	if (isMenu_ == false)
	{
		switch (progress_)
		{
		case GameScene::Progress::PRE:            // フレームカウントが規定値超えたら遷移
			if (countdownTimer_.GetisTimeOut())
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
			// ゲーム開始前のカウントダウンのアプデ
			countdownTimer_.Update(false, 1);

			// カウントダウン後の"始め!"の文字表示時間のアプデ
			if (countdownTimer_.GetisTimeOut())
			{
				ReadyGoTimer_.Update(false, 1);
			}
		}
		else if (progress_ == Progress::GAME)
		{
			// ゲーム時間のアプデ
			if (HitStopManager::GetInstance().GetIsStop())
			{
				nTimer_.Update(false, 0.f);
			}
			else
			{
				nTimer_.Update(false, GameVelocityManager::GetInstance().GetVelocity());
			}

			// カウントダウン後の"始め!"の文字表示時間のアプデ
			if (countdownTimer_.GetisTimeOut())
			{
				ReadyGoTimer_.Update(false, 1);
			}

			if (!HitStopManager::GetInstance().GetIsStop())
			{
				stage_->Update();

				player_->Update();
				EnemyManager::GetInstance().Update();

				if (nTimer_.GetisTimeOut())
				{
					Score::GetInstance()->HighScoreUpdate();

					Sound::GetInstance().PlayWave("decision_SE.wav", 0.2f);
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
		// ゲーム時間のアプデ
		countdownTimer_.Update(false, 0);
		nTimer_.Update(false, 0.f);
		ReadyGoTimer_.Update(false, 0);

		if (PadInput::GetInstance().GetLeftStickTilt().y >= 0.3f)
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
			UI::GetInstance()->SetSize(UIType::ToTitle, 1);
		}
		else if (PadInput::GetInstance().GetLeftStickTilt().y <= -0.3f)
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
			UI::GetInstance()->SetSize(UIType::Retry, 1);
		}

		if (destination_ == Destination::RETRY)
		{
			if (PadInput::GetInstance().GetTriggerButton(GAMEPAD_A))
			{
				Score::GetInstance()->HighScoreUpdate();

				Sound::GetInstance().PlayWave("decision_SE.wav", 0.2f);
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

				Sound::GetInstance().PlayWave("decision_SE.wav", 0.2f);
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
	if (countdownTimer_.GetisTimeOut() == false)
	{
		if (countdownTimer_.GetTimer() < 60)
		{
			float rate = (std::min)(countdownTimer_.GetTimer() / 60, 1.f);
			UI::GetInstance()->SetSize(UIType::Number3, Math::Ease::EaseOutQuint(rate, 1.f, 2.2f));
			UI::GetInstance()->SetPos(UIType::Number3, { 640.f,Math::Ease::EaseOutQuint(rate, 0.f, 340.f) });
			UI::GetInstance()->Draw2(UIType::Number3, 0.f);
		}
		else if (countdownTimer_.GetTimer() >= 60 && countdownTimer_.GetTimer() < 120)
		{
			float rate = (std::min)((countdownTimer_.GetTimer() - 60.f) / 60, 1.f);
			UI::GetInstance()->SetSize(UIType::Number2, Math::Ease::EaseOutQuint(rate, 1.f, 2.2f));
			UI::GetInstance()->SetPos(UIType::Number2, { 640.f,Math::Ease::EaseOutQuint(rate, 0.f, 340.f) });
			UI::GetInstance()->Draw2(UIType::Number2, 0.f);
		}
		else if (countdownTimer_.GetTimer() >= 120)
		{
			float rate = (std::min)((countdownTimer_.GetTimer() - 120.f) / 60, 1.f);
			UI::GetInstance()->SetSize(UIType::Number1, Math::Ease::EaseOutQuint(rate, 1.f, 2.2f));
			UI::GetInstance()->SetPos(UIType::Number1, { 640.f,Math::Ease::EaseOutQuint(rate, 0.f, 340.f) });
			UI::GetInstance()->Draw2(UIType::Number1, 0.f);
		}

	}
	else if (ReadyGoTimer_.GetisTimeOut() == false)
	{
		float rate = (std::min)(ReadyGoTimer_.GetTimer() / 60, 1.f);
		float rate2 = (std::min)(ReadyGoTimer_.GetTimer() / 20, 1.f);
		UI::GetInstance()->SetSize(UIType::ReadyGo, Math::Ease::EaseOutQuint(rate, 0.2f, 2.f));
		float alpha = (std::max)(0.8f - ReadyGoTimer_.GetTimer() / kReadyGoTimer_ * 4, 0.f);
		UI::GetInstance()->SetColor(UIType::ReadyGo, { 1,1,1,alpha });
		UI::GetInstance()->Draw2(UIType::ReadyGo, rate2 * -Math::Function::ToRadian(740));
	}

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
