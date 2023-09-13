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

	//������
	void Init();
	//�X�R�A�ǉ�
	void Add(uint32_t enemyNum);
	//�X�R�A�`��
	void Draw();
	void DrawHighScore();
	void DrawRank(float rot);
	void DrawImGui();

	//�n�C�X�R�A�̍X�V(�������m�F)
	void HighScoreUpdate();

	//�n�C�X�R�A�ۑ�
	void SaveScore();
	//�n�C�X�R�A�ǂݍ���
	void LoadScore();

	//���Z�����X�R�A���擾
	const uint32_t GetPlusScore() { return plusScore_; }

	void SetNowPos(const Vec2& pos) { nScorePos_ = pos; }
	void SetHighPos(const Vec2& pos) { hScorePos_ = pos; }
	void SetNowSize(float size) { nScoreSize_ = size; }
	void SetHighSize(float size) { hScoreSize_ = size; }

	void SetNowNum();	//���W�Ƃ��T�C�Y�Z�b�g�����炱��Ă�Ŕ��f������
	void SetHighNum();	//���W�Ƃ��T�C�Y�Z�b�g�����炱��Ă�Ŕ��f������

	void SetRank();

	//�w�肳�ꂽUI�̕`����W�ݒ�
	void SetRankPos(const Vec2& pos);
	//�w�肳�ꂽUI�̕`��T�C�Y�ݒ�
	void SetRankSize(float size);

private:
	uint32_t plusScore_;	//���Z�����X�R�A
	uint32_t nowScore_;		//���݂̃X�R�A
	uint32_t highScore_;	//���܂ł̃n�C�X�R�A

	DrawIntNumImage drawNum_;		//�����`��p
	DrawIntNumImage drawNumHigh_;	//�����`��p

	Vec2 nScorePos_;		//���݂̃X�R�A�̕`����W
	Vec2 hScorePos_;		//���܂ł̃n�C�X�R�A�̕`����W

	float nScoreSize_;		//���݂̃X�R�A�̕`��T�C�Y
	float hScoreSize_;		//���܂ł̃n�C�X�R�A�̕`��T�C�Y

	Rank rank_;
	Object obj_;
	std::vector<uint64_t> rankTex_;
};