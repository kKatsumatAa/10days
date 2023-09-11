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
//共通の処理
void GameVelocityState::Initialize(float slowVelBegin, float slowVelEnd, int32_t slowTimeMax)
{
	//線形補完の開始の値
	slowVelBegin_ = slowVelBegin;
	//終了時の値
	slowVelEnd_ = slowVelEnd;
	//時間上限
	slowTimeMax_ = slowTimeMax;
}

//共通の処理
void GameVelocityState::CommonUpdate(const std::function<void()>& endF)
{
	//経過割合
	float t = (float)time_ / (float)slowTimeMax_;

	//速度設定
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
//何もしない
void GameVelocityStateNone::Initialize(float slowVelBegin, float slowVelEnd, int32_t slowTimeMax)
{
	//共通の処理
	GameVelocityState::Initialize(slowVelBegin, slowVelEnd, slowTimeMax);
}

void GameVelocityStateNone::Update()
{
	//スローモーションフラグオフ
	GameVelocityManager::GetInstance().SetIsSlowMotion(false);
}


//-------------------------------------------------------------------------------------------
//開始
void GameVelocityStateBegin::Initialize(float slowVelBegin, float slowVelEnd, int32_t slowTimeMax)
{
	//次のステートの名前
	nextStateName_ = "DOING";

	//共通の処理
	GameVelocityState::Initialize(slowVelBegin, slowVelEnd, slowTimeMax);
}

void GameVelocityStateBegin::Update()
{
	//共通の処理
	CommonUpdate(NULL);
}


//--------------------------------------------------------------------------------------------
//実行中
void GameVelocityStateDoing::Initialize(float slowVelBegin, float slowVelEnd, int32_t slowTimeMax)
{
	//共通の処理
	GameVelocityState::Initialize(slowVelBegin, slowVelEnd, slowTimeMax);
}

void GameVelocityStateDoing::Update()
{
	//GameVelocityManagerの方でステート変更するのでなし
}


//--------------------------------------------------------------------------------------------
//解除
void GameVelocityStateEnd::Initialize(float slowVelBegin, float slowVelEnd, int32_t slowTimeMax)
{
	//次のステートの名前
	nextStateName_ = "NONE";

	//共通の処理
	GameVelocityState::Initialize(slowVelBegin, slowVelEnd, slowTimeMax);
}

void GameVelocityStateEnd::Update()
{
	//共通の処理
	std::function<void()>endF = [=]() {GameVelocityManager::GetInstance().SetIsSlowMotion(false); };
	CommonUpdate(endF);
}
