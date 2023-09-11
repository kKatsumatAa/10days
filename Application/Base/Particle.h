#pragma once
#include "Vec2.h"
#include "Object.h"
#include <Easing.h>

#include <memory>
#include <vector>

class Emitter final
{
	//粒子1粒
	struct Particle
	{
		std::unique_ptr<Object> obj_ = nullptr;
		//座標
		Vec2 pos;
		//大きさ
		Vec2 scale;
		Vec2 startScale;	//開始時の大きさ
		Vec2 endScale;		//終了時の大きさ
		//角度
		float rot = 0;
		//速度
		Vec2 velo;
		//加速度
		Vec2 accel;
		//重力
		float gravity = 0.98f;

		//色
		uint32_t color;
		//生存時間(フレーム数)
		Easing::EaseTimer aliveTimer;

		//イージング用タイマー
		Easing::EaseTimer easeTimer = 1.0f;

		//距離
		float radius = 0;
		float startRadius = 0;	//開始時の距離

		//画像
		uint32_t key;

	public:
		Particle() { ; }
		void Draw();
	};

private:
	//座標
	Vec2 pos_;
	//角度
	float rot_ = 0.0f;
	//大きさ
	Vec2 scale_{ 1.f,1.f };
	float minScale_;	//エミッターの最小サイズ(比率)
	float maxScale_;	//エミッターの最大サイズ(比率)
	//元の大きさ
	Vec2 originalScale_;
	//拡縮用倍率
	float scaling_;
	//拡縮用タイマー
	Easing::EaseTimer scalingTimer_ = 1.0f;
	//パーティクル出すタイミング用タイマー
	uint32_t particleTimer_ = 0;
	//何フレームに一回パーティクル追加するか
	uint32_t addInterval_;

	//画像
	uint64_t key_;

	static const uint32_t maxCount_ = 256;	//最大数
	std::vector<Particle> particles_;	//パーティクル配列

	bool isActive_ = false;

public:
	Emitter();
	//初期化
	void Init();
	//更新
	void Update(bool isGravity = false);
	//描画
	void DrawCircleParticle();
	void DrawGraph();

	//パーティクル追加
	//lifeは秒数指定
	void Add(uint32_t addNum, float life, float minScale, float maxScale, Vec2 minVelo, Vec2 maxVelo,
		Vec2 accel = { 0,0 }, float minRot = 0.0f, float maxRot = 0.0f, uint32_t color = 0xffffff);
	//パーティクル全消し
	void ClearParticles() { particles_.clear(); }

	//ゲッター//
	//座標取得
	Vec2 GetPos()const { return pos_; }
	//大きさ取得
	Vec2 GetScale()const { return scale_; }

	//パーティクル全部死んだか取得
	bool GetParticlesDead()const { return particles_.empty(); }

	//何個パーティクルあるか取得
	size_t GetParticlesSize()const { return particles_.size(); }

	//セッター//
	//座標設定
	void SetPos(float x, float y) { pos_ = { x,y }; }
	void SetPos(Vec2& pos) { pos_ = pos; }
	//大きさ設定
	void SetScale(Vec2& scale);
	//角度設定
	void SetRot(float rot) { rot_ = rot; }

	//有効フラグ設定
	void SetIsActive(bool isActive) { isActive_ = isActive; }
	//テクスチャの設定
	void SetTextureKey(uint32_t& key) { key_ = key; }

	//拡縮用タイマーが切り替わる時間設定(秒)
	void SetScalingTimer(float timer);
	//拡縮用タイマー開始
	//isRun:true = プラスされてく,false:マイナスされてく
	void StartScalingTimer(bool isRun = true);
};