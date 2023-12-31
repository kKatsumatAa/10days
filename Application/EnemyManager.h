#pragma once
#include "CombinedEnemies.h"
#include "CombinedEnemiesState.h"
#include "Stage.h"
#include <sstream>


//敵管理クラス
class EnemyManager final
{
private://変数
	//敵の配列
	std::vector<std::unique_ptr<Enemy>>enemies_;
	//くっついた敵同士をまとめた配列
	std::vector<std::unique_ptr<CombinedEnemies>> combinedEnemiesArray_;
	//プレイヤーのポインタ
	Player* player_ = nullptr;
	//ステージの
	Stage* stage_ = nullptr;
	//攻撃に当たった敵を保管
	std::unique_ptr<CombinedEnemies>mowDownedEnemies_ = nullptr;

	//敵の上限
	const uint32_t ENEMIES_MAX_ = 5;
	const uint32_t TUTORIAL_ENEMIES_MAX_ = 20;
	bool isAddEnemy_ = true;

	uint64_t enemyTexHandle_ = 0;
	uint64_t BigEnemyTexHandle_ = 0;
	uint64_t warningTexHandle_ = 0;

	//倒された敵の数用
	bool isDefeatedEnemies_ = false;
	uint32_t defeatedEnemiesNum_ = 0;

	//敵発生コマンド
	std::stringstream enemyPopCommands_;
	bool isWaitingPop_ = false;
	float popWaitTimer_ = 0;
	std::string popFileName_;

	const std::string ENEMIES_DATA_PATH_ = "Resources/enemyData/";

	//敵が画面外にいるときのUI
	std::vector<std::unique_ptr<Object>> enemyWarningUIs_;

private://関数
	EnemyManager() { ; }
	~EnemyManager() { ; }

public:
	EnemyManager& operator=(const EnemyManager& inst) = delete;
	EnemyManager(const EnemyManager& inst) = delete;

	static EnemyManager& GetInstance();

public:
	void Initialize(Player* player, Stage* stage);
	void Update();
	void Draw();

public:
	//csv読み込み
	void LoadEnemiesDataCSV(const std::string& name);
private:
	//csv読み込み更新処理
	void LoadEnemiesDataCSVUpdate(float speed);

private:
	//薙ぎ払われた敵をまとめて保存しておく
	void SaveMowDownEnemies();
	//くっつける更新処理
	void CombinedUpdate();
	//串刺しの敵をくっつける処理
	void SkewerCombinedUpdate();

	//生成の更新
	void GenerateUpdate();
	//薙ぎ払われてる最中の敵の塊の更新
	void MowDownTriggerEnemiesUpdate();

	//薙ぎ払われた瞬間を終わらせる
	void MowDownTriggerEnd();

private:
	//敵の数を取得
	int32_t GetEnemiesCount();

private:
	void CoalesceneceCombEnemiesEachOther(std::function<bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator,
		std::vector<std::unique_ptr<CombinedEnemies>>::iterator)> ifF);

	/// <summary>
	/// 合体してる敵を全て調べて合体させるか決める
	/// </summary>
	/// <param name="combEnemies"></param>
	/// <param name="settingFlag"></param>
	/// <param name="ifF"></param>
	void CheckCombinedEnemiesCondition(std::unique_ptr<CombinedEnemies>& combEnemies, bool& settingFlag,
		std::function <bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator) > ifF);

	/// <summary>
	/// 敵一体一体を調べて合体させるか決める
	/// </summary>
	/// <param name="enemy"></param>
	/// <param name="combEnemies"></param>
	/// <param name="settingFlag"></param>
	void CheckEnemiesCondition(std::unique_ptr<CombinedEnemies>& combEnemies,
		bool& settingFlag, std::function<bool(std::unique_ptr<Enemy>&)>ifF,
		std::function<bool(int32_t)>ifF2);
	/// <summary>
	/// 敵同士をつっくける共通処理
	/// </summary>
	/// <param name="combEnemies">くっついてる敵の配列（新たに作られる場合はnullptr）</param>
	/// <param name="isCombined">敵一体一体を追加していくか</param>
	/// <param name="isDockCombined">すでにある合体した敵に追加していくか</param>
	/// <param name="isPushBackArray">配列に追加するか</param>
	void AddCombinedEnemies(std::unique_ptr<CombinedEnemies> combEnemies, bool isCombined, bool isDockCombined,
		std::function<bool(std::vector<std::unique_ptr<Enemy>>::iterator)> addIfF, bool isPushBackArray = true);

public:
	void AddEnemy(std::unique_ptr<Enemy>enemy);
	void AddEnemy(const Vec2& pos, uint32_t combinedNum);

	//倒した敵の数(倒された敵がいたらtrue)
	bool GetDefeatedEnemiesNum(uint32_t& enemiesNum);
	//突進されてる敵の数
	uint32_t GetSkewerEnemiesNum();
	//くっついた瞬間の敵がいるか
	bool GetIsDockingTriggerAnyEnemy();
	//でかつよがいるか
	bool GetIsBigDangoAnyEnemy();

public:
	//突進されてる敵の塊の半径
	float GetSkewerEnemiesLength();

	void SetIsAddEnemy();
};


