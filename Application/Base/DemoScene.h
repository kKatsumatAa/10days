#pragma once
#include <memory>
#include "SceneState.h"
#include "Player.h"
#include "Enemy.h"
#include "Util.h"

class DemoScene final : public SceneState
{
public:
    // ä÷êî
    void Finalize()override;
    void Initialize()override;
    void Update()override;
    void Draw()override;
    void DrawSprite()override;
    void DrawSprite2()override;
    void DrawImgui()override;

private:
    // ïœêî
    //std::unique_ptr<Player> player_{ std::make_unique<Player>() };
    //std::unique_ptr<Enemy> enemy_{ std::make_unique<Enemy>(player_.get()) };
};

