#include "Particle.h"
#include "MathUtil.h"
#include "Easing.h"


Emitter::Emitter()
{
	addInterval_ = 0;
	maxScale_ = 0;
	minScale_ = 0;
	scaling_ = 0;
}

void Emitter::Init()
{
	particles_.clear();
}

void Emitter::Update(bool isGravity, float velocity)
{
	//�������s�����p�[�e�B�N����S�폜
	for (size_t i = 0; i < particles_.size(); i++)
	{
		if (!particles_[i].GetIsAlive())
		{
			particles_[i].obj_.release();
			particles_.erase(particles_.begin() + i);
			i = (size_t)-1;
		}
	}

	//�S�p�[�e�B�N���X�V
	for (size_t i = 0; i < particles_.size(); i++)
	{
		//�������ԂƃC�[�W���O�p�^�C�}�[�̍X�V
		particles_[i].timer += velocity;

		//�X�P�[���̐��`���
		particles_[i].scale.x = Easing::lerp(particles_[i].startScale.x, particles_[i].endScale.x, particles_[i].GetTimeRate());
		particles_[i].scale.y = Easing::lerp(particles_[i].startScale.y, particles_[i].endScale.y, particles_[i].GetTimeRate());

		//�����x�𑬓x�ɉ��Z
		particles_[i].velo += particles_[i].accel;

		//�d�͉��Z
		if (isGravity)
		{
			particles_[i].velo.y += particles_[i].gravity;
		}

		//���x�ɂ��ړ�
		particles_[i].pos += particles_[i].velo;
	}
}

void Emitter::DrawCircleParticle()
{
	for (auto& p : particles_)
	{
		p.Draw();
	}
}

void Emitter::DrawGraph()
{
	for (auto& p : particles_)
	{
		//��ł�낤;
	}
}

void Emitter::Add(uint32_t addNum, float life, float minScale, float maxScale, Vec2 minVelo, Vec2 maxVelo, Vec2 accel, float minRot, float maxRot, Vec4 color)
{
	for (uint32_t i = 0; i < addNum; i++)
	{
		//�w�肵���ő吔�����Ă��琶�����Ȃ�
		if (particles_.size() >= maxCount_)
		{
			return;
		}

		//�ǉ������v�f�̎Q��
		Particle p;
		//�G�~�b�^�[�̒����烉���_���ō��W������
		float x = Math::Function::Random<float>(-scale_.x, scale_.x);
		float y = Math::Function::Random<float>(-scale_.y, scale_.y);
		Vec2 randomPos(x, y);
		//�����͈̔͂���傫�������_���Ō���
		float scale_ = Math::Function::Random<float>(minScale, maxScale);
		Vec2 randomScale(scale_, scale_);
		//�����͈̔͂����΂����������_���Ō���
		Vec2 velo = {
			Math::Function::Random<float>(minVelo.x,maxVelo.x),
			Math::Function::Random<float>(minVelo.y,maxVelo.y) };
		//�����͈̔͂����]�������_���Ō���
		float rot = Math::Function::Random<float>(minRot, maxRot);

		//���܂������W�ɃG�~�b�^�[���̂̍��W�𑫂��Đ������ʒu��
		p.pos = randomPos + pos_;
		//���ł������ɍ��킹�ĉ�]
		p.rot = rot;
		p.velo = velo;
		p.accel = accel;
		p.scale = randomScale;
		p.startScale = randomScale;
		p.endScale = { 0,0 };
		p.color = color;
		//�C�[�W���O�p�̃^�C�}�[��ݒ�A�J�n
		p.timerMax = life * 60.0f;
		//
		p.obj_ = std::make_unique<Object>();

		particles_.push_back(std::move(p));
	}
}

void Emitter::SetScale(Vec2& scale)
{
	scale_ = scale;
	originalScale_ = scale_;			//�g�k�p�Ɍ��̃T�C�Y��ۊ�
}

void Emitter::SetScalingTimer(float timer)
{
	scalingTimer_.maxTime_ = timer;
}

void Emitter::StartScalingTimer(bool isRun)
{
	if (isRun)
	{
		scalingTimer_.Start();	//�ݒ�Ɠ����Ƀ^�C�}�[���X�^�[�g
	}
	else
	{
		scalingTimer_.ReverseStart();	//�ݒ�Ɠ����Ƀ^�C�}�[���X�^�[�g
	}
}


//---------------------------------------------------------------------------------
void Emitter::Particle::Draw()
{
	obj_->SetScale({ scale.x,scale.y,0 });
	obj_->SetTrans({ pos.x,pos.y,0 });
	obj_->SetRot({ 0,0,rot });

	if (key == 0U)
	{
		obj_->DrawBoxSprite(nullptr, 0U, color, { 0.5f,0.5f });
	}
	else
	{
		obj_->DrawBoxSprite(nullptr, key, color, { 0.5f,0.5f });
	}
}
