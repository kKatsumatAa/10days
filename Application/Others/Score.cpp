#include "Score.h"
#include "ImGuiManager.h"
#include "CameraManager.h"

#include <fstream>
#include <string>
#include <sstream>
#include <assert.h>

Score* Score::GetInstance()
{
	static Score instance;
	return &instance;
}

void Score::Init()
{
	nowScore_ = 0;
	drawNum_.Initialize(TextureManager::LoadGraph("number.png"));
	nScoreSize_ = 0.6f;
	nScorePos_ = { 1250.f,20.f };
	drawNum_.SetNum(
		nowScore_,
		{ nScorePos_.x - nScoreSize_ * 100.f,nScorePos_.y },
		{ 1.0f / 10.0f,1.0f }, { 100,160 }, nScoreSize_);

	drawNumHigh_.Initialize(TextureManager::LoadGraph("number.png"));
	hScoreSize_ = 0.6f;
	hScorePos_ = { 400.f ,200.f };
	drawNumHigh_.SetNum(
		highScore_,
		{ hScorePos_.x - hScoreSize_ * 100.f,hScorePos_.y },
		{ 1.0f / 10.0f,1.0f }, { 100,160 }, hScoreSize_);
}

void Score::Add(uint32_t enemyNum)
{
	uint32_t score;
	score = (uint32_t)((float)(enemyNum * 500) * (0.9f + 0.1f * (float)enemyNum));
	nowScore_ += score;

	//桁数求める
	float digit = 0.f;	//桁数
	uint32_t result = nowScore_;	//値が変更されないように格納

	while (result > 0)
	{
		result /= 10;
		digit++;
	}

	//scale(第5引数) * 画像の大きさ = 桁ごとにずらす値(digitに掛けてる値)
	drawNum_.SetNum(
		nowScore_,
		{ nScorePos_.x - digit * nScoreSize_ * 100.f,nScorePos_.y },
		{ 1.0f / 10.0f,1.0f }, { 100,160 }, nScoreSize_);
}

void Score::Draw()
{
	drawNum_.Draw(CameraManager::GetInstance().GetCamera2D("UICamera"));
}

void Score::DrawHighScore()
{
	drawNumHigh_.Draw(CameraManager::GetInstance().GetCamera2D("UICamera"));
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

		//桁数求める
		float digit = 0.f;	//桁数
		uint32_t result = highScore_;	//値が変更されないように格納

		while (result > 0)
		{
			result /= 10;
			digit++;
		}

		//scale(第5引数) * 画像の大きさ = 桁ごとにずらす値(digitに掛けてる値)
		drawNumHigh_.SetNum(
			highScore_,
			{ hScorePos_.x - digit * hScoreSize_ * 100.f,hScorePos_.y },
			{ 1.0f / 10.0f,1.0f }, { 100,160 }, hScoreSize_);
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
