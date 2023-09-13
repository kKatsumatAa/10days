#pragma once
#include<memory>
#include<string>
#include"Vec2.h"
#include<functional>


//�O���錾
class CombinedEnemies;

//���������G�̃X�e�[�g�e�N���X
class ICombinedEnemiesState
{
protected:
	float timer_ = 0;
	float timerMax_ = 30;
	//
	CombinedEnemies* enemies_ = nullptr;
	//
	std::string nextStateName_;

public:
	virtual ~ICombinedEnemiesState() { ; }

public:
	virtual void Initialize();
	virtual void Update() = 0;
	void TimerUpdate(const std::function<void(float)>& f = NULL, const std::function<void()>& endF = NULL);
	void ShakeUpdate(float minS, float maxS, float rate, float t);
	float GetTimeRate();

	void SetCombinedEnemies(CombinedEnemies* enemies) { enemies_ = enemies; }

public:
	static std::unique_ptr<ICombinedEnemiesState>GetState(const std::string& name);
};


//�ガ����ꒆ
class CombinedEnemiesStateMoveMowDown : public ICombinedEnemiesState
{
private:
	Vec2 direction_ = { 0,0 };

protected:
	//�Ԃ��̃p�����[�^
	//�ŏ��X�P�[��
	float minScale_ = 0.6f;
	//�ő�X�P�[��
	float maxScale_ = 1.1f;
	//�ǂ̂��炢�̃X�s�[�h�ł�邩
	float shakeRate_ = 0.9f;

public:
	void Initialize()override;
	void Update()override;
};

//�ːi���ꒆ
class CombinedEnemiesStateSkewer : public ICombinedEnemiesState
{
private:
	Vec2 direction_ = { 0,0 };

protected:
	//�Ԃ��̃p�����[�^
	//�ŏ��X�P�[��
	float minScale_ = 0.8f;
	//�ő�X�P�[��
	float maxScale_ = 1.3f;
	//�ǂ̂��炢�̃X�s�[�h�ł�邩
	float shakeRate_ = 0.2f;

public:
	void Initialize()override;
	void Update()override;
};

//���̌�
class CombinedEnemiesStateAfterCombined : public ICombinedEnemiesState
{
protected:
	//�Ԃ��̃p�����[�^
	//�ŏ��X�P�[��
	float minScale_ = 0.5f;
	//�ő�X�P�[��
	float maxScale_ = 1.2f;
	//�ǂ̂��炢�̃X�s�[�h�ł�邩
	float shakeRate_ = 0.8f;

public:
	void Initialize()override;
	void Update()override;
};

//�ҋ@��
class CombinedEnemiesStateMoveWait : public ICombinedEnemiesState
{
protected:
	//�Ԃ��̃p�����[�^
	//�ŏ��X�P�[��
	float minScale_ = 0.7f;
	//�ő�X�P�[��
	float maxScale_ = 1.1f;
	//�ǂ̂��炢�̃X�s�[�h�ł�邩
	float shakeRate_ = 0.7f;

public:
	void Initialize()override;
	void Update()override;
};

//�k�ݒ�
class CombinedEnemiesStateShrink : public ICombinedEnemiesState
{

public:
	void Initialize()override;
	void Update()override;
};

//�k��ő҂��Ă�
class CombinedEnemiesStateWaitStretch : public ICombinedEnemiesState
{

public:
	void Initialize()override;
	void Update()override;
};

//�L�ђ�
class CombinedEnemiesStateStretch : public ICombinedEnemiesState
{
private:
	//�G�̐��ɂ��ړ��̔{��
	const float VELOCITY_EXTEND_ = 0.05f;
	//�ł���̃X�s�[�h�{��
	const float BIG_VELOCITY_EXTEND_ = 0.45f;

public:
	void Initialize()override;
	void Update()override;
};
