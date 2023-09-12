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

    // 変数
    uint64_t png_result_ = 0;       //リザルト画像
    uint32_t result_BGM_ = 0;
    uint32_t sceneChange_SE_ = 0;   //シーン遷移音
    int32_t destination_;

    Object result_;

    // 関数
	void Finalize()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void DrawSprite()override;
    void DrawSprite2()override;
	void DrawImgui()override;
};

