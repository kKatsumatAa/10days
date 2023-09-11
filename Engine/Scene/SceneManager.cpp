#include "SceneManager.h"
#include <sstream>
#include <iomanip>
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "FbxLoader.h"
#include "JsonLevelLoader.h"
#include "CameraManager.h"
#include "SceneTransitionManager.h"
#include "SceneTransitionEffectState.h"
#include "CircleCollider.h"


//---------------------------------------------------------------------------------------
//デストラクタ
SceneManager::~SceneManager()
{
	state_->Finalize();
}

void SceneManager::SetNextScene(SceneFactory::Usage scene)
{
	nextScene_ = sceneFactory_->CreateScene(scene);

	//遷移演出開始
	ChangeScene();
}


void SceneManager::ChangeScene()
{
	if (nextScene_)
	{
		//シーン遷移演出時に非同期でロード
		SceneTransitionManager::GetInstance().BeginSceneTransition([=]()
			{
				if (state_) {
					state_->Finalize();
					state_.reset();
				}
				//シーンファクトリーでシーン生成
				state_ = std::move(nextScene_);
				state_->SetSceneManager(this);
				state_->Initialize();
				//画像アップロード
				DirectXWrapper::GetInstance().UpLoadTexture();
			});
	}
}

void SceneManager::StopWaveAllScene()
{
}


//---------------------------------------------------------------------------------------------------
void SceneManager::Initialize()
{
	TextureManager::Initialize();

	texhandle_[1] = TextureManager::LoadGraph("effect1.png");

	//インスタンス生成
	lightManager_ = std::move(LightManager::Create());
	//ライト色を設定
	lightManager_->SetDirLightColor(0, { 0.6f,0.6f,0.6f });
	//3Dオブジェクトにライトをセット(全体で一つを共有)
	Object::SetLight(lightManager_.get());
	//レベルマネージャーにも
	LevelManager::GetInstance().SetLightManager(lightManager_.get());

	//シーン遷移マネージャ
	SceneTransitionManager::GetInstance().Initialize();

	//画像アップロード
	DirectXWrapper::GetInstance().UpLoadTexture();
}

void SceneManager::Update()
{
	//シーン遷移マネージャ
	SceneTransitionManager::GetInstance().Update();

	//シーン遷移演出終わったら
	if (!SceneTransitionManager::GetInstance().GetIsDoingEffect() && state_)
	{
		//objマネージャ
		ObjectManager::GetInstance().Update();
		state_->Update();

		//オブジェマネージャーのデバッグ用アップデート
		ObjectManager::GetInstance().DebugUpdate();

		//カメラマネージャー
		CameraManager::GetInstance().Update();

		//仮
		CollisionManager::GetInstance()->CheckAllCollisions();
	}

	lightManager_->SetAmbientColor({ ambientColor_[0],ambientColor_[1], ambientColor_[2] });
	lightManager_->SetDiffuseColor({ diffuseColor_[0],diffuseColor_[1], diffuseColor_[2] });
	lightManager_->SetSpecularColor({ specularColor_[0],specularColor_[1], specularColor_[2] });

	lightManager_->Update();
}

void SceneManager::Draw()
{
	//ロードしてなければ描画だけ
	if (!SceneTransitionManager::GetInstance().GetIsLoadingOnly() && state_)
	{
		state_->Draw();
		//objマネージャ
		ObjectManager::GetInstance().Draw();
	}
}

void SceneManager::DrawSprite()
{
	//ロードしてなければ描画だけ
	if (!SceneTransitionManager::GetInstance().GetIsLoadingOnly() && state_)
	{
		state_->DrawSprite();

		//// スローモーション用のフレームカウンタの値が0以外 && プレイヤーのSceneM用のフラグがtrueなら
	//if (frameCount_slowMotion_ && Player::isSkewerScreenBlack4SceneM_)
	//{
	//    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	//    DrawBox(0, 0, 1280, 720, UtilL::Color::BLACK, true);
	//    SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	//}
	//DrawFormatString(0, 100, UtilL::Color::WHITE, frameCount_slowMotion_ == 0 ? "no slow" : "slow");
	//DrawFormatString(0, 120, UtilL::Color::WHITE, "slow: %d", frameCount_slowMotion_);
	//DrawFormatString(0, 160, UtilL::Color::GREEN, "frameCount: %d", frameCount_debug_);
	}

	//シーン遷移マネージャ
	SceneTransitionManager::GetInstance().Draw();
}

void SceneManager::DrawImgui()
{
	ImGui::Begin("LightColor");

	ImGui::SliderFloat3("ambientColor", ambientColor_, 0, 1.0f);
	ImGui::SliderFloat3("diffuseColor", diffuseColor_, 0, 1.0f);
	ImGui::SliderFloat3("specularColor", specularColor_, 0, 10.0f);

	ImGui::End();

	//ロードしてなければ
	if (!SceneTransitionManager::GetInstance().GetIsLoadingOnly() && state_)
	{
		state_->DrawImgui();
		LevelManager::GetInstance().DrawImGui();
		//objマネージャ
		ObjectManager::GetInstance().DrawImGui();
		//light
		lightManager_->DrawImGui();
		//カメラ
		CameraManager::GetInstance().ImguiUpdate();
	}
}


