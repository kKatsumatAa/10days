#include "CombinedEnemies.h"
#include "CombinedEnemiesState.h"
#include "Util.h"
#include "HitStop.h"
#include "CameraManager.h"


CombinedEnemies::~CombinedEnemies()
{
	state_.reset();
}

//-------------------------------------------------
void CombinedEnemies::Initialize(Player* player, Stage* stage, const Vec2& direction)
{
	player_ = player;
	stage_ = stage;
	//
	distance_ = (player_->GetPos() - player_->GetPos() + direction.GetNormalize() * length_ * ((float)enemiesNum_ / 2.0f)).GetLength();

	//中央位置計算
	//CalcCentorPos(player_->GetPos(), direction.GetNormalize());

	//ステート
	ChangeState("AFTER_COMBINED");
}

bool CombinedEnemies::GetIsBigDango()
{
	if (enemies_.size() == 1 && enemies_[0]->GetIsBigDango())
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------------------------------
bool CombinedEnemies::GetIsDockingAnyEnemy()
{
	for (auto& enemy : enemies_)
	{
		if (enemy->GetIsDocking())
		{
			return true;
		}
	}

	return false;
}

bool CombinedEnemies::GetIsMowDownTriggerAnyEnemy()
{
	for (auto& enemy : enemies_)
	{
		if (enemy->GetIsMowDownTrigger())
		{
			return true;
		}
	}

	return false;
}

bool CombinedEnemies::GetIsMowDownTriggerEnd()
{
	for (auto& enemy : enemies_)
	{
		if (enemy->GetIsMowDownTrigger())
		{
			return false;
		}
	}

	return true;
}

bool CombinedEnemies::GetIsDockingAndSkewer()
{
	if (GetIsDockingAnyEnemy() && isSkewer_)
	{
		return true;
	}

	return false;
}

void CombinedEnemies::AllEnemiesDockingEnd()
{
	for (auto& enemy : enemies_)
	{
		enemy->SetIsDocking(false);
	}
}

void CombinedEnemies::AllEnemiesEndMowDown()
{
	for (auto& enemy : enemies_)
	{
		enemy->SetIsMowDown(false);
		enemy->SetIsMowDownTrigger(false);
	}
}

void CombinedEnemies::MowDownEnd()
{
	isMowDown_ = false;
	AllEnemiesEndMowDown();
}

void CombinedEnemies::MowDownTriggerEnd()
{
	for (auto& enemy : enemies_)
	{
		enemy->SetIsMowDownTrigger(false);
	}
}

void CombinedEnemies::ChangeState(const std::string& name)
{
	std::string nameL = name;

	state_.reset();
	state_ = ICombinedEnemiesState::GetState(nameL);
	state_->SetCombinedEnemies(this);
	state_->Initialize();
}

//-----------------------------------------------------------------------------------------------------
void CombinedEnemies::CalcCentorPos(const Vec2& targetPos, const Vec2& direction, float length)
{
	//仮
	centorPos_ = targetPos + direction * length;
	if (enemies_.size() == 1)
	{
		enemies_[0]->SetPos(centorPos_);
	}
}

bool CombinedEnemies::SetInStagePos(const Vec2& pos, Vec2& pushBackVec)
{
	Vec2 pushBackVecL = { 0,0 };
	bool isPushBack = false;

	//x
	if (pos.x < stage_->GetLT().x)
	{
		pushBackVecL.x += stage_->GetLT().x - pos.x;
		isPushBack = true;
	}
	if (pos.x > stage_->GetRB().x)
	{
		pushBackVecL.x += stage_->GetRB().x - pos.x;
		isPushBack = true;
	}
	//y
	if (pos.y > stage_->GetRB().y)
	{
		pushBackVecL.y += stage_->GetRB().y - pos.y;
		isPushBack = true;
	}
	if (pos.y < stage_->GetLT().y)
	{
		pushBackVecL.y += stage_->GetLT().y - pos.y;
		isPushBack = true;
	}

	//押し戻す
	pushBackVec += pushBackVecL;

	return isPushBack;
}

bool CombinedEnemies::SetInStageEnemiesPos()
{
	bool isSet = false;
	Vec2 pushBackVec = { 0,0 };

	//先頭と最後の敵を見て押し戻し
	if (enemies_.size() >= 2)
	{
		isSet = SetInStagePos(enemies_[0]->GetPos(), pushBackVec);
		isSet = SetInStagePos(enemies_[enemies_.size() - 1]->GetPos(), pushBackVec);
	}
	//敵が一体のみだったら
	else if (enemies_.size())
	{
		isSet = SetInStagePos(enemies_[0]->GetPos(), pushBackVec);
	}

	centorPos_ += pushBackVec.GetNormalize() * (pushBackVec.GetLength() + Enemy::KRadius_);

	if (isSet)
	{
		ChangeState("WAIT");
	}

	return isSet;
}

void CombinedEnemies::AnyEnemyMowDownUpdate()
{
	if (isMowDown_ || isSkewer_)
	{
		return;
	}

	for (auto& enemy : enemies_)
	{
		//一体でも薙ぎ払われてたら
		if (enemy->GetIsMowDown())
		{
			//薙ぎ払う処理
			MowDown();
			break;
		}
	}
}

void CombinedEnemies::MowDown()
{
	for (auto& enemy : enemies_)
	{
		//敵単体の薙ぎ払いフラグ
		enemy->SetIsMowDown(true);
		mowDownVec_ = enemy->GetMowDownVec();
	}
	//敵全体の薙ぎ払いフラグ
	isMowDown_ = true;
	//ステート変更
	ChangeState("MOW_DOWN");
}

void CombinedEnemies::AnyEnemySkewerUpdate()
{
	bool isSkewer = false;

	//どれか一つでも突進されてるか
	for (auto& enemy : enemies_)
	{
		if (enemy->GetIsSkewer())
		{
			isSkewer = true;
			break;
		}
	}

	if (!isSkewer) { return; }

	//突進される処理
	Skewer();
}

void CombinedEnemies::Skewer()
{
	//どれか一つでも突進されてたら全部フラグ立てる
	for (auto& enemy : enemies_)
	{
		enemy->SetIsSkewer(true);
	}

	isSkewer_ = true;

	ChangeState("SKEWER");
}

void CombinedEnemies::SkewerUpdate()
{
	DirectionUpdate();
	//串の下の部分が端の敵になるように
	float enemyRadius = length_ / (float)enemiesNum_ / 2.0f;
	centorPos_ = player_->GetPos4SwordBottom() + (player_->GetMoveVec() * (length_ / 2.0f + enemyRadius));

	if (!player_->GetIsSkewer() && isSkewer_ && !HitStopManager::GetInstance().GetIsStop())
	{
		Dead();
	}
}


void CombinedEnemies::EnemiesMowDownTriggerUpdate()
{
	for (auto& enemy : enemies_)
	{
		enemy->TriggerFlagUpdate();
	}
}

void CombinedEnemies::BigDangoUpdate()
{
	if (GetIsSkewer())
	{
		return;
	}

	if (enemiesNum_ >= TO_BIG_NUM_)
	{
		for (auto itr = enemies_.begin(); itr != enemies_.end(); itr++)
		{
			if (enemies_.size() == 1)
			{
				break;
			}

			enemies_.erase(itr);
			itr = enemies_.begin();
		}

		enemiesNum_ = 1;

		enemies_[0]->SetIsBigDango(true);
		enemies_[0]->SetRad({ BIG_DANGO_EXTEND_ / 2.0f,0 });
		enemies_[0]->SetScaleExtend(BIG_DANGO_EXTEND_ * BIG_SCALE_EXTEND_);
		length_ = BIG_DANGO_EXTEND_;
	}
}

//-------------------------------------------------
void CombinedEnemies::Dead()
{
	for (auto& enemy : enemies_)
	{
		enemy->Dead();
	}

	isAlive_ = false;
}

//--------------------------------------------------
void CombinedEnemies::EnemiesPosUpdate()
{
	bool isSetCentorPos = false;

	//敵が一体の場合は敵の座標を使う（押し戻しなどがあるので）
	if (enemiesNum_ == 1 && !GetIsMowDownTriggerAnyEnemy())
	{
		centorPos_ = enemies_[0]->GetPos();
		return;
	}

	//中央のインデックス
	float centorIndex = (float)enemiesNum_ / 2.0f - 0.5f;

	uint32_t count = 0;
	//対応した位置に配置
	for (auto& enemy : enemies_)
	{
		enemy->SetPos(centorPos_ + direction_ * (centorIndex - (float)count) * (length_ / (float)enemiesNum_));

		count++;
	}
}

void CombinedEnemies::EnemiesScaleReset()
{
	for (auto& enemy : enemies_)
	{
		enemy->SetScale({ 1.0f,1.0f });
	}
}

void CombinedEnemies::EnemiesRotUpdate()
{
	float rot = atan2f(direction_.y, direction_.x);

	for (auto& enemy : enemies_)
	{
		enemy->SetRot(rot + PI / 2.0f);
	}
}

void CombinedEnemies::SetEnemiesScale(const Vec2& scale)
{
	for (auto& enemy : enemies_)
	{
		enemy->SetScale(scale);
	}
}

void CombinedEnemies::SetScaleSinRot(float minS, float maxS, float rate, int32_t timer)
{
	int32_t count = 0;
	for (auto& enemy : enemies_)
	{
		float rot = atan2f(direction_.y, direction_.x);

		Vec2 scale = { -sinf(rot) * sinf((float)timer * rate + (float)count),
			cosf(rot) * sinf((float)timer * rate + (float)count) };

		scale += {GetRand(minS, maxS),
			GetRand(minS, maxS)};

		scale.x = min(max(scale.x, minS), maxS);
		scale.y = min(max(scale.y, minS), maxS);

		enemy->SetScale(scale);
		count++;
	}
}

void CombinedEnemies::SetScale(const Vec2& scale)
{
	for (auto& enemy : enemies_)
	{
		Vec2 scaleL = scale;

		scaleL += {GetRand(-scale.x * 0.1f, scale.x * 0.1f),
			GetRand(-scale.y * 0.1f, scale.y * 0.1f)};

		enemy->SetScale(scaleL);
	}
}

//------------------------------------------------------------------------------
void CombinedEnemies::Update()
{
	//薙ぎ払いの更新
	//AnyEnemyMowDownUpdate();
	//突進の更新
	AnyEnemySkewerUpdate();

	//敵一体一体の座標更新
	EnemiesPosUpdate();

	state_->Update();
	//角度更新
	EnemiesRotUpdate();

	//薙ぎ払いフラグ更新
	EnemiesMowDownTriggerUpdate();

	BigDangoUpdate();

	//敵が一体の場合は押し戻しなどをするため
	if (enemiesNum_ == 1)
	{
		enemies_[0]->SetPos(centorPos_);
	}
}

void CombinedEnemies::Draw()
{
	for (auto& enemy : enemies_)
	{
		enemy->Draw();
	}
}

//------------------------------------------------------------
void CombinedEnemies::AddEnemy(std::unique_ptr<Enemy> enemy)
{
	enemy->SetIsDocking(false);
	enemy->SetIsMowDownTrigger(false);
	//敵の長さを加算していく
	float addRadius = Enemy::KRadius_ * 2.0f;
	length_ = radiusTmp_ + addRadius;
	radiusTmp_ = length_;
	//登録
	enemies_.push_back(std::move(enemy));
	//敵の数を加算
	enemiesNum_++;
	//合体フラグオフ
	AllEnemiesDockingEnd();
	if (enemies_.size() == 1)
	{
		centorPos_ = enemies_[0]->GetPos();
	}

	//どちらかが薙ぎ払われてたら
	/*else*/ if (GetIsMowDown())
	{
		MowDownEnd();
		ChangeState("AFTER_COMBINED");
	}
}

void CombinedEnemies::AddCombinedEnemies(std::unique_ptr<CombinedEnemies> combinedEneies)
{
	//中間地点を中心座標にする
	centorPos_ = centorPos_ + ((combinedEneies->centorPos_ - centorPos_) / 2.0f);

	for (auto& enemy : combinedEneies->enemies_)
	{
		AddEnemy(std::move(enemy));
	}

	//どちらかが薙ぎ払われてたら
	if (combinedEneies->GetIsMowDown() || GetIsMowDown())
	{
		MowDownEnd();
	}

	//敵くっついた音
	Sound::GetInstance().PlayWave("fusion_SE.wav",0.5f);

	ChangeState("AFTER_COMBINED");
}


//--------------------------------------------------------------
void CombinedEnemies::DirectionUpdate()
{
	//playerまでの距離を計算
	direction_ = (player_->GetPos() - centorPos_).GetNormalize();
}

