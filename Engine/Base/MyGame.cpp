#include "MyGame.h"
#include"SceneFactory.h"


void MyGame::Initialize()
{
	//���N���X�̏���������
	Framework::Initialize();

	//�Q�[���ŗL�̏�����
	CameraManager::GetInstance().Initialize();

	//�V�[���t�@�N�g���[�𐶐����A�}�l�[�W���[�ɃZ�b�g
	sceneFactory_ = std::make_unique<SceneFactory>();
	sceneM_->SetSceneFactory(sceneFactory_.get());
	sceneM_->Initialize();
	sceneM_->SetNextScene(SceneFactory::Usage::TITLE);
}

void MyGame::Finalize()
{
	//�Q�[���ŗL�̏I������


	//���N���X�̏I������
	Framework::Finalize();
}

void MyGame::Update()
{
	//���N���X�̍X�V����
	Framework::Update();

	//�Q�[���ŗL�̍X�V����
}

void MyGame::Draw()
{
	//�|�X�g�y���ɕ`�悷��
	{
		std::function<void()>f = [=]() {sceneM_->Draw(); };
		std::function<void()>spriteF = [=]() {sceneM_->DrawSprite(); };
		PostEffectManager::GetInstance().BeforeDraw(f, spriteF);
	}

	//���ۂɕ`��
	{
		//���ۂɕ`��----------------
		DirectXWrapper::GetInstance().PreDraw();

		//�|�X�g�y�����ۂɕ`��
		PostEffectManager::GetInstance().DrawDisplay();


#ifdef _DEBUG
		{
			//imgui
			imguiM_->Begin();


			//(imgui)
			sceneM_->DrawImgui();
			//�|�X�g�G�t�F�N�g
			PostEffectManager::GetInstance().ImGuiUpdate();


			imguiM_->End();

			imguiM_->Draw();
		}
#endif // DEBUG

		// 4.�`��R�}���h�����܂� //
		DirectXWrapper::GetInstance().PostDraw();
	}
}
