#pragma once
#include"Enemy.h"
#include <memory>
#include <vector>


class ICombinedEnemiesState;

//���������G����
class CombinedEnemies
{
private:
	//�ꏏ�ɂ������Ă�G�̔z��
	std::vector<std::unique_ptr<Enemy>>enemies_;
	//���̂��Ă�G�̒��S���W
	Vec2 centorPos_ = { 0,0 };
	//�G�S�̂̌�������
	Vec2 direction_ = { 0,0 };
	//�G�̐�
	uint32_t enemiesNum_ = 0;
	//�����X�s�[�h�̊�
	const float MOVE_SPEED_TMP_ = Enemy::kMoveSpeed_;
	//�S�̂̒���
	float length_ = 0.0f;
	//�v���C���[�܂ł̋���
	float distance_ = 0;
	const int32_t MOVE_TIMES_ = Enemy::kMoveTimes_; // ����ɕ����Ĉړ����邩(�u�Ԉړ��Ɍ�����̂�����邽�߁j
	const int32_t MOVE_INTERVAL_ = Enemy::kMoveInterval_; // �G�������܂ł̊Ԋu(�t���[���P��)
	//�v���C���[�̃|�C���^�[
	Player* player_ = nullptr;
	//�X�e�[�W�̃|�C���^
	Stage* stage_ = nullptr;
	int32_t frameCountWait_; // �ҋ@�p�t���[���J�E���g
	int32_t frameCountMove_; // �ړ��p�t���[���J�E���g
	//�X�e�[�g
	std::unique_ptr<ICombinedEnemiesState>state_ = nullptr;
	//
	float radiusTmp_;
	Vec2 centorPosTmp_;
	Vec2 directionTmp_;
	//�ガ��������
	Vec2 mowDownVec_ = { 0,0 };
	//�ガ�����Ă邩
	bool isMowDown_ = false;
	//���h���ɂ���Ă��邩
	bool isSkewer_ = false;
	//�����Ă��邩
	bool isAlive_ = true;
	//
	const float BIG_DANGO_EXTEND_ = Enemy::KRadius_ * 2.0f * 7.0f;
	const float BIG_SCALE_EXTEND_ = 1.0f / 32.0f;

public:
	//���̂Ńf�J�c�q�ɂȂ邩
	static const uint32_t TO_BIG_NUM_ = 8;


public:
	~CombinedEnemies();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="player_">player�|�C���^</param>
	/// <param name="direction">�v���C���[���猩���G�̕���</param>
	/// <param name="enemies">�������G�̔z��</param>
	void Initialize(Player* player, Stage* stage, const Vec2& direction);
	void Update();
	void Draw();

public:
	void AddEnemy(std::unique_ptr<Enemy>enemy);
	void AddCombinedEnemies(std::unique_ptr<CombinedEnemies>combinedEneies);
	//���������̍X�V
	void DirectionUpdate();
	//�G��̈�̂̈ʒu�X�V
	void EnemiesPosUpdate();
	//�X�P�[�������ɖ߂�
	void EnemiesScaleReset();
	//�ガ����
	void MowDown();
	//
	void CalcCentorPos(const Vec2& targetPos, const Vec2& direction, float length = 1.0f);

private:
	//�G���X�e�[�W�O�ɏo���Ȃ��悤��
	bool SetInStagePos(const Vec2& pos, Vec2& pushBackVec);
	//
	bool SetInStageEnemiesPos();

private:
	//�p�x�̍X�V
	void EnemiesRotUpdate();
	//�X�P�[�����Z�b�g
	void SetEnemiesScale(const Vec2& scale);
	//�������Ă�G�̂ǂꂩ��ł��ガ����ꂽ��
	void AnyEnemyMowDownUpdate();
	//�������Ă�G�̂ǂꂩ��ł��ːi���ꂽ��
	void AnyEnemySkewerUpdate();
	//�ːi����鏈��
	void Skewer();
	//�ガ����ꂽ�u�ԃt���O�̍X�V
	void EnemiesMowDownTriggerUpdate();
	//�f�J�c�q�̍X�V
	void BigDangoUpdate();

public:
	const Vec2& GetCentorPos() { return centorPos_; }
	void SetCentorPos(const Vec2& centorPos) { centorPos_ = centorPos; }
	const Vec2& GetDirection() { return direction_; }
	void SetDirection(const Vec2& direction) { direction_ = direction.GetNormalize(); }
	float GetLength() { return length_; }
	void SetLength(float radius) {
		length_ = radius;
	}

	void SetLengthTmp() {
		radiusTmp_ = length_;
	}
	void SetCentorPosTmp() { centorPosTmp_ = centorPos_; }
	void SetDirectionTmp() { directionTmp_ = direction_; }
	void SetIsMowDown(bool isMowDown) { isMowDown_ = isMowDown; }
	void SetIsSkewer(bool isSkewer) { isSkewer_ = isSkewer; }
	void SetIsSkewer(const Vec2& mowDownVec) { mowDownVec_ = mowDownVec; }

	float GetLengthTmp() { return radiusTmp_; }
	const Vec2& GetCentorPosTmp() { return centorPosTmp_; }
	const Vec2& GetDirectionTmp() { return directionTmp_; }

	int32_t GetEnemiesNum() { return enemiesNum_; }

	const Vec2& GetMowDownVec() { return mowDownVec_; }
	bool GetIsMowDown() { return isMowDown_; }
	bool GetIsSkewer() { return isSkewer_; }
	bool GetIsAlive() { return isAlive_; }
	bool GetIsBigDango();

	const Vec2& GetTargetPos() { return player_->GetPos(); }

	//�z��̎Q��
	std::vector<std::unique_ptr<Enemy>>& GetEnemies() { return enemies_; }

public:
	//�G�S�̂̒��łق��̓G�ƍ��̂������̂����邩
	bool GetIsDockingAnyEnemy();
	//�ガ�������ꂽ�u�Ԃ����邩
	bool GetIsMowDownTriggerAnyEnemy();
	//�S�Ă̓G���ガ����ꂽ�u�Ԃł͂Ȃ����ǂ���
	bool GetIsMowDownTriggerEnd();
	//�G�S�̂̒��łق��̋��h���ɂ���Ă�G�ƍ��̂������̂����邩
	bool GetIsDockingAndSkewer();
	//�ق��̓G�ƍ��̂����Ƃ���isDocking��false
	void AllEnemiesDockingEnd();
	//�ړ����I���A�S���̓G�̓ガ�����t���O���I�t
	void AllEnemiesEndMowDown();
	//�ガ�����I���
	void MowDownEnd();
	//�ガ����ꂽ�u�ԏI���
	void MowDownTriggerEnd();
	//�ːi����Ă�Ƃ��̍X�V
	void SkewerUpdate();
	//���S����
	void Dead();
	//�X�P�[���h�炷
	void SetScaleSinRot(float minS, float maxS, float rate, int32_t timer);
	//�X�P�[���Z�b�g
	void SetScale(const Vec2& scale);

public:
	void ChangeState(const std::string& name);
};

