#pragma once
#include "IEntity.h"
#include "Object.h"
#include "CollisionManger.h"
#include "PlayerAttack.h"
#include "PlayerSkewerAttack.h"


class Player final : public IEntity
{
public:
    // 定数
    const float kMoveSpeed_{ 3.f }; // 速度
    const int32_t kMaxInvincibleFrame_{ 120 }; // 無敵時間(フレーム)
    const float kPushBackDist_{ 2.f }; // 押し戻す距離
    const float kKnockbackDist_{ 20.f }; // 敵に触れられてノックバックする距離
    const int32_t kMaxKnockbackFrame_{ 10 }; // ノクバするとき何フレーム掛けてノクバするか

    // 薙ぎ払い
    static const float kMowDist_; // 薙ぎ払いで吹き飛ばす距離
    const float kMowSupportCenterDist_{ 35.f }; // mowsupportの中心が、プレイヤーの中心からどんくらいはなれてるか
    //const float kMowAttackCenterDist_{ 10.f }; // 薙ぎ払い攻撃の範囲の中心がプレイヤーの中心からどれだけ離れてるか
    const float kMowSwordCenterDist_{ 30.f }; // 薙ぎ払い攻撃の絵の中心がプレイヤーの中心からどのくらい離れてるか
    const float kMaxRangeSwordDegree_{ 200.f }; // 剣をどのくらいの角度まで振るか
    const float kMowArrowXRate_{ 0.1f }; // 常時表示する薙ぎ払い用の矢印の拡大率X
    const float kMowArrowYRate_{ 0.34f }; // 常時表示する薙ぎ払い用の矢印の拡大率Y
    const float kMowArrowDist2Self_{ 80.f }; // 常時表示する薙ぎ払い用の矢印とプレイヤーの距離

    // 串刺し
    const int32_t kChargeFrame4Skewer_{ 1 }; // 串刺し攻撃の為に、何フレームボタンを押し続けなければならないか
    const float kSkewerAttackCenterDist_{ 35.f }; // 串刺し攻撃の範囲の中心がプレイヤーの中心からどれだけ離れてるか ※仕様上、目標+5frameにする必要がある。
    const int32_t kSlowMotionFrameUntilStart_{ 0 }; // スローモーションが始まるまでのフレーム
    const float kPngScale_{ 0.07f }; // 画像の拡大
    const int32_t kMaxFrameSkewerEndHitStop_{ 20 }; // 串刺し終了後のヒットストップが何フレームかかり続けるか
    static bool isSkewerScreenBlack4SceneM_;
    const int32_t kMaxFrameSkewerEndShrink_{ 5 }; // 串刺し終了後、縮む時に何フレームかかるか
    const int32_t kMaxFrame_postSkewerInvincible_{ 30 }; // 串刺し終了後攻撃を食らわないための無敵フレーム


    // 定義
    enum class State
    {
        MOVE,
        ATTACK_MOW,
        ATTACK_SKEWER,
    };

    // 関数
    Player(CollisionManger* colMPtr, Stage* stagePtr);
    ~Player(void) override;

    void Initialize();
    void Update(void) override;
    void Draw(void) override;

    void ResetSkewerInfo4Pause(void);

private:
    void MoveUpdate(void);
    void MowAttackUpdate(void);
    void SkewerAttackUpdate(void);
    void OnCollision(void);

    // 変数
    CollisionManger* colMPtr_;

    int32_t frameCount_invincible_; // 無敵時間カウントのためのフレームカウンタ
    int32_t frameCount_4Skewer_; // 串刺しの溜めのフレームカウンタ
    int32_t frameCount_SkewerEndHitStop_; // 串刺し終了後のヒットストップの為のフレームカウンタ
    Vec2 vec_move_; // プレイヤーの向き
    State state_; // 現在の状態fsm

    Vec2 pos4Line_{}; // debug用プレイヤーの向き描画の溜めの座標

    bool isKnockback_{}; // ノクバされま〜す
    int32_t frameCount_knockback_;
    Vec2 vec_contactE2P_; // 触れられた敵からプレイヤーへの向き
    
    int32_t frameCount_postSkewerInvincible_;
    bool isSkewerEndShrink_;
    int32_t frameCount_SkewerEndShrink_; // 串刺し終了後の縮むフレームカウンタ
    float dist_p4su2p4sb_; // 串刺し終了時のpos4SwordUpからpos4SwordBottomまでの距離

    PlayerMowAttack mow_; // 薙ぎ払い用クラス
    PlayerSkewerAttack skewer_; // 串刺し用クラス

    Vec4 areaColor_;    //攻撃範囲とかの色


    // resources
    bool isRight_{};
    uint64_t png_player_ = TextureManager::LoadGraph("player.png");
    uint64_t png_arrow_ = TextureManager::LoadGraph("arrow.png");  //矢印画像
    uint64_t png_skewerArea_ = TextureManager::LoadGraph("skewer_area.png"); // 串刺し範囲
    uint64_t png_sword_ = TextureManager::LoadGraph("sword.png");
    uint64_t png_swordUp_ = TextureManager::LoadGraph("sword_up.png");
    uint64_t png_swordBottom_ = TextureManager::LoadGraph("sword_bottom.png");
    Vec2 pos4Sword_; // 串の絵のための座標
    Vec2 pos4SwordUp_; // 串の絵のための座標
    Vec2 pos4SwordBottom_; // 串の絵のための座標
    float rot4Sword2_; // 串の絵自体を回転させるための角度
    float rot4RotationSword_; // 串を回転移動させる為の角度

    Object arrow_;
    Object skewerArea_;
    Object sword_;
    Object swordUp_;
    Object swordBottom_;

public:
    // setter
    void SetVecMove(const Vec2& vec) { vec_move_ = vec; }

    // getter
    bool GetIsSkewerEndShrink(void) { return isSkewerEndShrink_; }
    const Vec2& GetDirectionVec() { return pos4Line_; }
    const Vec2& GetMoveVec() { return vec_move_; }
    bool GetIsSkewer(void) { return skewer_.GetIsSkewer(); }
    int32_t GetFrameCountAttack(void) { return mow_.GetFrameCountAttack(); }
    const Vec2& GetPos4SwordBottom(void) { return pos4SwordBottom_; }
};
