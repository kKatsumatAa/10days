#include "EnemyManager.h"
#include "KeyboardInput.h"
#include <memory>
#include "Util.h"
#include "Score.h"
#include <fstream>
#include "GameVelocityManager.h"
#include "GameVelocityState.h"


EnemyManager& EnemyManager::GetInstance()
{
	static EnemyManager sInst;
	return sInst;
}

//---------------------------------------------
void EnemyManager::Initialize(Player* player, Stage* stage)
{
	player_ = player;
	stage_ = stage;

	enemies_.clear();
	combinedEnemiesArray_.clear();

	enemyTexHandle_ = TextureManager::LoadGraph("enemy.png");
	BigEnemyTexHandle_ = TextureManager::LoadGraph("big_enemy.png");

	enemyPopCommands_.clear();
	isWaitingPop_ = false;
}

void EnemyManager::SaveMowDownEnemies()
{
	//薙ぎ払われた敵がいるか
	bool isMowDowned = false;
	bool isMowDownedComb = false;

	//薙ぎ払われてる敵の塊がいるか
	std::function<bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator)> ifF
		= [=](std::vector<std::unique_ptr<CombinedEnemies>>::iterator itr)
	{
		return itr->get()->GetIsMowDownTriggerAnyEnemy();
	};
	CheckCombinedEnemiesCondition(mowDownedEnemies_, isMowDownedComb, ifF);

	//薙ぎ払われてる最中か
	for (auto itrB = combinedEnemiesArray_.begin(); itrB != combinedEnemiesArray_.end(); itrB++)
	{
		//薙ぎ払われてる最中の敵の塊があるか
		if (itrB->get()->GetIsMowDownTriggerAnyEnemy())
		{
			//敵のまとまりに別のまとまりを追加
			if (mowDownedEnemies_ == nullptr)
			{
				mowDownedEnemies_ = std::make_unique<CombinedEnemies>();
				mowDownedEnemies_->Initialize(player_, stage_, player_->GetDirectionVec());
			}
			//てきのかたまりを追加する
			mowDownedEnemies_->AddCombinedEnemies(std::move(*itrB));
			combinedEnemiesArray_.erase(itrB);

			if (combinedEnemiesArray_.size())
			{
				itrB = combinedEnemiesArray_.begin();
			}
			else
			{
				break;
			}
		}
	}

	////薙ぎ払われてる最中の敵がいるか
	//std::function<bool(std::unique_ptr<Enemy>&)>ifF2 = [=](std::unique_ptr<Enemy>& enemy)
	//{
	//	return enemy->GetIsMowDownTrigger();
	//};
	//std::function<bool(int32_t)>ifF3 = [=](int32_t count)
	//{
	//	return count >= 1;
	//};
	//CheckEnemiesCondition(mowDownedEnemies_, isMowDowned, ifF2, ifF3);

	//if (mowDownedEnemies_)
	//{
	//	mowDownedEnemies_.get();
	//}

	////薙ぎ払われてる最中の敵を追加していく
	//std::function<bool(std::vector<std::unique_ptr<Enemy>>::iterator)>addIfF =
	//	[=](std::vector<std::unique_ptr<Enemy>>::iterator itr)
	//{
	//	return itr->get()->GetIsMowDownTrigger();
	//};
	//AddCombinedEnemies(std::move(mowDownedEnemies_), isMowDowned, isMowDownedComb, addIfF, false);
}

void EnemyManager::CombinedUpdate()
{
	//薙ぎ払われた敵がいるか
	bool isMowDowned = false;
	bool isMowDownedComb = false;
	//くっついてる敵で合体したものをまとめる配列
	std::unique_ptr<CombinedEnemies> combEnemies = nullptr;
	uint32_t count = 0;

	//くっついてる敵の中でほかのくっついてる敵と合体したか
	std::function<bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator, std::vector<std::unique_ptr<CombinedEnemies>>::iterator)>ifF0
		= [=](std::vector<std::unique_ptr<CombinedEnemies>>::iterator itrA, std::vector<std::unique_ptr<CombinedEnemies>>::iterator itrB)
	{
		return (itrA->get()->GetIsDockingAnyEnemy())
			&& (itrB->get()->GetIsDockingAnyEnemy());
	};
	CoalesceneceCombEnemiesEachOther(ifF0);

	//くっついてる敵が、ほかの敵と合体したか
	std::function<bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator)> ifF
		= [=](std::vector<std::unique_ptr<CombinedEnemies>>::iterator itr)
	{
		return itr->get()->GetIsDockingAnyEnemy();
	};
	CheckCombinedEnemiesCondition(combEnemies, isMowDownedComb, ifF);

	////敵と合体したか
	//std::function<bool(std::unique_ptr<Enemy>&)>ifF2 = [=](std::unique_ptr<Enemy>& enemy)
	//{
	//	return enemy->GetIsDocking() || enemy->GetIsMowDownTrigger();
	//};
	//std::function<bool(int32_t)>ifF3 = [=](int32_t count)
	//{
	//	return count >= 2;
	//};
	//CheckEnemiesCondition(combEnemies, isMowDowned, ifF2, ifF3);

	//合体してる敵に敵を一体一体追加
	std::function<bool(std::vector<std::unique_ptr<Enemy>>::iterator)>addIfF =
		[=](std::vector<std::unique_ptr<Enemy>>::iterator itr)
	{
		return itr->get()->GetIsDocking() || itr->get()->GetIsMowDownTrigger();
	};
	AddCombinedEnemies(std::move(combEnemies), isMowDowned, isMowDownedComb, addIfF);
}

//突進の時に合体させる
void EnemyManager::SkewerCombinedUpdate()
{
	//串刺しの敵と合体したか
	bool isSkewerDocking = false;
	bool isDockSkewCombined = false;
	//くっついてる敵で合体したものをまとめる配列
	std::unique_ptr<CombinedEnemies> skewerCombEnemies = nullptr;
	uint32_t count = 0;

	//くっついてる敵の中でほかのくっついてる敵と合体したか
	std::function<bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator, std::vector<std::unique_ptr<CombinedEnemies>>::iterator)>ifF
		= [=](std::vector<std::unique_ptr<CombinedEnemies>>::iterator itrA, std::vector<std::unique_ptr<CombinedEnemies>>::iterator itrB)
	{
		return itrA->get()->GetIsSkewer() && itrB->get()->GetIsSkewer();
	};
	CoalesceneceCombEnemiesEachOther(ifF);

	//くっついてる敵が、ほかの突進されてる敵に当たったか
	std::function<bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator)> ifF2 = [=](std::vector<std::unique_ptr<CombinedEnemies>>::iterator itr)
	{
		return itr->get()->GetIsDockingAndSkewer();
	};
	CheckCombinedEnemiesCondition(skewerCombEnemies, isDockSkewCombined, ifF2);

	////串ざされてる敵と合体したか
	//std::function<bool(std::unique_ptr<Enemy>&)>ifF3 = [=](std::unique_ptr<Enemy>& enemy)
	//{
	//	return /*enemy->GetIsDocking() && */enemy->GetIsSkewer();
	//};
	//std::function<bool(int32_t)>ifF4 = [=](int32_t count)
	//{
	//	return count >= 1;
	//};
	//CheckEnemiesCondition(skewerCombEnemies, isSkewerDocking, ifF3, ifF4);

	//くっついてる敵に追加していく処理
	std::function<bool(std::vector<std::unique_ptr<Enemy>>::iterator)>addIfF =
		[=](std::vector<std::unique_ptr<Enemy>>::iterator itr)
	{
		return itr->get()->GetIsSkewer();
	};
	AddCombinedEnemies(std::move(skewerCombEnemies), isSkewerDocking, isDockSkewCombined, addIfF);
}

void EnemyManager::GenerateUpdate()
{
	if (GetEnemiesCount() < (int32_t)ENEMIES_MAX_)
	{
		float x = GetRand(100, 1100);
		float y = GetRand(100.0f, 600.0f);

		EnemyManager::GetInstance().AddEnemy({ x,y }, (uint32_t)GetRand(1, 4));
	}
}

void EnemyManager::MowDownTriggerEnemiesUpdate()
{
	if (mowDownedEnemies_)
	{
		//正面にテレポート
		mowDownedEnemies_->SetDirection(-player_->GetMoveVec());
		mowDownedEnemies_->CalcCentorPos(player_->GetPos(), player_->GetMoveVec().GetNormalize(),
			player_->kMowSupportCenterDist_ + mowDownedEnemies_->GetLength() + player_->GetRad().x);
		mowDownedEnemies_->EnemiesPosUpdate();

		//薙ぎ払う
		mowDownedEnemies_->MowDown();
		//配列に登録
		combinedEnemiesArray_.push_back(std::move(mowDownedEnemies_));
		mowDownedEnemies_ = nullptr;
	}

}

void EnemyManager::MowDownTriggerEnd()
{
	for (auto& enemy : enemies_)
	{
		enemy->SetIsMowDownTrigger(false);
	}
	for (auto& enemies : combinedEnemiesArray_)
	{
		enemies->MowDownTriggerEnd();
	}
	if (mowDownedEnemies_)
	{
		mowDownedEnemies_->MowDownEnd();
	}
}

int32_t EnemyManager::GetEnemiesCount()
{
	int32_t count = 0;

	count += (int32_t)enemies_.size();

	count += (int32_t)combinedEnemiesArray_.size();

	if (mowDownedEnemies_)
	{
		count++;
	}

	return count;
}

//------------------------------------------------------------------------------------------------------------------
//共通処理
void EnemyManager::CoalesceneceCombEnemiesEachOther(std::function<bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator,
	std::vector<std::unique_ptr<CombinedEnemies>>::iterator)> ifF)
{
	//二つ以上ないとがったいできないので
	if (combinedEnemiesArray_.size() >= 2)
	{
		for (std::vector<std::unique_ptr<CombinedEnemies>>::iterator itr = combinedEnemiesArray_.begin();
			itr != combinedEnemiesArray_.end();
			itr++)
		{
			if (combinedEnemiesArray_.size() < 2)
			{
				break;
			}

			auto itrB = itr;
			itrB++;

			for (itrB; itrB != combinedEnemiesArray_.end(); itrB++)
			{
				//条件クリアすれば
				if (ifF(itr, itrB) && itr != itrB)
				{
					//敵のまとまりに別のまとまりを追加
					itr->get()->AddCombinedEnemies(std::move(*itrB));
					combinedEnemiesArray_.erase(itrB);

					if (combinedEnemiesArray_.size() >= 2)
					{
						itrB = combinedEnemiesArray_.begin();
					}
					else
					{
						itrB = combinedEnemiesArray_.begin();
						break;
					}
				}
			}
		}
	}
}

void EnemyManager::CheckCombinedEnemiesCondition(std::unique_ptr<CombinedEnemies>& combEnemies, bool& settingFlag,
	std::function<bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator)> ifF)
{
	for (auto itr = combinedEnemiesArray_.begin(); itr != combinedEnemiesArray_.end(); itr++)
	{
		if (ifF(itr))
		{
			//所有権渡す
			combEnemies = std::move(*itr);
			combinedEnemiesArray_.erase(itr);
			settingFlag = true;
			break;
		}
	}
}

void EnemyManager::CheckEnemiesCondition(std::unique_ptr<CombinedEnemies>& combEnemies, bool& settingFlag,
	std::function<bool(std::unique_ptr<Enemy>&)> ifF,
	std::function<bool(int32_t)>ifF2)
{
	int32_t count = 0;

	for (auto& enemy : enemies_)
	{
		if (ifF(enemy))
		{
			count++;
			//条件満たしたら
			if (ifF2(count) || (combEnemies && count >= 1))
			{
				settingFlag = true;
				break;
			}
		}
	}
}

void EnemyManager::AddCombinedEnemies(std::unique_ptr<CombinedEnemies> combEnemies, bool isCombined, bool isDockCombined
	, std::function<bool(std::vector<std::unique_ptr<Enemy>>::iterator)> addIfF, bool isPushBackArray)
{
	if (isCombined)
	{
		//単体と単体が合体した場合は新たにインスタンス
		if (combEnemies == nullptr)
		{
			combEnemies = std::make_unique<CombinedEnemies>();
		}

		for (auto itr = enemies_.begin(); itr != enemies_.end(); itr++)
		{
			if (addIfF(itr))
			{
				//今はtrue（仮
				if (true)
				{
					itr->get()->SetIsMowDown(false);
				}
				//くっついた敵の配列に追加する
				combEnemies->AddEnemy(std::move(*itr));
				//元の配列から削除
				enemies_.erase(itr);

				//エラー回避
				if (enemies_.size() > 0)
				{
					itr = enemies_.begin();
				}
				else
				{
					break;
				}
			}
		}

		//新しくインスタンスを作成した場合
		if (isDockCombined == false)
		{
			//初期化
			combEnemies->Initialize(player_, stage_, player_->GetDirectionVec());
		}
		combEnemies->AllEnemiesDockingEnd();
	}
	//薙ぎ払われる最中の敵を保存しておくインスタンスでなければ
	if (combEnemies && isPushBackArray)
	{
		//くっついた敵の塊を管理する配列に追加
		combinedEnemiesArray_.push_back(std::move(combEnemies));
	}
}

//----------------------------------------------------------------------------------
void EnemyManager::Update()
{
	//敵の生成処理
	LoadEnemiesDataCSVUpdate(GameVelocityManager::GetInstance().GetVelocity());

	for (auto itr = enemies_.begin(); itr != enemies_.end(); itr++)
	{
		itr->get()->Update();

		if (!itr->get()->GetIsAlive())
		{
			enemies_.erase(itr);

			if (enemies_.size())
			{
				itr = enemies_.begin();
			}
			else
			{
				break;
			}
		}
	}

	for (auto itr = combinedEnemiesArray_.begin(); itr != combinedEnemiesArray_.end(); itr++)
	{
		itr->get()->Update();

		if (!itr->get()->GetIsAlive())
		{
			//倒された敵の数用
			for (uint32_t i = 0; i < itr->get()->GetEnemies().size(); i++)
			{
				if (itr->get()->GetEnemies()[i]->GetIsBigDango())
				{
					defeatedEnemiesNum_ += CombinedEnemies::TO_BIG_NUM_ * 2 - 1;
				}
				else
				{
					defeatedEnemiesNum_++;
				}
			}
			isDefeatedEnemies_ = true;
			//スコア加算
			Score::GetInstance()->Add(defeatedEnemiesNum_);
			//撃破音
			Sound::GetInstance().PlayWave("brake_SE.wav", 0.2f);

			combinedEnemiesArray_.erase(itr);

			if (combinedEnemiesArray_.size())
			{
				itr = combinedEnemiesArray_.begin();
			}
			else
			{
				break;
			}
		}
	}
	defeatedEnemiesNum_ = 0;

	//薙ぎ払われてる最中の保存しておく更新
	SaveMowDownEnemies();
	//くっつく敵がいるかの更新処理
	CombinedUpdate();
	//突進されてる敵とくっついたか
	SkewerCombinedUpdate();
	//薙ぎ払われてる最中の更新2
	MowDownTriggerEnemiesUpdate();

	//薙ぎ払われた瞬間を終わらせる
	MowDownTriggerEnd();
}

void EnemyManager::Draw()
{
	for (auto& enemy : enemies_)
	{
		enemy->Draw();
	}

	for (auto& combinedEnemies : combinedEnemiesArray_)
	{
		combinedEnemies->Draw();
	}

	if (mowDownedEnemies_)
	{
		mowDownedEnemies_->Draw();
	}
}


//-------------------------------------------------------------
void EnemyManager::LoadEnemiesDataCSV(const std::string& name)
{
	std::string fullPath = ENEMIES_DATA_PATH_ + name;
	popFileName_ = name;
	///ファイルを開く
	std::ifstream file;
	file.open(fullPath);
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands_ << file.rdbuf();

	file.close();
}

void EnemyManager::LoadEnemiesDataCSVUpdate(float speed)
{
	//待機処理
	if (isWaitingPop_)
	{
		popWaitTimer_ -= speed;

		if (popWaitTimer_ <= 0)
		{
			isWaitingPop_ = false;
		}
		return;
	}

	//一行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (std::getline(enemyPopCommands_, line))
	{
		//１行分の文字列をストリームに変換
		std::istringstream lineStream(line);

		std::string word;

		//,区切りで行の先頭文字列を取得
		std::getline(lineStream, word, ' ');

		// "//"から始まる行はコメント
		if (word.find("//") == 0)
		{
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0)
		{
			//x座標
			std::getline(lineStream, word, ' ');
			float x = (float)std::atof(word.c_str());

			//y座標
			std::getline(lineStream, word, ' ');
			float y = (float)std::atof(word.c_str());

			//敵の数
			std::getline(lineStream, word, ' ');
			uint32_t num = std::atoi(word.c_str());

			//敵追加
			AddEnemy({ x,y }, num);
		}

		//WAITコマンド
		if (word.find("WAIT") == 0)
		{
			std::getline(lineStream, word, ' ');

			//待ち時間
			int32_t waitTime = std::atoi(word.c_str());

			//待機開始
			popWaitTimer_ = (float)waitTime;
			isWaitingPop_ = true;

			//ループ抜ける
			break;
		}

		//LOOPコマンド
		if (word.find("LOOP") == 0)
		{
			enemyPopCommands_.clear();
			LoadEnemiesDataCSV(popFileName_);
		}
	}
}

//-------------------------------------------------------------
void EnemyManager::AddEnemy(std::unique_ptr<Enemy> enemy)
{
	std::unique_ptr<CombinedEnemies>cEs = std::make_unique<CombinedEnemies>();
	cEs->AddEnemy(std::move(enemy));
	cEs->Initialize(player_, stage_, player_->GetDirectionVec());

	combinedEnemiesArray_.push_back(std::move(cEs));
}

void EnemyManager::AddEnemy(const Vec2& pos, uint32_t combinedNum)
{
	std::unique_ptr<CombinedEnemies>cEs = std::make_unique<CombinedEnemies>();

	for (uint32_t i = 0; i < combinedNum; i++)
	{
		std::unique_ptr<Enemy>enemy = std::make_unique<Enemy>(CollisionManger::GetInstance(), player_,
			stage_, enemyTexHandle_, BigEnemyTexHandle_);
		enemy->SetPos(pos);
		Vec2 dir = player_->GetPos() - pos;
		enemy->SetRot(atan2f(dir.y, dir.x));
		enemy->SetRad({ Enemy::KRadius_,0 });
		cEs->AddEnemy(std::move(enemy));
	}

	cEs->Initialize(player_, stage_, player_->GetDirectionVec());
	combinedEnemiesArray_.push_back(std::move(cEs));
}

bool EnemyManager::GetDefeatedEnemiesNum(uint32_t& enemiesNum)
{
	if (isDefeatedEnemies_)
	{
		enemiesNum = defeatedEnemiesNum_;
		isDefeatedEnemies_ = false;
		defeatedEnemiesNum_ = 0;

		return true;
	}

	return false;
}

uint32_t EnemyManager::GetSkewerEnemiesNum()
{
	uint32_t count = 0;

	for (auto& enemies : combinedEnemiesArray_)
	{
		if (enemies->GetIsSkewer())
		{
			count += enemies->GetEnemiesNum();
		}
	}

	return count;
}

float EnemyManager::GetSkewerEnemiesLength()
{
	for (auto& enemies : combinedEnemiesArray_)
	{
		if (enemies->GetIsSkewer())
		{
			return enemies->GetLength();
		}
	}

	return 0.0f;
}
