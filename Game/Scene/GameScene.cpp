﻿#include "GameScene.h"
#include"Input/KeyBoradInputManager.h"
#include"Input/ControllerInputManager.h"
#include"Imgui/MyImgui.h"
#include"Helper/ResourceFilePass.h"
#include"../Game/Debug/ParameterMgr.h"
#include"Math/KazMath.h"
#include"../Game/Input/Input.h"
#include"../Game/Player/Player.h"
#include"../Game/Camera.h"
#include"../Game/Collision/MeshCollision.h"
#include"../Bullet/BulletMgr.h"
#include"../KazLibrary/Buffer/GBufferMgr.h"
#include "../KazLibrary/PostEffect/Outline.h"
#include "../Game/Enemy/PreEnemy.h"
#include "../Game/ThrowableObject/ThrowableObjectController.h"

#include"../MapLoader/MapLoader.h"

GameScene::GameScene(DrawingByRasterize& arg_rasterize, int f_mapNumber) :

	//DrawFuncHelperでのモデル読み込み
	m_line(arg_rasterize),
	m_stage(arg_rasterize, "Resource/Stage/", "Stage.gltf"),
	m_uiManager(arg_rasterize),
	m_gadgetMaanager(arg_rasterize),
	m_HPBarManager(arg_rasterize),
	m_heartRateManager(arg_rasterize),
	m_menu(arg_rasterize)
{

	/*
	テクスチャやモデルの読み込みはTextureRenderやModelRenderのコンストラクタで読み込まれますが、
	読み込み単体の処理は下の処理になります。(多重読み込み防止あり)
	読み込みの成否はコンソールに出力されます
	*/
	std::shared_ptr<ModelInfomation> model = ModelLoader::Instance()->Load("Resource/Test/Virus/", "virus_cur.gltf");
	KazBufferHelper::BufferData texture = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/Test/texas.png");

	//アニメーション再生
	//m_modelAnimationRender.m_model.m_animator->Play("繧｢繝ｼ繝槭メ繝･繧｢Action", true, false);

	m_stageTransform.pos = { 0.0f, -20.0f, 0.0f };
	m_stageTransform.scale = { 8.0f, 1.0f, 8.0f };

	MapManager::Init();
	int stageNumber = 0;
	m_stageManager.Init(arg_rasterize, f_mapNumber);
	m_stageNum = f_mapNumber;

	m_stageMeshCollision = std::make_shared<MeshCollision>();
	m_stageMeshCollision->Setting(m_stageManager.m_stage->m_stageModelRender.m_model.m_modelInfo->modelData[0].vertexData, m_stageManager.m_stage->m_transform);


	m_player = std::make_shared<Player>(arg_rasterize, MapManager::GetPlayerStartPosition(0));
	m_camera = std::make_shared<Camera>();
	m_bulletMgr = std::make_shared<BulletMgr>(arg_rasterize);
	m_throwableObjectController = std::make_shared<ThrowableObjectController>(arg_rasterize);

	m_sceneNum = SCENE_NONE;
	for (auto& index : m_preEnemy) {

		index = std::make_shared<PreEnemy>(arg_rasterize);

	}

}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	m_sceneNum = SCENE_NONE;
	m_bulletMgr->Init();
	m_uiManager.Init();
	m_gadgetMaanager.Init();
}

void GameScene::PreInit()
{
}

void GameScene::Finalize()
{
}

void GameScene::Input()
{
	//ゲームシーンへ
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_0))
	{
		m_sceneNum = 0;
	}
}

void GameScene::Update(DrawingByRasterize& arg_rasterize)
{
	/*
	カメラを使用する際は下の関数を使用し、eye, target, upの値を入れることで計算できます
	計算結果は描画情報に渡ります。
	CameraMgr::Instance()->Camera({}, {}, {});
	*/
	//デバック用のカメラワーク(操作はBlenderと同じ)
	//m_debuCamera.Update();

	//メニューが開かれていない時に更新を通す
	if (!m_menu.GetIsMenuOpen())
	{
		m_uiManager.Update();
		m_gadgetMaanager.Update();
		m_HPBarManager.Update(0);

		m_player->Update(m_camera, m_stageMeshCollision, m_bulletMgr, m_throwableObjectController, m_stageManager.GetColliders());
		m_camera->Update(m_player->GetTransform(), m_stageMeshCollision, m_player->GetIsADS());
		m_bulletMgr->Update(m_stageMeshCollision);


		m_stageManager.Update(arg_rasterize);

		static bool flag = false;
		if (KeyBoradInputManager::Instance()->InputTrigger(DIK_U))
		{
			if (flag) flag = false;
			else flag = true;
		}
		if (flag)
		{
			m_heartRateManager.Update(60);
		}
		else
		{
			m_heartRateManager.Update(120);
		}
	}
	auto hogehoge = MapManager::GetEnemyData(m_stageNum);
	m_menu.Update();

	m_throwableObjectController->Update(m_player->GetTransform().pos, m_camera->GetShotQuaternion().GetFront(), m_stageManager.GetColliders());

	for (auto& index : m_preEnemy)
	{
		index->CheckInEcho(m_stageMeshCollision);
	}
}

void GameScene::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	//描画命令発行

	m_player->Draw(arg_rasterize, arg_blasVec);
	//m_line.m_render.Draw(arg_rasterize, arg_blasVec, { 0.0f,0.0f,0.0f }, { 100.0f,100.0f,100.0f }, KazMath::Color(255, 0, 0, 255));
	//m_stage.m_model.Draw(arg_rasterize, arg_blasVec, m_stageTransform);
	m_bulletMgr->Draw(arg_rasterize, arg_blasVec);

	//ここにあるのはデラが描画したい者たち
	m_stageManager.Draw(arg_rasterize, arg_blasVec);
	m_uiManager.Draw(arg_rasterize);
	m_gadgetMaanager.Draw(arg_rasterize);
	m_HPBarManager.Draw(arg_rasterize);
	m_heartRateManager.Draw(arg_rasterize);


	m_menu.Draw(arg_rasterize);
	//m_line.m_render.Draw(arg_rasterize, arg_blasVec, { 0.0f,0.0f,0.0f }, { 100.0f,100.0f,100.0f }, KazMath::Color(255, 0, 0, 255));
	m_bulletMgr->Draw(arg_rasterize, arg_blasVec);
	m_throwableObjectController->Draw(arg_rasterize, arg_blasVec);


	for (auto& index : m_preEnemy) {

		index->Draw(arg_rasterize, arg_blasVec);
	}
}

int GameScene::SceneChange()
{
	if (m_sceneNum != SCENE_NONE)
	{
		int tmp = m_sceneNum;
		m_sceneNum = SCENE_NONE;
		return tmp;
	}
	return SCENE_NONE;
}
