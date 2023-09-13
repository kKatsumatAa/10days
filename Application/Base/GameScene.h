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
    const float kReadyGoTimer_{ 500 };

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
    void GameSceneUpdate(void);
    void Draw()override;
    void DrawSprite()override;
    void DrawSprite2()override;
    void DrawImgui()override;

private:
    uint32_t game_BGM_ = 0;
    uint32_t sceneChange_SE_ = 0;   //シーン遷移音

    // 変数
    std::unique_ptr<Stage> stage_{ std::make_unique<Stage>() };

    std::unique_ptr<Player> player_{ std::make_unique<Player>(CollisionManger::GetInstance(), stage_.get()) };
    UtilL::NTimer nTimer_{3600};	//60f*秒数　3600で60秒
    UtilL::NTimer countdownTimer_{180};
    UtilL::NTimer ReadyGoTimer_{kReadyGoTimer_};
    UtilL::NTimer finTimer_{60}; // ケツのカウントダウン時の文字用
    UtilL::NTimer finTimer2_{60}; // ケツのカウントダウン時の文字用

    Progress progress_;
    int32_t frameCount_preGame_; // ゲーム前イベント用のフレームカウンタ。

    bool isMenu_;
    int32_t destination_;

    //テスト用
    DrawIntNumImage drawNum_;
};

