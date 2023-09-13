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
	//寿命が尽きたパーティクルを全削除
	for (size_t i = 0; i < particles_.size(); i++)
	{
		if (!particles_[i].GetIsAlive())
		{
			particles_[i].obj_.release();
			particles_.erase(particles_.begin() + i);
			i = (size_t)-1;
		}
	}

	//全パーティクル更新
	for (size_t i = 0; i < particles_.size(); i++)
	{
		//生存時間とイージング用タイマーの更新
		particles_[i].timer += velocity;

		//スケールの線形補間
		particles_[i].scale.x = Easing::lerp(particles_[i].startScale.x, particles_[i].endScale.x, particles_[i].GetTimeRate());
		particles_[i].scale.y = Easing::lerp(particles_[i].startScale.y, particles_[i].endScale.y, particles_[i].GetTimeRate());

		//加速度を速度に加算
		particles_[i].velo += particles_[i].accel;

		//重力加算
		if (isGravity)
		{
			particles_[i].velo.y += particles_[i].gravity;
		}

		//速度による移動
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
		//後でやろう;
	}
}

void Emitter::Add(uint32_t addNum, float life, float minScale, float maxScale, Vec2 minVelo, Vec2 maxVelo, Vec2 accel, float minRot, float maxRot, Vec4 color)
{
	for (uint32_t i = 0; i < addNum; i++)
	{
		//指定した最大数超えてたら生成しない
		if (particles_.size() >= maxCount_)
		{
			return;
		}

		//追加した要素の参照
		Particle p;
		//エミッターの中からランダムで座標を決定
		float x = Math::Function::Random<float>(-scale_.x, scale_.x);
		float y = Math::Function::Random<float>(-scale_.y, scale_.y);
		Vec2 randomPos(x, y);
		//引数の範囲から大きさランダムで決定
		float scale_ = Math::Function::Random<float>(minScale, maxScale);
		Vec2 randomScale(scale_, scale_);
		//引数の範囲から飛ばす方向ランダムで決定
		Vec2 velo = {
			Math::Function::Random<float>(minVelo.x,maxVelo.x),
			Math::Function::Random<float>(minVelo.y,maxVelo.y) };
		//引数の範囲から回転をランダムで決定
		float rot = Math::Function::Random<float>(minRot, maxRot);

		//決まった座標にエミッター自体の座標を足して正しい位置に
		p.pos = randomPos + pos_;
		//飛んでく方向に合わせて回転
		p.rot = rot;
		p.velo = velo;
		p.accel = accel;
		p.scale = randomScale;
		p.startScale = randomScale;
		p.endScale = { 0,0 };
		p.color = color;
		//イージング用のタイマーを設定、開始
		p.timerMax = life * 60.0f;
		//
		p.obj_ = std::make_unique<Object>();

		particles_.push_back(std::move(p));
	}
}

void Emitter::SetScale(Vec2& scale)
{
	scale_ = scale;
	originalScale_ = scale_;			//拡縮用に元のサイズを保管
}

void Emitter::SetScalingTimer(float timer)
{
	scalingTimer_.maxTime_ = timer;
}

void Emitter::StartScalingTimer(bool isRun)
{
	if (isRun)
	{
		scalingTimer_.Start();	//設定と同時にタイマーもスタート
	}
	else
	{
		scalingTimer_.ReverseStart();	//設定と同時にタイマーもスタート
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
