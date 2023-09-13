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
	Densetu
};

class Score
{
public:
	static Score* GetInstance();

	//初期化
	void Init();
	//スコア追加
	void Add(uint32_t enemyNum);
	//スコア描画
	void Draw();
	void DrawHighScore();
	void DrawImGui();

	//ハイスコアの更新(したか確認)
	void HighScoreUpdate();

	//ハイスコア保存
	void SaveScore();
	//ハイスコア読み込み
	void LoadScore();

	void SetNowPos(const Vec2& pos) { nScorePos_ = pos; }
	void SetHighPos(const Vec2& pos) { hScorePos_ = pos; }
	void SetNowSize(float size) { nScoreSize_ = size; }
	void SetHighSize(float size) { hScoreSize_ = size; }

	void SetNowNum();	//座標とかサイズセットしたらこれ呼んで反映させる
	void SetHighNum();	//座標とかサイズセットしたらこれ呼んで反映させる

	void SetRank();

private:
	uint32_t nowScore_;		//現在のスコア
	uint32_t highScore_;	//今までのハイスコア

	DrawIntNumImage drawNum_;		//数字描画用
	DrawIntNumImage drawNumHigh_;	//数字描画用

	Vec2 nScorePos_;		//現在のスコアの描画座標
	Vec2 hScorePos_;		//今までのハイスコアの描画座標

	float nScoreSize_;		//現在のスコアの描画サイズ
	float hScoreSize_;		//今までのハイスコアの描画サイズ

	Rank rank_;
};