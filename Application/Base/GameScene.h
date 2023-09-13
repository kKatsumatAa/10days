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
    // �萔
    float kMaxGameTimer_{ 60.f };
    const float kReadyGoTimer_{ 500 };

    // ��`
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

    // �֐�
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
    uint32_t sceneChange_SE_ = 0;   //�V�[���J�ډ�

    // �ϐ�
    std::unique_ptr<Stage> stage_{ std::make_unique<Stage>() };

    std::unique_ptr<Player> player_{ std::make_unique<Player>(CollisionManger::GetInstance(), stage_.get()) };
    UtilL::NTimer nTimer_{3600};	//60f*�b���@3600��60�b
    UtilL::NTimer countdownTimer_{180};
    UtilL::NTimer ReadyGoTimer_{kReadyGoTimer_};
    UtilL::NTimer finTimer_{60}; // �P�c�̃J�E���g�_�E�����̕����p
    UtilL::NTimer finTimer2_{60}; // �P�c�̃J�E���g�_�E�����̕����p

    Progress progress_;
    int32_t frameCount_preGame_; // �Q�[���O�C�x���g�p�̃t���[���J�E���^�B

    bool isMenu_;
    int32_t destination_;

    //�e�X�g�p
    DrawIntNumImage drawNum_;
};

