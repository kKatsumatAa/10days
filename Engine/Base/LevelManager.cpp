#include "LevelManager.h"
#include "ModelManager.h"
#include "CameraManager.h"
#include "TouchableObject.h"
#include "MeshCollider.h"
#include "ObjectManager.h"
#include "SphereCollider.h"


const std::string LevelManager::S_OBJ_GROUP_NAME_ = LevelManager::GetInstance().FILE_NAME_;


LevelManager::~LevelManager()
{
}

LevelManager& LevelManager::GetInstance()
{
	static LevelManager sInst;
	return sInst;
}

void LevelManager::StaticInitialize()
{
}

void LevelManager::LoadLevelData(int32_t fileIndex)
{
	//json読み込み
	JsonLevelLoader::Getinstance().LoadJsonFile(FILE_NAME_ + std::to_string(fileIndex));

	//オブジェクト、カメラをクリア
	ObjectManager::GetInstance().ClearAllObj();
	CameraManager::GetInstance().Initialize();
	lightManager_->InitializeActive();

	//レベルデータからカメラを取得
	for (auto& cameraData : JsonLevelLoader::Getinstance().levelData_->cameras)
	{
		//カメラ名からカメラを作成
		CameraManager::GetInstance().AddCamera(cameraData->fileName);
		//cameraのパラメータセット
		Camera* camera = CameraManager::GetInstance().GetCamera(cameraData->fileName);
		camera->SetEye(cameraData->camera->GetEye());
		camera->SetTarget(cameraData->camera->GetTarget());
		camera->SetUp(cameraData->camera->GetUp());

		//とりあえずカメラをセット
		CameraManager::GetInstance().SetUsingCamera(cameraData->fileName);

		//cameraManagerに登録されてるのでこっちでは登録しない
	}

	//レベルデータからライトを取得
	for (auto& lightData : JsonLevelLoader::Getinstance().levelData_->lights)
	{
		//
		LoadLight(*lightData.get());
	}

	//レベルデータからオブジェクトを生成、配置
	for (auto& objData : JsonLevelLoader::Getinstance().levelData_->objects)
	{
		//
		LoadObj(*objData.get());
	}
}

void LevelManager::CheckLandShapeObject(const LevelData::ObjectData& objData, bool& isLandShape)
{
	//地形オブジェクト
	if (objData.colliderData.attribute & COLLISION_ATTR_LANDSHAPE)
	{
		isLandShape = true;
	}
	else
	{
		isLandShape = false;
	}
}

void LevelManager::SetCollider(Object* obj, const LevelData::ObjectData& objData, bool isSettingCollider)
{
	//タイプがなければコライダー無しなので
	if (objData.colliderData.colliderType != CollisionShapeType::SHAPE_UNKNOWN)
	{
		CollisionShapeType type = objData.colliderData.colliderType;
		uint16_t attribute = objData.colliderData.attribute;

		if (isSettingCollider)
		{
			//球コライダー
			if (type == COLLISIONSHAPE_SPHERE)
			{
				obj->SetCollider(std::make_unique<SphereCollider>());
			}
			//メッシュコライダー
			else if (type == COLLISIONSHAPE_MESH)
			{
				obj->SetCollider(std::make_unique<MeshCollider>());
			}
		}
		//判定属性セット
		obj->GetCollider()->SetAttribute(attribute);
	}
}

void LevelManager::CreateObjectOrTouchableObject(std::unique_ptr<Object>& obj, LevelData::ObjectData& objData, bool isLandShape, IModel* model)
{
	//地形オブジェクトとして使うのなら
	if (isLandShape_)
	{
		obj = TouchableObject::Create(std::move(objData.worldMat), model);
	}
	//それ以外は普通のObject
	else
	{
		obj = std::make_unique<Object>();
		obj->SetWorldMat(std::move(objData.worldMat));
	}
}

void LevelManager::SetParentNode(Object* obj, const LevelData::ObjectData& objData, IModel* model
	, Object* child)
{
	if (child && objData.childData->parentNodeData.nodeName.size())
	{
		child->ParentFbxNode(obj, model, objData.childData->parentNodeData.nodeName);
	}
}

//-------------------------------------------------------------------------------------------
void LevelManager::LoadObj(LevelData::ObjectData& objData)
{
	//親がいたらスキップ(登録済みならworldMatはmove()されて無いので)
	if (/*objData.fileName == "gun" &&*/ !objData.worldMat)
	{
		return;
	}

	IModel* model = nullptr;
	//ファイル名から登録済みモデルを検索
	//(enemyはオブジェ、メッシュごとに演出させたいので別々のインスタンスとして読み込む)
	if (objData.fileName.find("enemy") != std::string::npos)
	{
		model = ModelManager::GetInstance().LoadModel(objData.fileName, false, true);
	}
	else
	{
		model = ModelManager::GetInstance().LoadModel(objData.fileName);
	}

	//3Dオブジェクトを生成
	std::unique_ptr <Object> newObj = {};

	//地形オブジェクトとして使うか
	CheckLandShapeObject(objData, isLandShape_);

	newObj = std::make_unique<Object>();

	//名前
	newObj->SetObjName(objData.fileName);

	//判定系
	SetCollider(newObj.get(), objData, !isLandShape_);

	//モデルセット
	newObj->SetModel(model);

	newObj->PlayAnimation(true);

	//obj登録
	ObjectManager::GetInstance().AddObject(S_OBJ_GROUP_NAME_, std::move(newObj));
}

void LevelManager::LoadLight(LevelData::LightData& lightData)
{
	int32_t index = -1;

	//方向ライトなら
	if (lightData.fileName.find("dir") != std::string::npos)
	{
		lightManager_->GetDoNotUseDirLightIndex(index);

		lightManager_->SetDirLightActive(index, true);

		lightManager_->SetDirLightDir(index,
			{ lightData.dir.x_,lightData.dir.y_,lightData.dir.z_ });
	}
	//ポイントライトなら
	else if (lightData.fileName.find("point") != std::string::npos)
	{
		lightManager_->GetDoNotUsePointLightIndex(index);

		lightManager_->SetPointLightActive(index, true);

		lightManager_->SetPointLightPos(index,
			{ lightData.trans.x_,lightData.trans.y_ ,lightData.trans.z_ });

		lightManager_->SetPointLightAtten(index,
			{ lightData.scale.z_,lightData.scale.x_,lightData.scale.y_ });
	}
	//スポットライトなら
	else if (lightData.fileName.find("spot") != std::string::npos)
	{
		lightManager_->GetDoNotUseSpotLightIndex(index);

		lightManager_->SetSpotLightActive(index, true);

		lightManager_->SetSpotLightPos(index,
			{ lightData.trans.x_,lightData.trans.y_ ,lightData.trans.z_ });

		lightManager_->SetSpotLightAtten(index,
			{ lightData.scale.z_,lightData.scale.x_,lightData.scale.y_ });

		lightManager_->SetSpotLightDir(index,
			{ lightData.dir.x_,lightData.dir.y_,lightData.dir.z_ });
	}
}

void LevelManager::Update()
{

}

void LevelManager::Draw(Camera* camera)
{
}

void LevelManager::DrawImGui()
{

}
