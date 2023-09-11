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
//何もしていない
void TransitionEffectNothingState::Initialize()
{
	//演出フラグオフ
	sceneTransitionManager_->SetIsDoingEffect(false);
}

void TransitionEffectNothingState::Update()
{
	SceneTransitionEffectState::Update();
}


//--------------------------------------------------------
//演出始め
void TransitionEffectBeginState::Initialize()
{
	//遷移演出始め
	sceneTransitionManager_->GetSceneTransition().Start();
}

void TransitionEffectBeginState::Update()
{
	SceneTransitionEffectState::Update();

	//時間が終わったら
	if (sceneTransitionManager_->GetSceneTransition().GetIsBeginEnd())
	{
		//演出ないステートに
		sceneTransitionManager_->ChangeEffectState(std::make_unique<TransitionEffectDoingState>());
	}
}


//--------------------------------------------------------
//演出中
void TransitionEffectDoingState::Initialize()
{
	//預かっていたロード用の関数を実行
	sceneTransitionManager_->BeginAsyncLoad(loadFunc_);
}

void TransitionEffectDoingState::Update()
{
	SceneTransitionEffectState::Update();

	//ローディング終わったら
	if (!sceneTransitionManager_->GetIsLoadingOnly())
	{
		sceneTransitionManager_->ChangeEffectState(std::make_unique<TransitionEffectEndState>());
	}
}


//--------------------------------------------------------
//演出終わり
void TransitionEffectEndState::Initialize()
{
	//遷移演出終わり
	//sceneTransitionManager_->GetSceneTransition().End();
}

void TransitionEffectEndState::Update()
{
	SceneTransitionEffectState::Update();

	//時間が終わったら
	if (sceneTransitionManager_->GetSceneTransition().GetIsExe())
	{
		//演出ないステートに
		sceneTransitionManager_->ChangeEffectState(std::make_unique<TransitionEffectNothingState>());
	}
}