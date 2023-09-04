#pragma once
#include<string>
#include"Vec3.h"
#include<memory>

//�O���錾
class GameVelocityState;

//�Q�[���X�s�[�h�Ǘ��N���X
class GameVelocityManager final
{
private:
	struct SlowMotionData
	{
		//�X���[���[�V�������L����
		bool isSlowMotion = false;
		//�X���[�̏����l
		float slowVelBegin = 0;
		//�X���[�̏I���l
		float slowVelEnd = 0;
		//�X���[�̎��ԏ��
		int32_t slowTimeMax = 0;
	};

private:
	float gameVelocity_ = 0;
	//�F
	const Vec3 VEL_COL_MIN_ = { 0.5f,0.8f,0.8f };

	//�X���[���[�V����
	SlowMotionData slowData_;
	//�X���[���[�V�����X�e�[�g
	std::unique_ptr<GameVelocityState>slowState_ = nullptr;


public:
	const float GAME_VELOCITY_MAX_ = 1.0f;
	const float GAME_VELOCITY_MIN_ = 0.03f;

private:
	GameVelocityManager() { ; }
	~GameVelocityManager() { ; }

public:
	GameVelocityManager(const GameVelocityManager& other) = delete;
	GameVelocityManager& operator=(const GameVelocityManager& other) = delete;

	static GameVelocityManager& GetInstance();

public:
	void Initialize();

	void Update();

	void UpdateImGui();

public:
	//�X���[���[�V�����X�e�[�g�ύX
	void ChangeSlowState(const std::string& name);

private:
	//�X���[���[�V�����̃f�[�^���Z�b�g
	void SetSlowMotionData(int32_t lerpFrame, float velocity, bool isSlowMotion);

public:
	/// <summary>
///�X���[���[�V�����n�߂�
/// </summary>
/// <param name="lerpFrame">���X�ɃX���[�ɂȂ�t���[����</param>
/// <param name="endVelocity">�ŏI�I�ȃQ�[���X�s�[�h</param>
	void BeginSlowMotion(int32_t lerpFrame, float endVelocity);

	/// <summary>
	/// �X���[���[�V�����I���
	/// </summary>
	/// <param name="lerpFrame">���X�ɃX���[���I���t���[����</param>
	/// <param name="endVelocity">�ŏI�I�ȃQ�[���X�s�[�h</param>
	void EndSlowMotion(int32_t lerpFrame, float endVelocity);
	//�����ݒ肵�Ȃ�ver�i�X�^�[�g���̃p�����[�^�g�p����j
	void EndSlowMotion();

public:
	//�Q�[���̃X�s�[�h���擾
	float GetVelocity();
	void SetVelocity(float velocity) { gameVelocity_ = velocity; }

	//�X���[���[�V��������
	bool GetIsSlowMotion() { return slowData_.isSlowMotion; }
	void SetIsSlowMotion(bool isSlowMotion) { slowData_.isSlowMotion = isSlowMotion; }
};