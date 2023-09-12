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
	//�n�C�X�R�A�̓ǂݍ���//
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
	//�n�C�X�R�A�̍X�V��������h��ւ���
	if (highScore_ < nowScore_)
	{
		highScore_ = nowScore_;
	}
}

void Score::SaveScore()
{
	//�t�@�C���o�͏���
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
	//�t�@�C�����͏���
	std::ifstream reading_file;

	reading_file.open("./Resources/data/highScore.txt");
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (reading_file.fail())
	{
		assert(0);
	}

	std::string line;

	while (getline(reading_file, line))
	{
		//�������int�ɂ��đ��
		highScore_ = (uint32_t)atoi(line.c_str());
	}
}
