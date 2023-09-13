#pragma once
#include <memory>
#include "DrawIntNumImage.h"

enum class Rank
{
	Kakedasi,
	Hanninmae,
	Itininmae,
	Itiryu,
	Jukuren,
	Densetu,

	Max
};

class Score
{
public:
	static Score* GetInstance();

	void LoadTex();

	//初期化
	void Init();
	//スコア追加
	void Add(uint32_t enemyNum);
	//スコア描画
	void Draw();
	void DrawHighScore();
	void DrawRank(float rot);
	void DrawImGui();

	//ハイスコアの更新(したか確認)
	void HighScoreUpdate();

	//ハイスコア保存
	void SaveScore();
	//ハイスコア読み込み
	void LoadScore();

	//加算されるスコアを取得
	const uint32_t GetPlusScore() { return plusScore_; }

	void SetNowPos(const Vec2& pos) { nScorePos_ = pos; }
	void SetHighPos(const Vec2& pos) { hScorePos_ = pos; }
	void SetNowSize(float size) { nScoreSize_ = size; }
	void SetHighSize(float size) { hScoreSize_ = size; }

	void SetNowNum();	//座標とかサイズセットしたらこれ呼んで反映させる
	void SetHighNum();	//座標とかサイズセットしたらこれ呼んで反映させる

	void SetRank();

	//指定されたUIの描画座標設定
	void SetRankPos(const Vec2& pos);
	//指定されたUIの描画サイズ設定
	void SetRankSize(float size);

private:
	uint32_t plusScore_;	//加算されるスコア
	uint32_t nowScore_;		//現在のスコア
	uint32_t highScore_;	//今までのハイスコア

	DrawIntNumImage drawNum_;		//数字描画用
	DrawIntNumImage drawNumHigh_;	//数字描画用

	Vec2 nScorePos_;		//現在のスコアの描画座標
	Vec2 hScorePos_;		//今までのハイスコアの描画座標

	float nScoreSize_;		//現在のスコアの描画サイズ
	float hScoreSize_;		//今までのハイスコアの描画サイズ

	Rank rank_;
	Object obj_;
	std::vector<uint64_t> rankTex_;
};