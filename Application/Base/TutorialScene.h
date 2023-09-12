#pragma once
#include <memory>
#include "SceneState.h"
#include "Player.h"
#include "Enemy.h"
#include "UtilL.h"
#include "Stage.h"
#include "CollisionManger.h"
#include "DrawIntNumImage.h"

class TutorialScene :
    public SceneState
{
public:
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

    // �萔
    const int32_t kMaxFrameSkipTutorial_{ 100 }; // pad-B���t���[���������Ń`���[�g���A�����X�L�b�v���邩
    const int32_t kMaxFrame4StartTrans_{ 50 }; // ���������͂��߂�܂ŉ��t���[�������邩
    const int32_t kMaxFrameTransColor_{ 90 }; // �����ɂȂ�܂ŉ��t���[�������邩
    const int32_t kIntervalNextSign_{ 50 }; // ���������Ă��玟�̊Ŕ\���܂ŉ��t���[���҂�

    // �֐�
    void Initialize(void) override;
    void Update(void) override;
    void TutorialSceneUpdate(void);
    void Draw(void) override {};
    void DrawSprite(void) override;
    void DrawSprite2(void) override;
    void DrawImgui(void) override {};
    void Finalize(void) override {};

    // �ϐ�
    bool isMenu_; // ���j���[�p
    int32_t destination_{}; // ���j���[���̎��V�[���̑J�ڐ�
    int32_t frameCount_skipTutorial_{}; // �`���[�g���A�����X�L�b�v����p�̃t���[���J�E���^

    uint32_t progressNum_{ 1 }; // �`���[�g���A���i�s�x
    int32_t frameCount_sign_{}; // �Ŕ𓧖��ɂ���Ƃ��̃J�E���^
    int32_t alpha_sign_{ 255 }; // �Ŕ̓����x

    std::unique_ptr<Stage> stage_{ std::make_unique<Stage>() };

    std::unique_ptr<Player> player_{ std::make_unique<Player>(CollisionManger::GetInstance(), stage_.get()) };
    UtilL::Timer timer_{};
};

