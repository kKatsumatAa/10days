#pragma once
#include <memory>
#include "DrawIntNumImage.h"

class Score
{
public:
	static Score* GetInstance();

	//������
	void Init();
	//�X�R�A�ǉ�
	void Add(uint32_t enemyNum);
	//�X�R�A�`��
	void Draw();
	void DrawHighScore();
	void DrawImGui();

	//�n�C�X�R�A�̍X�V(�������m�F)
	void HighScoreUpdate();

	//�n�C�X�R�A�ۑ�
	void SaveScore();
	//�n�C�X�R�A�ǂݍ���
	void LoadScore();

	uint32_t nowScore_;		//���݂̃X�R�A
	uint32_t highScore_;	//���܂ł̃n�C�X�R�A

	DrawIntNumImage drawNum_;		//�����`��p
	DrawIntNumImage drawNumHigh_;	//�����`��p

	float nScoreSize_;		//���݂̃X�R�A�̕`��T�C�Y
	float hScoreSize_;		//���܂ł̃n�C�X�R�A�̕`��T�C�Y
};