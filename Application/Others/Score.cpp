#include "Score.h"
#include "ImGuiManager.h"

#include <fstream>
#include <string>
#include <sstream>
#include <assert.h>

uint32_t Score::nowScore_ = 0;
uint32_t Score::highScore_ = 0;
DrawIntNumImage Score::drawNum_;

void Score::Init()
{
	nowScore_ = 0;
	drawNum_.Initialize(TextureManager::LoadGraph("number.png"));
	drawNum_.SetNum(nowScore_, { 0,0 }, { 1.0f / 10.0f,1.0f }, { 100,160 }, 1.0f);
	//ハイスコアの読み込み//
}

void Score::Add(uint32_t enemyNum)
{
	uint32_t score;
	score = (uint32_t)((float)(enemyNum * 500) * (0.9f + 0.1f * (float)enemyNum));
	nowScore_ += score;
	drawNum_.SetNum(nowScore_, { 0,0 }, { 1.0f / 10.0f,1.0f }, { 100,160 }, 1.0f);
}

void Score::Draw()
{
	drawNum_.Draw();
}

void Score::DrawImGui()
{
	ImGui::Begin("Score");

	ImGui::Text("NowScore: %d", (int)nowScore_);
	ImGui::Text("HighScore: %d", (int)highScore_);

	ImGui::End();
}

void Score::HighScoreUpdate()
{
	//ハイスコアの更新あったら塗り替える
	if (highScore_ < nowScore_)
	{
		highScore_ = nowScore_;
	}
}

void Score::SaveScore()
{
	//ファイル出力処理
	std::ofstream writing_file;

	std::string filename = "";
	filename = "./Resources/data/highScore.txt";

	writing_file.open(filename, std::ios::out);

	uint32_t h = highScore_;
	writing_file << h << std::endl;

	writing_file.close();
}

void Score::LoadScore()
{
	//ファイル入力処理
	std::ifstream reading_file;

	reading_file.open("./Resources/data/highScore.txt");
	//ファイルオープン失敗をチェック
	if (reading_file.fail())
	{
		assert(0);
	}

	std::string line;

	while (getline(reading_file, line))
	{
		//文字列をintにして代入
		highScore_ = (uint32_t)atoi(line.c_str());
	}
}
