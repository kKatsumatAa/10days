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

	void SetNowPos(const Vec2& pos) { nScorePos_ = pos; }
	void SetHighPos(const Vec2& pos) { hScorePos_ = pos; }
	void SetNowSize(float size) { nScoreSize_ = size; }
	void SetHighSize(float size) { hScoreSize_ = size; }

	void SetNowNum();	//���W�Ƃ��T�C�Y�Z�b�g�����炱��Ă�Ŕ��f������
	void SetHighNum();	//���W�Ƃ��T�C�Y�Z�b�g�����炱��Ă�Ŕ��f������

	void SetRank();

private:
	uint32_t nowScore_;		//���݂̃X�R�A
	uint32_t highScore_;	//���܂ł̃n�C�X�R�A

	DrawIntNumImage drawNum_;		//�����`��p
	DrawIntNumImage drawNumHigh_;	//�����`��p

	Vec2 nScorePos_;		//���݂̃X�R�A�̕`����W
	Vec2 hScorePos_;		//���܂ł̃n�C�X�R�A�̕`����W

	float nScoreSize_;		//���݂̃X�R�A�̕`��T�C�Y
	float hScoreSize_;		//���܂ł̃n�C�X�R�A�̕`��T�C�Y

	Rank rank_;
};