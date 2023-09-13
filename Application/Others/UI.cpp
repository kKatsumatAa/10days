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
	//メモリ確保+初期化
	for (uint32_t i = 0; i < (uint32_t)UIType::Max; i++)
	{
		ui_.emplace_back();
		ancorPoint_.emplace_back();
		ancorPoint_[i] = { 0.f,0.f };	//左上
		color_.emplace_back();
		color_[i] = { 1.f, 1.f, 1.f, 1.f };	//全部の色使う
	}

	//読み込み～
	ui_[(uint32_t)UIType::Abutton] = TextureManager::LoadGraph("Abutton.png");
	ui_[(uint32_t)UIType::Bbutton] = TextureManager::LoadGraph("Bbutton.png");
	ui_[(uint32_t)UIType::Rbutton] = TextureManager::LoadGraph("Rbutton.png");
	ui_[(uint32_t)UIType::Menubutton] = TextureManager::LoadGraph("Menubutton.png");
	ui_[(uint32_t)UIType::Lstick] = TextureManager::LoadGraph("Lstick.png");
	ui_[(uint32_t)UIType::Move] = TextureManager::LoadGraph("move.png");
	ui_[(uint32_t)UIType::Attack] = TextureManager::LoadGraph("attack.png");
	ui_[(uint32_t)UIType::Skewer] = TextureManager::LoadGraph("skewer.png");
	ui_[(uint32_t)UIType::Pause] = TextureManager::LoadGraph("pause.png");
	ui_[(uint32_t)UIType::Retry] = TextureManager::LoadGraph("retry.png");
	ui_[(uint32_t)UIType::ToTitle] = TextureManager::LoadGraph("toTitle.png");
	ui_[(uint32_t)UIType::Time] = TextureManager::LoadGraph("time.png");
	ui_[(uint32_t)UIType::NowScore] = TextureManager::LoadGraph("score.png");
	ui_[(uint32_t)UIType::HighScore] = TextureManager::LoadGraph("highScore.png");
	ui_[(uint32_t)UIType::Makimono] = TextureManager::LoadGraph("makimono.png");
	ui_[(uint32_t)UIType::ReadyGo] = TextureManager::LoadGraph("start.png");
	ui_[(uint32_t)UIType::Finish] = TextureManager::LoadGraph("finish.png");
	ui_[(uint32_t)UIType::NewRecord] = TextureManager::LoadGraph("new_record.png");


	ui_[(uint32_t)UIType::Tutorial1] = TextureManager::LoadGraph("tutorial_UI_01.png");
	ui_[(uint32_t)UIType::Tutorial2] = TextureManager::LoadGraph("tutorial_UI_02.png");
	ui_[(uint32_t)UIType::Tutorial3] = TextureManager::LoadGraph("tutorial_UI_03.png");
	ui_[(uint32_t)UIType::Tutorial4] = TextureManager::LoadGraph("tutorial_UI_04.png");
	ui_[(uint32_t)UIType::Tutorial5] = TextureManager::LoadGraph("tutorial_UI_05.png");
	ui_[(uint32_t)UIType::Tutorial6] = TextureManager::LoadGraph("tutorial_UI_06.png");
	ui_[(uint32_t)UIType::Tutorial7] = TextureManager::LoadGraph("tutorial_UI_07.png");


	ui_[(uint32_t)UIType::Number0] = TextureManager::LoadGraph("number0.png");
	ui_[(uint32_t)UIType::Number1] = TextureManager::LoadGraph("number1.png");
	ui_[(uint32_t)UIType::Number2] = TextureManager::LoadGraph("number2.png");
	ui_[(uint32_t)UIType::Number3] = TextureManager::LoadGraph("number3.png");
	ui_[(uint32_t)UIType::Number4] = TextureManager::LoadGraph("number4.png");
	ui_[(uint32_t)UIType::Number5] = TextureManager::LoadGraph("number5.png");
	ui_[(uint32_t)UIType::Number6] = TextureManager::LoadGraph("number6.png");
	ui_[(uint32_t)UIType::Number7] = TextureManager::LoadGraph("number7.png");
	ui_[(uint32_t)UIType::Number8] = TextureManager::LoadGraph("number8.png");
	ui_[(uint32_t)UIType::Number9] = TextureManager::LoadGraph("number9.png");
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
		CameraManager::GetInstance().GetCamera2D("UICamera"),
		ui_[(uint32_t)uiType],
		color_[(uint32_t)uiType],
		ancorPoint_[(uint32_t)uiType]);
}

void UI::Draw2(UIType uiType, float rot)
{
    objUI_[(uint32_t)uiType].SetRot({0,0,rot});
    objUI_[(uint32_t)uiType].DrawBoxSprite(
        CameraManager::GetInstance().GetCamera2D("UICamera"),
        ui_[(uint32_t)uiType],
        color_[(uint32_t)uiType],
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

void UI::SetColor(UIType uiType, const Vec4& color)
{
	color_[(uint32_t)uiType] = color;
}
