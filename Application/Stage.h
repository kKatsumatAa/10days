#pragma once
#include "Vec2.h"
#include "Object.h"
#include <memory>

class Stage final
{
public:
    // �֐�
    Stage(void) = default;
    ~Stage(void) = default;

    void Initialize(const Vec2& lt,const Vec2& rb);
    void Update(void);
    void Draw(void);

private:
    // �ϐ�
    Vec2 leftTop_{};
    Vec2 RightBottom_{};

    uint64_t png_Stage_ = 0;        //�X�e�[�W�摜
    uint64_t png_Background_ = 0;   //�w�i�摜

    Object objStage_;
    Object objBack_;

public:
    // getter
    const Vec2& GetLT(void) { return leftTop_; }
    const Vec2& GetRB(void) { return RightBottom_; }

    // setter
    void SetLT(const Vec2& lt) { leftTop_ = lt; }
    void SetRB(const Vec2& rb) { RightBottom_ = rb; }
};

