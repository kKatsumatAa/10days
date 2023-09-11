#pragma once

#include "Vec2.h"
#include "TextureManager.h"
#include <cstdint>
#include"Object.h"

class SceneTransition
{
public:
    // �萔
    static const int32_t kMaxFrameRolled_{ 50 }; // ���t���[���|���Ĉړ����Ă��邩
    static const int32_t kMaxFrameRolledTrans_{ 10 }; // ���t���[���|���ē����ɂȂ邩
    static const int32_t kMaxFrameTBBelt_{ 10 }; // ���t���[���|���邩
    static const int32_t kTotalFrame_{kMaxFrameRolled_ + kMaxFrameRolledTrans_ + kMaxFrameTBBelt_}; // ��L�̍��v�l

    // �֐�
    SceneTransition(void) { Reset(); }
    ~SceneTransition(void) = default;

    static void StaticInit();
    void Reset(void);
    void Start(void);
    void End(void);
    void Update(void);
    void Draw(void);

private:
    // �ϐ�
    bool isExe_;
    //�J�n�̉��o���I�������
    bool isBeginEnd_ = false;
    int32_t alpha_; // �摜�̃A���t�@�l

    Vec2 pos_rolledScroll_; // �����̊�����Ă镔���̊G�̍��W
    Vec2 pos_unrollScroll_; // �����̂܂�������ĂȂ������̊G�̍��W
    Vec2 pos_TBBeltBG_; // �������J�ڂ�����̏�̑ѕ����̍��W

    int32_t frameCount_rolled_; // �����ׂ̃J�E���^
    int32_t frameCount_transBG_; // �^�񒆂𓧉߂���J�E���^
    int32_t frameCount_moveTBBG_; // �㉺�̑т��J����ׂ̃J�E���^

    // resorce
    static uint64_t png_rolledScroll_;
    static uint64_t png_unrollScroll_;
    static uint64_t png_topBeltBG_ ;
    static uint64_t png_bottomBeltBG_;

    Object makimono_;
    Object wasiBack2_;
    Object wasiFrameUp2_;
    Object wasiFrameBottom2_;

public:
    bool GetIsBeginEnd() { return isBeginEnd_; }
    bool GetIsExe() { return isExe_; }
};
