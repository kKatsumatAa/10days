#pragma once
#include <memory>
#include "SceneState.h"
#include "Player.h"
#include "Enemy.h"
#include "UtilL.h"
#include "Stage.h"
#include "CollisionManger.h"

class GameScene :
    public SceneState
{
public:
    // ’è”
    float kMaxGameTimer_{ 60.f };

    // ŠÖ”
    void Finalize()override;
    void Initialize()override;
    void Update()override;
    void Draw()override;
    void DrawSprite()override;

    void DrawImgui()override;

private:
    uint32_t game_BGM_ = 0;
    uint32_t sceneChange_SE_ = 0;   //ƒV[ƒ“‘JˆÚ‰¹

    // •Ï”
    std::unique_ptr<Stage> stage_{ std::make_unique<Stage>() };

    std::unique_ptr<Player> player_{ std::make_unique<Player>(CollisionManger::GetInstance(), stage_.get()) };
    UtilL::Timer timer_{};
};

