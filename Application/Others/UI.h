#pragma once
#include <memory>
#include <vector>
#include <Vec2.h>
#include "Object.h"

enum class UIType
{
	Abutton,	//Aボタン
	Bbutton,	//Bボタン
	Rbutton,	//Rボタン
	Menubutton,	//メニューボタン
	Lstick,		//Lスティック
	Move,		//移動
	Attack,		//なぎ払い
	Skewer,		//串刺し
	Pause,		//ポーズ
	Retry,		//リトライ
	ToTitle,	//タイトルへ
	Time,		//残り時間
	NowScore,	//今回の記録
	HighScore,	//最高記録
	Makimono,	//巻物
    ReadyGo,    // 始め！
    Finish,     // そこまで!
    NewRecord,  //新記録!

    Tutorial1,
    Tutorial2,
    Tutorial3,
    Tutorial4,
    Tutorial5,
    Tutorial6,
    Tutorial7,

    Number0,
    Number1,
    Number2,
    Number3,
    Number4,
    Number5,
    Number6,
    Number7,
    Number8,
    Number9,

	Max			//最大数(for文とか回す用)
};

class UI
{
private:
	std::vector<Object> objUI_{((uint32_t)UIType::Max)};		//UI描画用オブジェクト群
	std::vector<uint64_t> ui_{};		//UIテクスチャハンドル群
	std::vector<Vec2> ancorPoint_{};	//UI描画基準点群
	std::vector<Vec4> color_{};			//UI描画色群

public:
	UI();
	static UI* GetInstance();
	//全て読み込み
	void AllLoad();
	void Init();
	void Update();

	//指定されたUIを描画
	void Draw(UIType uiType);
	void Draw2(UIType uiType, float rot);

	//指定されたUIの描画座標設定
	void SetPos(UIType uiType,const Vec2& pos);
	//指定されたUIの描画サイズ設定
	void SetSize(UIType uiType, float size);
	//指定されたUIの描画基準点設定
	void SetAncorPoint(UIType uiType, const Vec2& ancorPoint);
	//指定されたUIの描画色設定
	void SetColor(UIType uiType, const Vec4& color);
};