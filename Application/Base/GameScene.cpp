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

void GameScene::Finalize()
{
}

void GameScene::Initialize(void)
{
	//BGM再生
	Sound::GetInstance().PlayWave("play_BGM.wav", 1.0f, true);

	// (0,0) ~ (1280,720) よりちょい内側
	stage_->Initialize({ 10,10 }, { 1270,710 });

	player_->SetPos({ 300,300 });
	player_->SetRot(0);
	player_->SetRad({ 10,0 });

	EnemyManager::GetInstance().Initialize(player_.get(), stage_.get());

	//timer_.Start(kMaxGameTimer_);
	timer_.Start(1000000000);

	ParticleManagerL::GetInstance()->Init();

	//ゲームスピード
	GameVelocityManager::GetInstance().Initialize();

	Score::Init();
	Update();
}

void GameScene::Update(void)
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
		Score::HighScoreUpdate();

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

void GameScene::Draw(void)
{

}

void GameScene::DrawSprite()
{
	stage_->Draw();

	player_->Draw();
	EnemyManager::GetInstance().Draw();

	ParticleManagerL::GetInstance()->Draw();

	////DrawFormatString(0, 380, UtilL::Color::RED, "Scene: GAME");
	////DrawFormatString(0, 0, UtilL::Color::WHITE, "[DEBUG]key-0で終了時間を10秒に変更。既に経過してる場合はGameScene終了");
	////DrawFormatString(0, 20, UtilL::Color::WHITE, "time: %f", timer_.GetElapsedTime());
	////DrawPad();

	//Score::Draw();
}

void GameScene::DrawImgui()
{
	//ゲームスピード
	GameVelocityManager::GetInstance().UpdateImGui();
}
