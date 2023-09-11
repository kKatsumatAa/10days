#pragma once
#include<memory>
#include<string>
#include<functional>


//�O���錾
class GameVelocityManager;

//�Q�[���X�s�[�h�N���X�̃X�e�[�g�e�N���X
class GameVelocityState
{
protected:
	//�o�ߎ���
	int32_t time_ = 0;
	//�X���[�̏����l
	float slowVelBegin_ = 0;
	//�X���[�̏I���l
	float slowVelEnd_ = 0;
	//�X���[�̎��ԏ��
	int32_t slowTimeMax_ = 0;
	//���̃X�e�[�g�̖��O
	std::string nextStateName_;

public:
	virtual ~GameVelocityState() { ; }

	virtual void Initialize(float slowVelBegin, float slowVelEnd, int32_t slowTimeMax);
	virtual void Update() = 0;
	void CommonUpdate(const std::function<void()>& endF);

public:
	static std::unique_ptr<GameVelocityState>GetState(const std::string& name);
};


//�������Ȃ�
class GameVelocityStateNone : public GameVelocityState
{
public:
	void Initialize(float slowVelBegin, float slowVelEnd, int32_t slowTimeMax)override;
	void Update() override;
};

//�X���[���[�V�����n��
class GameVelocityStateBegin : public GameVelocityState
{
public:
	void Initialize(float slowVelBegin, float slowVelEnd, int32_t slowTimeMax)override;
	void Update() override;
};

//�X���[���[�V������
class GameVelocityStateDoing : public GameVelocityState
{
public:
	void Initialize(float slowVelBegin, float slowVelEnd, int32_t slowTimeMax)override;
	void Update() override;
};

//����
class GameVelocityStateEnd : public GameVelocityState
{
public:
	void Initialize(float slowVelBegin, float slowVelEnd, int32_t slowTimeMax)override;
	void Update() override;
};