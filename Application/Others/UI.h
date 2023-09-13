#pragma once
#include <memory>
#include <vector>
#include <Vec2.h>
#include "Object.h"

enum class UIType
{
	Abutton,	//A�{�^��
	Bbutton,	//B�{�^��
	Rbutton,	//R�{�^��
	Menubutton,	//���j���[�{�^��
	Lstick,		//L�X�e�B�b�N
	Move,		//�ړ�
	Attack,		//�Ȃ�����
	Skewer,		//���h��
	Pause,		//�|�[�Y
	Retry,		//���g���C
	ToTitle,	//�^�C�g����
	Time,		//�c�莞��
	NowScore,	//����̋L�^
	HighScore,	//�ō��L�^
	Makimono,	//����
    ReadyGo,    // �n�߁I
    Finish,     // �����܂�!
    NewRecord,  //�V�L�^!

    Tutorial1,
    Tutorial2,
    Tutorial3,
    Tutorial4,
    Tutorial5,
    Tutorial6,
    Tutorial7,

    Number0,
    Number1,
    Number2,
    Number3,
    Number4,
    Number5,
    Number6,
    Number7,
    Number8,
    Number9,

	Max			//�ő吔(for���Ƃ��񂷗p)
};

class UI
{
private:
	std::vector<Object> objUI_{((uint32_t)UIType::Max)};		//UI�`��p�I�u�W�F�N�g�Q
	std::vector<uint64_t> ui_{};		//UI�e�N�X�`���n���h���Q
	std::vector<Vec2> ancorPoint_{};	//UI�`���_�Q
	std::vector<Vec4> color_{};			//UI�`��F�Q

public:
	UI();
	static UI* GetInstance();
	//�S�ēǂݍ���
	void AllLoad();
	void Init();
	void Update();

	//�w�肳�ꂽUI��`��
	void Draw(UIType uiType);
	void Draw2(UIType uiType, float rot);

	//�w�肳�ꂽUI�̕`����W�ݒ�
	void SetPos(UIType uiType,const Vec2& pos);
	//�w�肳�ꂽUI�̕`��T�C�Y�ݒ�
	void SetSize(UIType uiType, float size);
	//�w�肳�ꂽUI�̕`���_�ݒ�
	void SetAncorPoint(UIType uiType, const Vec2& ancorPoint);
	//�w�肳�ꂽUI�̕`��F�ݒ�
	void SetColor(UIType uiType, const Vec4& color);
};