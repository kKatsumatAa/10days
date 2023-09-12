#pragma once
#include <memory>
#include "DrawIntNumImage.h"

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

	uint32_t nowScore_;		//現在のスコア
	uint32_t highScore_;	//今までのハイスコア

	DrawIntNumImage drawNum_;		//数字描画用
	DrawIntNumImage drawNumHigh_;	//数字描画用

	float nScoreSize_;		//現在のスコアの描画サイズ
	float hScoreSize_;		//今までのハイスコアの描画サイズ
};