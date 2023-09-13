#pragma once
#include<memory>
#include<string>
#include"Vec2.h"
#include<functional>


//前方宣言
class CombinedEnemies;

//くっついた敵のステート親クラス
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


//薙ぎ払われ中
class CombinedEnemiesStateMoveMowDown : public ICombinedEnemiesState
{
private:
	Vec2 direction_ = { 0,0 };

protected:
	//ぶるんのパラメータ
	//最小スケール
	float minScale_ = 0.6f;
	//最大スケール
	float maxScale_ = 1.1f;
	//どのくらいのスピードでやるか
	float shakeRate_ = 0.9f;

public:
	void Initialize()override;
	void Update()override;
};

//突進され中
class CombinedEnemiesStateSkewer : public ICombinedEnemiesState
{
private:
	Vec2 direction_ = { 0,0 };

protected:
	//ぶるんのパラメータ
	//最小スケール
	float minScale_ = 0.8f;
	//最大スケール
	float maxScale_ = 1.3f;
	//どのくらいのスピードでやるか
	float shakeRate_ = 0.2f;

public:
	void Initialize()override;
	void Update()override;
};

//合体後
class CombinedEnemiesStateAfterCombined : public ICombinedEnemiesState
{
protected:
	//ぶるんのパラメータ
	//最小スケール
	float minScale_ = 0.5f;
	//最大スケール
	float maxScale_ = 1.2f;
	//どのくらいのスピードでやるか
	float shakeRate_ = 0.8f;

public:
	void Initialize()override;
	void Update()override;
};

//待機中
class CombinedEnemiesStateMoveWait : public ICombinedEnemiesState
{
protected:
	//ぶるんのパラメータ
	//最小スケール
	float minScale_ = 0.7f;
	//最大スケール
	float maxScale_ = 1.1f;
	//どのくらいのスピードでやるか
	float shakeRate_ = 0.7f;

public:
	void Initialize()override;
	void Update()override;
};

//縮み中
class CombinedEnemiesStateShrink : public ICombinedEnemiesState
{

public:
	void Initialize()override;
	void Update()override;
};

//縮んで待ってる
class CombinedEnemiesStateWaitStretch : public ICombinedEnemiesState
{

public:
	void Initialize()override;
	void Update()override;
};

//伸び中
class CombinedEnemiesStateStretch : public ICombinedEnemiesState
{
private:
	//敵の数による移動の倍率
	const float VELOCITY_EXTEND_ = 0.05f;
	//でかつよのスピード倍率
	const float BIG_VELOCITY_EXTEND_ = 0.45f;

public:
	void Initialize()override;
	void Update()override;
};
