#pragma once
#include <memory>
#include <string>
#include "SceneState.h"

// �V�KScene�N���X����邲�ƂɁA������CreateScene()�ɒǋL���Ă�������
#include "DemoScene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"

class SceneFactory final
{
public:
    // ��`
    enum class Usage
    {
        DEMO,
        TITLE,
        GAME,
        RESULT,
    };

    // �֐�
    std::unique_ptr<SceneState> CreateScene(Usage scene);
};

