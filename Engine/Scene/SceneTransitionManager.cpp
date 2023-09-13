#include "SceneTransitionManager.h"
#include "SceneTransitionEffectState.h"


const std::string SceneTransitionManager::TRANSITION_SOUND_NAME_ = "noise.wav";



SceneTransitionManager& SceneTransitionManager::GetInstance()
{
	static SceneTransitionManager inst;
	return inst;
}

void SceneTransitionManager::Initialize()
{
	isDoingEffect_ = false;

	transition_.StaticInit();

	//�X�e�[�g
	ChangeEffectState(std::make_unique<TransitionEffectNothingState>());
}

void SceneTransitionManager::ChangeEffectState(std::unique_ptr<SceneTransitionEffectState> state)
{
	if (effectState_)
	{
		effectState_.reset();
	}
	effectState_ = std::move(state);
	effectState_->SetSceneTransitionEffect(this, loadFunc_);
	effectState_->Initialize();
}

void SceneTransitionManager::BeginSceneTransition(const std::function<void()>& loadFunc)
{
	if (isDoingEffect_ == false)
	{
		Sound::GetInstance().PlayWave("sceneChange_SE.wav", 0.2f);
	}
	isDoingEffect_ = true;

	//�ǂݍ��݂̊֐��R�s�[
	loadFunc_ = loadFunc;

	//�X�e�[�g�ύX
	ChangeEffectState(std::make_unique<TransitionEffectBeginState>());
}

void SceneTransitionManager::BeginAsyncLoad(std::function<void()> loadFunc)
{
	//�񓯊��œǂݍ��݊J�n
	async_.StartAsyncFunction(loadFunc);
}

//---------------------------------------------
void SceneTransitionManager::Update()
{
	//���[�h�I������u��
	if (async_.GetFinishedAsync())
	{
		async_.EndThread();
	}

	effectState_->Update();
}

void SceneTransitionManager::Draw()
{
	effectState_->Draw();
}
