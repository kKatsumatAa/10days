#pragma once

#include "Vec2.h"
#include "TextureManager.h"
#include <cstdint>
#include"Object.h"

class SceneTransition
{
public:
    // 定数
    static const int32_t kMaxFrameRolled_{ 50 }; // 何フレーム掛けて移動してくるか
    static const int32_t kMaxFrameRolledTrans_{ 10 }; // 何フレーム掛けて透明になるか
    static const int32_t kMaxFrameTBBelt_{ 10 }; // 何フレーム掛けるか
    static const int32_t kTotalFrame_{kMaxFrameRolled_ + kMaxFrameRolledTrans_ + kMaxFrameTBBelt_}; // 上記の合計値

    // 関数
    SceneTransition(void) { Reset(); }
    ~SceneTransition(void) = default;

    static void StaticInit();
    void Reset(void);
    void Start(void);
    void End(void);
    void Update(void);
    void Draw(void);

private:
    // 変数
    bool isExe_;
    //開始の演出が終わったか
    bool isBeginEnd_ = false;
    int32_t alpha_; // 画像のアルファ値

    Vec2 pos_rolledScroll_; // 巻物の巻かれてる部分の絵の座標
    Vec2 pos_unrollScroll_; // 巻物のまだ巻かれてない部分の絵の座標
    Vec2 pos_TBBeltBG_; // 巻物が遷移した先の上の帯部分の座標

    int32_t frameCount_rolled_; // 巻く為のカウンタ
    int32_t frameCount_transBG_; // 真ん中を透過するカウンタ
    int32_t frameCount_moveTBBG_; // 上下の帯が捌ける為のカウンタ

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
