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
	//BGM�Đ�
	Sound::GetInstance().PlayWave("play_BGM.wav", 1.0f, true);

	// (0,0) ~ (1280,720) ��肿�傢����
	stage_->Initialize({ 10,10 }, { 1270,710 });

	player_->SetPos({ 300,300 });
	player_->SetRot(0);
	player_->SetRad({ 10,0 });

	EnemyManager::GetInstance().Initialize(player_.get(), stage_.get());

	//timer_.Start(kMaxGameTimer_);
	timer_.Start(1000000000);

	ParticleManagerL::GetInstance()->Init();

	//�Q�[���X�s�[�h
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
		//BGM�X�g�b�v
		Sound::GetInstance().StopWave("play_BGM.wav");
		SceneManager::GetInstance().SetNextScene(SceneFactory::Usage::RESULT);
	}

	ParticleManagerL::GetInstance()->Update(GameVelocityManager::GetInstance().GetVelocity());

	CollisionManger::GetInstance()->Update();

	//�Q�[���X�s�[�h
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
	////DrawFormatString(0, 0, UtilL::Color::WHITE, "[DEBUG]key-0�ŏI�����Ԃ�10�b�ɕύX�B���Ɍo�߂��Ă�ꍇ��GameScene�I��");
	////DrawFormatString(0, 20, UtilL::Color::WHITE, "time: %f", timer_.GetElapsedTime());
	////DrawPad();

	//Score::Draw();
}

void GameScene::DrawImgui()
{
	//�Q�[���X�s�[�h
	GameVelocityManager::GetInstance().UpdateImGui();
}
