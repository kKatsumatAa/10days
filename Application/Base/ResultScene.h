#pragma once
#include "SceneState.h"
#include "Object.h"
#include <memory>

class ResultScene :
    public SceneState
{
public:
    enum Destination
    {
        RETRY,
        TITLE,
    };

    // �ϐ�
    uint64_t png_result_ = 0;       //���U���g�摜
    uint32_t result_BGM_ = 0;
    uint32_t sceneChange_SE_ = 0;   //�V�[���J�ډ�
    int32_t destination_;

    Object result_;

    // �֐�
	void Finalize()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void DrawSprite()override;
    void DrawSprite2()override;
	void DrawImgui()override;
};

