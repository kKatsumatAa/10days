#pragma once
#include "Vec2.h"
#include "Object.h"
#include <Easing.h>

#include <memory>
#include <vector>

class Emitter final
{
	//���q1��
	struct Particle
	{
		std::unique_ptr<Object> obj_ = nullptr;
		//���W
		Vec2 pos;
		//�傫��
		Vec2 scale;
		Vec2 startScale;	//�J�n���̑傫��
		Vec2 endScale;		//�I�����̑傫��
		//�p�x
		float rot = 0;
		//���x
		Vec2 velo;
		//�����x
		Vec2 accel;
		//�d��
		float gravity = 0.98f;

		//�F
		uint32_t color;
		//��������(�t���[����)
		Easing::EaseTimer aliveTimer;

		//�C�[�W���O�p�^�C�}�[
		Easing::EaseTimer easeTimer = 1.0f;

		//����
		float radius = 0;
		float startRadius = 0;	//�J�n���̋���

		//�摜
		uint32_t key;

	public:
		Particle() { ; }
		void Draw();
	};

private:
	//���W
	Vec2 pos_;
	//�p�x
	float rot_ = 0.0f;
	//�傫��
	Vec2 scale_{ 1.f,1.f };
	float minScale_;	//�G�~�b�^�[�̍ŏ��T�C�Y(�䗦)
	float maxScale_;	//�G�~�b�^�[�̍ő�T�C�Y(�䗦)
	//���̑傫��
	Vec2 originalScale_;
	//�g�k�p�{��
	float scaling_;
	//�g�k�p�^�C�}�[
	Easing::EaseTimer scalingTimer_ = 1.0f;
	//�p�[�e�B�N���o���^�C�~���O�p�^�C�}�[
	uint32_t particleTimer_ = 0;
	//���t���[���Ɉ��p�[�e�B�N���ǉ����邩
	uint32_t addInterval_;

	//�摜
	uint64_t key_;

	static const uint32_t maxCount_ = 256;	//�ő吔
	std::vector<Particle> particles_;	//�p�[�e�B�N���z��

	bool isActive_ = false;

public:
	Emitter();
	//������
	void Init();
	//�X�V
	void Update(bool isGravity = false);
	//�`��
	void DrawCircleParticle();
	void DrawGraph();

	//�p�[�e�B�N���ǉ�
	//life�͕b���w��
	void Add(uint32_t addNum, float life, float minScale, float maxScale, Vec2 minVelo, Vec2 maxVelo,
		Vec2 accel = { 0,0 }, float minRot = 0.0f, float maxRot = 0.0f, uint32_t color = 0xffffff);
	//�p�[�e�B�N���S����
	void ClearParticles() { particles_.clear(); }

	//�Q�b�^�[//
	//���W�擾
	Vec2 GetPos()const { return pos_; }
	//�傫���擾
	Vec2 GetScale()const { return scale_; }

	//�p�[�e�B�N���S�����񂾂��擾
	bool GetParticlesDead()const { return particles_.empty(); }

	//���p�[�e�B�N�����邩�擾
	size_t GetParticlesSize()const { return particles_.size(); }

	//�Z�b�^�[//
	//���W�ݒ�
	void SetPos(float x, float y) { pos_ = { x,y }; }
	void SetPos(Vec2& pos) { pos_ = pos; }
	//�傫���ݒ�
	void SetScale(Vec2& scale);
	//�p�x�ݒ�
	void SetRot(float rot) { rot_ = rot; }

	//�L���t���O�ݒ�
	void SetIsActive(bool isActive) { isActive_ = isActive; }
	//�e�N�X�`���̐ݒ�
	void SetTextureKey(uint32_t& key) { key_ = key; }

	//�g�k�p�^�C�}�[���؂�ւ�鎞�Ԑݒ�(�b)
	void SetScalingTimer(float timer);
	//�g�k�p�^�C�}�[�J�n
	//isRun:true = �v���X����Ă�,false:�}�C�i�X����Ă�
	void StartScalingTimer(bool isRun = true);
};