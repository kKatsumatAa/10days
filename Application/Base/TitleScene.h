#pragma once
#include "SceneState.h"
#include <memory>
#include "Object.h"

class TitleScene :
    public SceneState
{
private:
    //�^�C�g���摜�n���h��
    uint64_t titleImageHandle_ = 0;
    uint32_t title_BGM_ = 0;
    uint32_t sceneChange_SE_ = 0;  //�V�[���J�ډ�
    uint32_t decision_SE_ = 0;  //���艹
    Object titleObj_;

public:
    // �֐�
    void Finalize()override;
    void Initialize()override;
    void Update()override;
    void Draw()override;
    void DrawSprite()override;
    void DrawSprite2()override;
    void DrawImgui()override;
};

