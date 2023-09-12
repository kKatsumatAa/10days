#include "UI.h"

UI::UI()
{
}

UI* UI::GetInstance()
{
	static UI instance;
	return &instance;
}

void UI::AllLoad()
{
	//読み込み～
	ui_[(uint32_t)UIType::Abutton] = TextureManager::LoadGraph("Resources/texture/Abutton.png");
	ui_[(uint32_t)UIType::Bbutton] = TextureManager::LoadGraph("Resources/texture/Bbutton.png");
	ui_[(uint32_t)UIType::Rbutton] = TextureManager::LoadGraph("Resources/texture/Rbutton.png");
	ui_[(uint32_t)UIType::Lstick] = TextureManager::LoadGraph("Resources/texture/Lstick.png");
	ui_[(uint32_t)UIType::Move] = TextureManager::LoadGraph("Resources/texture/move.png");
	ui_[(uint32_t)UIType::Attack] = TextureManager::LoadGraph("Resources/texture/attack.png");
	ui_[(uint32_t)UIType::Skewer] = TextureManager::LoadGraph("Resources/texture/skewer.png");
	ui_[(uint32_t)UIType::Pause] = TextureManager::LoadGraph("Resources/texture/pause.png");
	ui_[(uint32_t)UIType::Retry] = TextureManager::LoadGraph("Resources/texture/retry.png");
	ui_[(uint32_t)UIType::ToTitle] = TextureManager::LoadGraph("Resources/texture/toTitle.png");
	ui_[(uint32_t)UIType::NowScore] = TextureManager::LoadGraph("Resources/texture/score.png");
	ui_[(uint32_t)UIType::HighScore] = TextureManager::LoadGraph("Resources/texture/highScore.png");

	ui_[(uint32_t)UIType::Tutorial1] = TextureManager::LoadGraph("Resources/texture/tutorial_UI_01.png");
	ui_[(uint32_t)UIType::Tutorial2] = TextureManager::LoadGraph("Resources/texture/tutorial_UI_02.png");
	ui_[(uint32_t)UIType::Tutorial3] = TextureManager::LoadGraph("Resources/texture/tutorial_UI_03.png");
	ui_[(uint32_t)UIType::Tutorial4] = TextureManager::LoadGraph("Resources/texture/tutorial_UI_04.png");
	ui_[(uint32_t)UIType::Tutorial5] = TextureManager::LoadGraph("Resources/texture/tutorial_UI_05.png");
	ui_[(uint32_t)UIType::Tutorial6] = TextureManager::LoadGraph("Resources/texture/tutorial_UI_06.png");
	ui_[(uint32_t)UIType::Tutorial7] = TextureManager::LoadGraph("Resources/texture/tutorial_UI_07.png");

	//アンカーポイント初期化
	for (uint32_t i = 0; i < (uint32_t)UIType::Max; i++)
	{
		ancorPoint_[i] = { 0.f,0.f };	//左上
	}
}

void UI::Init()
{
}

void UI::Update()
{
}

void UI::Draw(UIType uiType)
{
	//指定されたUIを描画
	objUI_[(uint32_t)uiType].DrawBoxSprite(
		nullptr, ui_[(uint32_t)uiType],
		{ 1.0f,1.0f,1.0f,1.0f },
		ancorPoint_[(uint32_t)uiType]);
}

void UI::SetPos(UIType uiType, const Vec2& pos)
{
	objUI_[(uint32_t)uiType].SetTrans({ pos.x,pos.y,0.f });
}

void UI::SetSize(UIType uiType, float size)
{
	objUI_[(uint32_t)uiType].SetScale({ size,size,1.0f });
}

void UI::SetAncorPoint(UIType uiType, const Vec2& ancorPoint)
{
	ancorPoint_[(uint32_t)uiType] = ancorPoint;
}
