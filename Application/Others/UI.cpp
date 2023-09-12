#include "UI.h"
#include "CameraManager.h"

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
	//ƒƒ‚ƒŠŠm•Û+‰Šú‰»
	for (uint32_t i = 0; i < (uint32_t)UIType::Max; i++)
	{
		ui_.emplace_back();
		ancorPoint_.emplace_back();
		ancorPoint_[i] = { 0.f,0.f };	//¶ã
	}

	//“Ç‚İ‚İ`
	ui_[(uint32_t)UIType::Abutton] = TextureManager::LoadGraph("Abutton.png");
	ui_[(uint32_t)UIType::Bbutton] = TextureManager::LoadGraph("Bbutton.png");
	ui_[(uint32_t)UIType::Rbutton] = TextureManager::LoadGraph("Rbutton.png");
	ui_[(uint32_t)UIType::Lstick] = TextureManager::LoadGraph("Lstick.png");
	ui_[(uint32_t)UIType::Move] = TextureManager::LoadGraph("move.png");
	ui_[(uint32_t)UIType::Attack] = TextureManager::LoadGraph("attack.png");
	ui_[(uint32_t)UIType::Skewer] = TextureManager::LoadGraph("skewer.png");
	ui_[(uint32_t)UIType::Pause] = TextureManager::LoadGraph("pause.png");
	ui_[(uint32_t)UIType::Retry] = TextureManager::LoadGraph("retry.png");
	ui_[(uint32_t)UIType::ToTitle] = TextureManager::LoadGraph("toTitle.png");
	ui_[(uint32_t)UIType::NowScore] = TextureManager::LoadGraph("score.png");
	ui_[(uint32_t)UIType::HighScore] = TextureManager::LoadGraph("highScore.png");

	ui_[(uint32_t)UIType::Tutorial1] = TextureManager::LoadGraph("tutorial_UI_01.png");
	ui_[(uint32_t)UIType::Tutorial2] = TextureManager::LoadGraph("tutorial_UI_02.png");
	ui_[(uint32_t)UIType::Tutorial3] = TextureManager::LoadGraph("tutorial_UI_03.png");
	ui_[(uint32_t)UIType::Tutorial4] = TextureManager::LoadGraph("tutorial_UI_04.png");
	ui_[(uint32_t)UIType::Tutorial5] = TextureManager::LoadGraph("tutorial_UI_05.png");
	ui_[(uint32_t)UIType::Tutorial6] = TextureManager::LoadGraph("tutorial_UI_06.png");
	ui_[(uint32_t)UIType::Tutorial7] = TextureManager::LoadGraph("tutorial_UI_07.png");
}

void UI::Init()
{
}

void UI::Update()
{
}

void UI::Draw(UIType uiType)
{
	//w’è‚³‚ê‚½UI‚ğ•`‰æ
	objUI_[(uint32_t)uiType].DrawBoxSprite(
		CameraManager::GetInstance().GetCamera2D("UICamera"),
		ui_[(uint32_t)uiType],
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
