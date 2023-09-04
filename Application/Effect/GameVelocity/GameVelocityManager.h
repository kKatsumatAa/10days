#pragma once
#include<string>
#include"Vec3.h"
#include<memory>

//前方宣言
class GameVelocityState;

//ゲームスピード管理クラス
class GameVelocityManager final
{
private:
	struct SlowMotionData
	{
		//スローモーションが有効か
		bool isSlowMotion = false;
		//スローの初期値
		float slowVelBegin = 0;
		//スローの終わり値
		float slowVelEnd = 0;
		//スローの時間上限
		int32_t slowTimeMax = 0;
	};

private:
	float gameVelocity_ = 0;
	//色
	const Vec3 VEL_COL_MIN_ = { 0.5f,0.8f,0.8f };

	//スローモーション
	SlowMotionData slowData_;
	//スローモーションステート
	std::unique_ptr<GameVelocityState>slowState_ = nullptr;


public:
	const float GAME_VELOCITY_MAX_ = 1.0f;
	const float GAME_VELOCITY_MIN_ = 0.03f;

private:
	GameVelocityManager() { ; }
	~GameVelocityManager() { ; }

public:
	GameVelocityManager(const GameVelocityManager& other) = delete;
	GameVelocityManager& operator=(const GameVelocityManager& other) = delete;

	static GameVelocityManager& GetInstance();

public:
	void Initialize();

	void Update();

	void UpdateImGui();

public:
	//スローモーションステート変更
	void ChangeSlowState(const std::string& name);

private:
	//スローモーションのデータをセット
	void SetSlowMotionData(int32_t lerpFrame, float velocity, bool isSlowMotion);

public:
	/// <summary>
///スローモーション始める
/// </summary>
/// <param name="lerpFrame">徐々にスローになるフレーム数</param>
/// <param name="endVelocity">最終的なゲームスピード</param>
	void BeginSlowMotion(int32_t lerpFrame, float endVelocity);

	/// <summary>
	/// スローモーション終わる
	/// </summary>
	/// <param name="lerpFrame">徐々にスローが終わるフレーム数</param>
	/// <param name="endVelocity">最終的なゲームスピード</param>
	void EndSlowMotion(int32_t lerpFrame, float endVelocity);
	//引数設定しないver（スタート時のパラメータ使用する）
	void EndSlowMotion();

public:
	//ゲームのスピードを取得
	float GetVelocity();
	void SetVelocity(float velocity) { gameVelocity_ = velocity; }

	//スローモーション中か
	bool GetIsSlowMotion() { return slowData_.isSlowMotion; }
	void SetIsSlowMotion(bool isSlowMotion) { slowData_.isSlowMotion = isSlowMotion; }
};