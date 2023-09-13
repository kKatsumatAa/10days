#pragma once
#include "SceneState.h"
#include "Object.h"
#include <memory>
#include "UtilL.h"

class ResultScene :
    public SceneState
{
public:
    // 定義
    enum Destination
    {
        RETRY,
        TITLE,
    };

    // 定数
    const int32_t kMaxFrame_wait4Start_{ 20 };
    const int32_t kMaxFrame_currentScore_{ 80 };
    const int32_t kMaxFrame_highScore_{ 80 };
    const int32_t kMaxFrame_newRecord_{ 300 };
    const int32_t kMaxFrame_DisplayRank_{ 300 };


    // 変数
    uint64_t png_result_ = 0;       //リザルト画像
    uint32_t result_BGM_ = 0;
    uint32_t sceneChange_SE_ = 0;   //シーン遷移音
    int32_t destination_;


    UtilL::NTimer timer_wait4Start_{(float)kMaxFrame_wait4Start_};
    bool isEndAllDisplay_{}; // 全部の表示演出が終わったか
    UtilL::NTimer timer_currentScore_{(float)kMaxFrame_currentScore_};
    UtilL::NTimer timer_highScore_{(float)kMaxFrame_highScore_};
    bool isNewRecord_{};
    UtilL::NTimer timer_newRecord_{(float)kMaxFrame_newRecord_};
    UtilL::NTimer timer_DisplayRank_{(float)kMaxFrame_DisplayRank_};

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

