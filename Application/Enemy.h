#pragma once
#include "Player.h"
#include "IEntity.h"


class Enemy final : public IEntity
{
public:
	// 定数
	static const float kMoveSpeed_; // 速度
	static const int32_t kMoveTimes_{ 5 }; // 何回に分けて移動するか(瞬間移動に見えるのを避けるため）
	static const int32_t kMoveInterval_{ 100 }; // 敵が動くまでの間隔(フレーム単位)
	//static const float kMowRatio_; // 吹っ飛ばされたとき距離に対して、どのくらいの割合で移動していくか
	static const float kPushBackDist_; // 押し戻す距離
	static const float kPngScale_; // 画像の拡大率
	static const uint32_t kMowFrame_ = 20; // 何フレームかけて吹き飛ばされるか
	static const float KRadius_; // 半径

	// 関数
	Enemy(CollisionManger* colMPtr, Player* playerPtr, Stage* stagePtr, uint64_t texHandle);
	~Enemy(void) override;

	void Update(void) override;
	//薙ぎ払い関係のアップデート
	void MowDownFlagUpdate();

	void Draw(void) override;

private:
    // なんか分かんないけど、押し戻し処理通ってるけど、敵同士が押し戻しされません。
	void OnCollision(void);

	// 変数
	Player* playerPtr_;
	CollisionManger* colMPtr_;

	int32_t frameCount_wait_; // 待機用フレームカウント
	int32_t frameCount_move_; // 移動用フレームカウント

	bool isAlive_{ true }; // 生きてるか

	// 薙ぎ払われたか
	bool isMowDown_ = false;
	bool isMowDownTrigger_ = false;
	//デカ団子
	bool isBigDango_ = false;
	Vec2 vec_mow_{}; // 吹っ飛ばされる方向
	// 吹き飛ばされる距離だけちゃんと移動したか計測する変数
	//float tortalMowDist{};
	// 吹き飛ばされる時のイージング用タイマー
	int32_t frameCount_mow_;
	// 薙ぎ払われた最中に合体OKフラグ
	bool isDocking_{};
	// 自分、今串刺しされてますフラグ
	bool isSkewer_;

	Vec2 scale_ = { 1.0f,1.0f };

	// resource
	uint64_t png_enemy_;

public:
	// setter
	void SetIsMowDown(bool isMowDown) { isMowDown_ = isMowDown; }
	void SetIsMowDownTrigger(bool isMowDownTrigger) { isMowDownTrigger_ = isMowDownTrigger; }
	void SetIsDocking(bool isDocking) { isDocking_ = isDocking; }
	void SetIsSkewer(bool isSkewer) { isSkewer_ = isSkewer; }
	void SetIsBigDango(bool isBigDango) { isBigDango_ = isBigDango; }
	void SetScale(const Vec2& scale) { scale_ = scale; }

	// getter
	bool GetIsMowDown() { return isMowDown_; }
	bool GetIsMowDownTrigger() { return isMowDownTrigger_; }
	bool GetIsAlive(void) { return isAlive_; }
	bool GetIsDocking() { return isDocking_; }
	const Vec2& GetMowDownVec() { return vec_mow_; }
	bool GetIsSkewer(void) { return isSkewer_; }
	bool GetIsBigDango(void) { return isBigDango_; }
	const Vec2& GetScale() { return scale_; }

public:
	//合体したときに外部で呼び出す
	void Docking();
	//死亡時に呼び出す
	void Dead();
};

