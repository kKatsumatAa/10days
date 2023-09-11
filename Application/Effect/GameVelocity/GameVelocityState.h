#pragma once
#include<memory>
#include<string>
#include<functional>


//前方宣言
class GameVelocityManager;

//ゲームスピードクラスのステート親クラス
class GameVelocityState
{
protected:
	//経過時間
	int32_t time_ = 0;
	//スローの初期値
	float slowVelBegin_ = 0;
	//スローの終わり値
	float slowVelEnd_ = 0;
	//スローの時間上限
	int32_t slowTimeMax_ = 0;
	//次のステートの名前
	std::string nextStateName_;

public:
	virtual ~GameVelocityState() { ; }

	virtual void Initialize(float slowVelBegin, float slowVelEnd, int32_t slowTimeMax);
	virtual void Update() = 0;
	void CommonUpdate(const std::function<void()>& endF);

public:
	static std::unique_ptr<GameVelocityState>GetState(const std::string& name);
};


//何もしない
class GameVelocityStateNone : public GameVelocityState
{
public:
	void Initialize(float slowVelBegin, float slowVelEnd, int32_t slowTimeMax)override;
	void Update() override;
};

//スローモーション始め
class GameVelocityStateBegin : public GameVelocityState
{
public:
	void Initialize(float slowVelBegin, float slowVelEnd, int32_t slowTimeMax)override;
	void Update() override;
};

//スローモーション中
class GameVelocityStateDoing : public GameVelocityState
{
public:
	void Initialize(float slowVelBegin, float slowVelEnd, int32_t slowTimeMax)override;
	void Update() override;
};

//解除
class GameVelocityStateEnd : public GameVelocityState
{
public:
	void Initialize(float slowVelBegin, float slowVelEnd, int32_t slowTimeMax)override;
	void Update() override;
};