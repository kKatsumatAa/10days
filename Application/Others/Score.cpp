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

	//�������߂�
	float digit = 0.f;	//����
	uint32_t result = nowScore_;	//�l���ύX����Ȃ��悤�Ɋi�[

	while (result > 0)
	{
		result /= 10;
		digit++;
	}

	//scale(��5����) * �摜�̑傫�� = �����Ƃɂ��炷�l(digit�Ɋ|���Ă�l)
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
	//�n�C�X�R�A�̍X�V��������h��ւ���
	if (highScore_ < nowScore_)
	{
		highScore_ = nowScore_;

		//�������߂�
		float digit = 0.f;	//����
		uint32_t result = highScore_;	//�l���ύX����Ȃ��悤�Ɋi�[

		while (result > 0)
		{
			result /= 10;
			digit++;
		}

		//scale(��5����) * �摜�̑傫�� = �����Ƃɂ��炷�l(digit�Ɋ|���Ă�l)
		drawNumHigh_.SetNum(
			highScore_,
			{ hScorePos_.x - digit * hScoreSize_ * 100.f,hScorePos_.y },
			{ 1.0f / 10.0f,1.0f }, { 100,160 }, hScoreSize_);
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
