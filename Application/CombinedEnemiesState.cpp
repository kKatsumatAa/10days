#include "CombinedEnemiesState.h"
#include "CombinedEnemies.h"
#include "Util.h"
#include "MathUtil.h"
#include "GameVelocityManager.h"
#include "GameVelocityState.h"
#include "MathUtil.h"



float lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

std::unique_ptr<ICombinedEnemiesState> ICombinedEnemiesState::GetState(const std::string& name)
{
	std::unique_ptr<ICombinedEnemiesState>state = nullptr;

	if (name == "MOW_DOWN")
	{
		state = std::make_unique<CombinedEnemiesStateMoveMowDown>();
	}
	if (name == "SKEWER")
	{
		state = std::make_unique<CombinedEnemiesStateSkewer>();
	}
	if (name == "AFTER_COMBINED")
	{
		state = std::make_unique<CombinedEnemiesStateAfterCombined>();
	}
	if (name == "WAIT")
	{
		state = std::make_unique<CombinedEnemiesStateMoveWait>();
	}
	if (name == "SHRINK")
	{
		state = std::make_unique<CombinedEnemiesStateShrink>();
	}
	if (name == "WAIT_STRETCH")
	{
		state = std::make_unique<CombinedEnemiesStateWaitStretch>();
	}
	if (name == "STRETCH")
	{
		state = std::make_unique<CombinedEnemiesStateStretch>();
	}

	return std::move(state);
}

//-------------------------------------------------------------------------------------
//共通
void ICombinedEnemiesState::Initialize()
{
	enemies_->SetCentorPosTmp();
	enemies_->SetDirectionTmp();
	enemies_->SetLengthTmp();
}

void ICombinedEnemiesState::TimerUpdate(const std::function<void(float)>& f, const std::function<void()>& endF)
{
	float t = GetTimeRate();

	if (f)
	{
		f(t);
	}

	if (t >= 1.0f)
	{
		if (endF)
		{
			endF();
		}

		enemies_->ChangeState(nextStateName_);
		return;
	}

	timer_ += GameVelocityManager::GetInstance().GetVelocity();
}

void ICombinedEnemiesState::ShakeUpdate(float minS, float maxS, float rate, float t)
{
	enemies_->SetScaleSinRot(minS + lerp(0, 1.0f - minS, t), maxS + lerp(0, 1.0f - maxS, t), rate, (int32_t)timer_);
}

float ICombinedEnemiesState::GetTimeRate()
{
	return min(timer_ / timerMax_, 1.0f);
}

//-------------------------------------------------------------------------------------
//薙ぎ払われ中
void CombinedEnemiesStateMoveMowDown::Initialize()
{
	nextStateName_ = "AFTER_COMBINED";
	timerMax_ = Enemy::kMowFrame_;

	direction_ = -enemies_->GetDirection();
	//途中でステート変わったとき用
	enemies_->SetLength(enemies_->GetLengthTmp());
	enemies_->SetCentorPosTmp();
	enemies_->EnemiesScaleReset();
}

void CombinedEnemiesStateMoveMowDown::Update()
{
	Vec2 centorPT = enemies_->GetCentorPosTmp();

	std::function<void(float)>f = [=](float t) {
		enemies_->SetCentorPos({ lerp(centorPT.x, centorPT.x + direction_.x * Player::kMowDist_ * timerMax_,Math::Ease::EaseOutCirc(t,0,1.0f)),
			lerp(centorPT.y, centorPT.y + direction_.y * Player::kMowDist_ * timerMax_, Math::Ease::EaseOutCirc(t,0,1.0f)) });

		ShakeUpdate(minScale_, maxScale_, shakeRate_, t);
	};

	std::function<void()>endF = [=]() {
		enemies_->MowDownEnd();
	};

	TimerUpdate(f, endF);
}


//--------------------------------------------------------------------------------------
//突進されてる
void CombinedEnemiesStateSkewer::Initialize()
{
	//途中でステート変わったとき用
	enemies_->SetLength(enemies_->GetLengthTmp());
	enemies_->SetCentorPosTmp();
	enemies_->SetIsSkewer(true);
}

void CombinedEnemiesStateSkewer::Update()
{
	timer_++;

	enemies_->SetScaleSinRot(minScale_, maxScale_, shakeRate_, (int32_t)timer_);
	enemies_->SkewerUpdate();
}

//--------------------------------------------------------------------------------------
//合体後
void CombinedEnemiesStateAfterCombined::Initialize()
{
	timerMax_ = 50;
	nextStateName_ = "WAIT";
	enemies_->SetIsMowDown(false);
	enemies_->DirectionUpdate();
}

void CombinedEnemiesStateAfterCombined::Update()
{
	std::function<void(float)>f = [=](float t) {
		ShakeUpdate(minScale_, maxScale_, shakeRate_, t);
	};
	std::function<void()>endF = [=]() {
		enemies_->EnemiesScaleReset();
	};

	TimerUpdate(f, endF);
}

//--------------------------------------------------------------------------------------
//待機中
void CombinedEnemiesStateMoveWait::Initialize()
{
	nextStateName_ = "SHRINK";
	enemies_->SetIsMowDown(false);
}

void CombinedEnemiesStateMoveWait::Update()
{
	//向き更新
	enemies_->DirectionUpdate();

	std::function<void(float)>f = [=](float t) {
		ShakeUpdate(minScale_, maxScale_, shakeRate_, t);
	};

	TimerUpdate(f);
}


//--------------------------------------------------------------------------------------
//縮み中
void CombinedEnemiesStateShrink::Initialize()
{
	nextStateName_ = "WAIT_STRETCH";
	timerMax_ = 10;
	//基にするパラメータを保存
	ICombinedEnemiesState::Initialize();
}

void CombinedEnemiesStateShrink::Update()
{
	std::function<void(float)>f = [=](float t) {
		//長さセット
		enemies_->SetLength(lerp(enemies_->GetLengthTmp(), enemies_->GetLengthTmp() / 2.0f, Math::Ease::EaseInCubic(t, 0, 1.0f)));
		//スケール
		enemies_->SetScale({ lerp(1.0f, 1.5f, t),lerp(1.0f, 0.5f, t) });
	};

	TimerUpdate(f);
}



//--------------------------------------------------------------------------------------
//伸び待機
void CombinedEnemiesStateWaitStretch::Initialize()
{
	nextStateName_ = "STRETCH";
}

void CombinedEnemiesStateWaitStretch::Update()
{
	std::function<void(float)>f = [=](float t) {
		//長さセット
		enemies_->SetLength(enemies_->GetLengthTmp() / 2.0f);
		//スケール
		enemies_->SetScale({ 1.5f,0.5f });
	};

	TimerUpdate(f);
}


//--------------------------------------------------------------------------------------
//伸び中
void CombinedEnemiesStateStretch::Initialize()
{
	nextStateName_ = "WAIT";
	timerMax_ = 13;
}

void CombinedEnemiesStateStretch::Update()
{
	Vec2 centorPT = enemies_->GetCentorPosTmp();
	Vec2 directionT = enemies_->GetDirectionTmp();
	float radiusT = enemies_->GetLengthTmp();

	std::function<void(float)>f = [=](float t) {
		//半径セット
		enemies_->SetLength(lerp(enemies_->GetLengthTmp() / 2.0f, enemies_->GetLengthTmp(), Math::Ease::EaseInCubic(t, 0, 1.0f)));
		//半径分向いてる方向に進む
		Vec2 dirL = directionT * radiusT * 2.0f / (float)enemies_->GetEnemiesNum();
		//でかつよなら
		if (enemies_->GetIsBigDango())
		{
			dirL *= BIG_VELOCITY_EXTEND_;
		}
		else
		{
			dirL = dirL + dirL * VELOCITY_EXTEND_ * (float)(enemies_->GetEnemiesNum() - 1);
		}
		Vec2 nextPos = { lerp(centorPT.x, centorPT.x + dirL.x, t),
			lerp(centorPT.y, centorPT.y + dirL.y, t) };

		//スケール
		enemies_->SetScale({ lerp(1.5f,1.0f, t),lerp(0.5f,1.0f,  t) });

		//すり抜けたら
		Vec2 directionT2 = (enemies_->GetTargetPos() - enemies_->GetCentorPos()).GetNormalize();
		if ((enemies_->GetTargetPos() - nextPos).GetNormalize().Dot(directionT2) <= 0.0f)
		{
			nextPos = enemies_->GetTargetPos() + (-directionT2 * enemies_->GetLengthTmp() / 2.0f);
		}

		enemies_->SetCentorPos(nextPos);
	};

	TimerUpdate(f);
}


