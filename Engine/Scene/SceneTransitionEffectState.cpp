#include "SceneTransitionEffectState.h"
#include "SceneTransitionManager.h"
#include "Util.h"


void SceneTransitionEffectState::SetSceneTransitionEffect(SceneTransitionManager* sceneTransitionEffect,
	const std::function<void()>& loadFunc)
{
	sceneTransitionManager_ = sceneTransitionEffect;
	loadFunc_ = loadFunc;
}

void SceneTransitionEffectState::Update()
{
	timer_++;

	sceneTransitionManager_->GetSceneTransition().Update();
}

void SceneTransitionEffectState::Draw()
{
	sceneTransitionManager_->GetSceneTransition().Draw();
}

//--------------------------------------------------------
//�������Ă��Ȃ�
void TransitionEffectNothingState::Initialize()
{
	//���o�t���O�I�t
	sceneTransitionManager_->SetIsDoingEffect(false);
}

void TransitionEffectNothingState::Update()
{
	SceneTransitionEffectState::Update();
}


//--------------------------------------------------------
//���o�n��
void TransitionEffectBeginState::Initialize()
{
	//�J�ډ��o�n��
	sceneTransitionManager_->GetSceneTransition().Start();
}

void TransitionEffectBeginState::Update()
{
	SceneTransitionEffectState::Update();

	//���Ԃ��I�������
	if (sceneTransitionManager_->GetSceneTransition().GetIsBeginEnd())
	{
		//���o�Ȃ��X�e�[�g��
		sceneTransitionManager_->ChangeEffectState(std::make_unique<TransitionEffectDoingState>());
	}
}


//--------------------------------------------------------
//���o��
void TransitionEffectDoingState::Initialize()
{
	//�a�����Ă������[�h�p�̊֐������s
	sceneTransitionManager_->BeginAsyncLoad(loadFunc_);
}

void TransitionEffectDoingState::Update()
{
	SceneTransitionEffectState::Update();

	//���[�f�B���O�I�������
	if (!sceneTransitionManager_->GetIsLoadingOnly())
	{
		sceneTransitionManager_->ChangeEffectState(std::make_unique<TransitionEffectEndState>());
	}
}


//--------------------------------------------------------
//���o�I���
void TransitionEffectEndState::Initialize()
{
	//�J�ډ��o�I���
	//sceneTransitionManager_->GetSceneTransition().End();
}

void TransitionEffectEndState::Update()
{
	SceneTransitionEffectState::Update();

	//���Ԃ��I�������
	if (sceneTransitionManager_->GetSceneTransition().GetIsExe())
	{
		//���o�Ȃ��X�e�[�g��
		sceneTransitionManager_->ChangeEffectState(std::make_unique<TransitionEffectNothingState>());
	}
}