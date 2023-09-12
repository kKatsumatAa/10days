#pragma once
#include <memory>
#include "SceneState.h"
#include "Player.h"
#include "Enemy.h"
#include "UtilL.h"
#include "Stage.h"
#include "CollisionManger.h"
#include "DrawIntNumImage.h"

class GameScene :
	public SceneState
{
public:
	// 定数
	float kMaxGameTimer_{ 60.f };
	const int32_t kMaxFrame_preGame_{ 180 }; // ゲーム前イベントが何フレームか

	// 定義
	enum class Progress
	{
		PRE,
		GAME,
		POST,
	};

	enum Destination
	{
		RETRY,
		TITLE,
	};

	// 関数
	void Finalize()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void DrawSprite()override;

	void DrawImgui()override;

private:
	uint32_t game_BGM_ = 0;
	uint32_t sceneChange_SE_ = 0;   //シーン遷移音

	// 変数
	std::unique_ptr<Stage> stage_{ std::make_unique<Stage>() };

	std::unique_ptr<Player> player_{ std::make_unique<Player>(CollisionManger::GetInstance(), stage_.get()) };
	UtilL::Timer timer_{};

	Progress progress_;
	int32_t frameCount_preGame_; // ゲーム前イベント用のフレームカウンタ。

	bool isMenu_;
	int32_t destination_;

	//テスト用
	DrawIntNumImage drawNum_;
	uint32_t num_ = 114514;
};

