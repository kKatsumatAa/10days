#include "GameVelocityState.h"
#include "GameVelocityManager.h"
#include "Util.h"


std::unique_ptr<GameVelocityState> GameVelocityState::GetState(const std::string& name)
{
	std::unique_ptr<GameVelocityState> state = nullptr;

	if (name == "NONE")
	{
		state = std::make_unique<GameVelocityStateNone>();
	}
	if (name == "BEGIN")
	{
		state = std::make_unique<GameVelocityStateBegin>();
	}
	if (name == "DOING")
	{
		state = std::make_unique<GameVelocityStateDoing>();
	}
	if (name == "END")
	{
		state = std::make_unique<GameVelocityStateEnd>();
	}

	return std::move(state);
}

//------------------------------------------------------------------
//���ʂ̏���
void GameVelocityState::Initialize(float slowVelBegin, float slowVelEnd, int32_t slowTimeMax)
{
	//���`�⊮�̊J�n�̒l
	slowVelBegin_ = slowVelBegin;
	//�I�����̒l
	slowVelEnd_ = slowVelEnd;
	//���ԏ��
	slowTimeMax_ = slowTimeMax;
}

//���ʂ̏���
void GameVelocityState::CommonUpdate(const std::function<void()>& endF)
{
	//�o�ߊ���
	float t = (float)time_ / (float)slowTimeMax_;

	//���x�ݒ�
	GameVelocityManager::GetInstance().SetVelocity(LerpVec3({ slowVelBegin_,0,0 }, { slowVelEnd_,0,0 }, EaseInOut(t)).x);

	if (t >= 1.0f)
	{
		if (endF)
		{
			endF();
		}

		GameVelocityManager::GetInstance().ChangeSlowState(nextStateName_);
	}

	time_++;
}


//-------------------------------------------------------------------------------------------
//�������Ȃ�
void GameVelocityStateNone::Initialize(float slowVelBegin, float slowVelEnd, int32_t slowTimeMax)
{
	//���ʂ̏���
	GameVelocityState::Initialize(slowVelBegin, slowVelEnd, slowTimeMax);
}

void GameVelocityStateNone::Update()
{
	//�X���[���[�V�����t���O�I�t
	GameVelocityManager::GetInstance().SetIsSlowMotion(false);
}


//-------------------------------------------------------------------------------------------
//�J�n
void GameVelocityStateBegin::Initialize(float slowVelBegin, float slowVelEnd, int32_t slowTimeMax)
{
	//���̃X�e�[�g�̖��O
	nextStateName_ = "DOING";

	//���ʂ̏���
	GameVelocityState::Initialize(slowVelBegin, slowVelEnd, slowTimeMax);
}

void GameVelocityStateBegin::Update()
{
	//���ʂ̏���
	CommonUpdate(NULL);
}


//--------------------------------------------------------------------------------------------
//���s��
void GameVelocityStateDoing::Initialize(float slowVelBegin, float slowVelEnd, int32_t slowTimeMax)
{
	//���ʂ̏���
	GameVelocityState::Initialize(slowVelBegin, slowVelEnd, slowTimeMax);
}

void GameVelocityStateDoing::Update()
{
	//GameVelocityManager�̕��ŃX�e�[�g�ύX����̂łȂ�
}


//--------------------------------------------------------------------------------------------
//����
void GameVelocityStateEnd::Initialize(float slowVelBegin, float slowVelEnd, int32_t slowTimeMax)
{
	//���̃X�e�[�g�̖��O
	nextStateName_ = "NONE";

	//���ʂ̏���
	GameVelocityState::Initialize(slowVelBegin, slowVelEnd, slowTimeMax);
}

void GameVelocityStateEnd::Update()
{
	//���ʂ̏���
	std::function<void()>endF = [=]() {GameVelocityManager::GetInstance().SetIsSlowMotion(false); };
	CommonUpdate(endF);
}
