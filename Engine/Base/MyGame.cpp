#include "MyGame.h"
#include"SceneFactory.h"


void MyGame::Initialize()
{
	//基底クラスの初期化処理
	Framework::Initialize();

	//ゲーム固有の初期化
	CameraManager::GetInstance().Initialize();

	//シーンファクトリーを生成し、マネージャーにセット
	sceneFactory_ = std::make_unique<SceneFactory>();
	sceneM_->SetSceneFactory(sceneFactory_.get());
	sceneM_->Initialize();
	sceneM_->SetNextScene(SceneFactory::Usage::TITLE);
}

void MyGame::Finalize()
{
	//ゲーム固有の終了処理


	//基底クラスの終了処理
	Framework::Finalize();
}

void MyGame::Update()
{
	//基底クラスの更新処理
	Framework::Update();

	//ゲーム固有の更新処理
}

void MyGame::Draw()
{
	//ポストペラに描画する
	{
		std::function<void()>f = [=]() {sceneM_->Draw(); };
		std::function<void()>spriteF = [=]() {sceneM_->DrawSprite(); };
		std::function<void()>spriteF2 = [=]() {sceneM_->DrawSprite2(); };
		PostEffectManager::GetInstance().BeforeDraw(f, spriteF, spriteF2);
	}

	//実際に描画
	{
		//実際に描画----------------
		DirectXWrapper::GetInstance().PreDraw();

		//ポストペラ実際に描画
		PostEffectManager::GetInstance().DrawDisplay();


#ifdef _DEBUG
		{
			//imgui
			imguiM_->Begin();


			//(imgui)
			sceneM_->DrawImgui();
			//ポストエフェクト
			PostEffectManager::GetInstance().ImGuiUpdate();


			imguiM_->End();

			imguiM_->Draw();
		}
#endif // DEBUG

		// 4.描画コマンドここまで //
		DirectXWrapper::GetInstance().PostDraw();
	}
}
