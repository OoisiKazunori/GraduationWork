#include "GameScene.h"
#include"Input/KeyBoradInputManager.h"
#include"Input/ControllerInputManager.h"
#include"Imgui/MyImgui.h"
#include"Helper/ResourceFilePass.h"
#include"../Game/Debug/ParameterMgr.h"
#include"Math/KazMath.h"
#include"../Game/Input/Input.h"
#include"../Game/Player/Player.h"
#include "../Enemy/EnemyManager.h"
#include"../Game/Camera.h"
#include"../Game/Collision/MeshCollision.h"
#include"../Bullet/BulletMgr.h"
#include"../KazLibrary/Buffer/GBufferMgr.h"
#include "../KazLibrary/PostEffect/Outline.h"
#include "../Game/Enemy/PreEnemy.h"
#include "../Game/ThrowableObject/ThrowableObjectController.h"
#include "StageSelectScene.h"
#include"../MapLoader/MapLoader.h"
#include "../UI/UI.h"
#include"../KazLibrary/Debug/DebugKey.h"
#include"../Footprint/FootprintMgr.h"
#include "../Effect/StopMgr.h"

GameScene::GameScene(DrawingByRasterize& arg_rasterize, int f_mapNumber, bool f_isGoal) :
	//DrawFuncHelperでのモデル読み込み
	m_uiManager(arg_rasterize),
	//m_gadgetMaanager(arg_rasterize),
	m_HPBarManager(arg_rasterize),
	m_heartRateManager(arg_rasterize),
	m_menu(arg_rasterize),
	m_resultManager(arg_rasterize),
	m_goalPoint(arg_rasterize),
	m_dangerManager(arg_rasterize),
	m_titleTex(arg_rasterize, "Resource/Title/TaitleLogo.png", true),
	m_isClear(false),
	m_turret(arg_rasterize),
	m_titleLogoModel(arg_rasterize, "Resource/Title/", "TitleLogoModel.gltf"),
	m_clickToStart(arg_rasterize, "Resource/Title/", "ClickToStartModel.gltf"),
	m_intractUI(arg_rasterize),
	m_todo(arg_rasterize)
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

	//帰って行くときのフラグ
	m_isToStartPos = f_isGoal;

	m_stageNum = f_mapNumber;
	MapManager::Init();
	int stageNumber = 0;
	m_stageManager.Init(arg_rasterize, f_mapNumber, f_isGoal);
	if (f_mapNumber == 0 && !f_isGoal)
	{
		Menu::InitGetFileIndex();
	}

	if (f_mapNumber == 0)
	{
		m_isTitle = true;
	}
	else
	{
		m_isTitle = false;
	}

	m_stageMeshCollision = std::make_shared<MeshCollision>();
	//2階に行くとき確認する
	m_stageMeshCollision->Setting(m_stageManager.m_stage[m_stageNum]->m_stageModelRender.m_model.m_modelInfo->modelData[m_stageNum].vertexData, m_stageManager.m_stage[m_stageNum]->m_transform);

	//m_enemyManager = std::make_shared<EnemyManager>();
	//auto l_enemyData = MapManager::GetEnemyData(m_stageNum);
	//m_enemyManager->SetMapData(m_stageNum, l_enemyData, arg_rasterize);
	//MapManager::GetPlayerStartPosition(0)
	//m_player = std::make_shared<Player>(arg_rasterize, KazMath::Transform3D({ -202.0f,-38.0f,291.0f }));
	m_player = std::make_shared<Player>(arg_rasterize, MapManager::GetPlayerStartPosition(m_stageNum));
	m_camera = std::make_shared<Camera>();
	m_bulletMgr = std::make_shared<BulletMgr>(arg_rasterize);
	m_throwableObjectController = std::make_shared<ThrowableObjectController>(arg_rasterize);

	m_sceneNum = SCENE_NONE;


	m_axis.Load(arg_rasterize, "Resource/Test/", "Axis.glb");
	m_axixTransform.scale.z += 1.0f;

	FootprintMgr::Instance()->Setting(arg_rasterize);

	//敵
	m_enemyManager = std::make_shared<EnemyManager>();
	for (int i = 0; i < m_stageManager.GetEnemyCount(); ++i)
	{
		//0が空
		m_enemyManager->AddEnemyData(
			i,
			m_stageManager.GetEnemyPositions(i + 1));
	}
	for (int i = 0; i < m_stageManager.GetTurretCount(); ++i)
	{
		std::list<KazMath::Transform3D> l_pos;
		l_pos.push_back(
			m_stageManager.GetTurretPosition(i));
		m_enemyManager->AddEnemyData(
			i + m_stageManager.GetEnemyCount(),
			l_pos);
	}
	m_enemyManager->SetModelData(arg_rasterize);

	//タイトルロゴモデルの位置を調整。
	m_titleLogoTransform.pos = TITLELOGO_POS;
	m_titleLogoTransform.Rotation(KazMath::Vec3<float>(0.0f, 1.0f, 0.0f), DirectX::XM_PI / 2.0f);
	m_titleLogoSineTimer = 0;
	m_titleLogoSIneRotationTimer = 0;
	m_titleLogoExitTimer = 0;

	if (f_isGoal)
	{
		m_player->SetPosition(m_stageManager.GetGoalTransform().pos);
		m_isTitle = false;
	}
	EchoArray::Instance()->Init();


	for (auto& obj : m_serverSmokeEmitter)
	{
		obj.Load(arg_rasterize);
	}

	SmokeEmitter::EmittData emittData;
	emittData.m_emittPos =
		KazMath::Vec3<float>(-39.7f, 92.0f, -12.0f);
	emittData.m_range =
		KazMath::Vec3<float>(2.0f, 1.0f, 6.0f);
	emittData.m_smokeTime =
		60 * 10;
	emittData.m_loopFlag =
		true;
	emittData.m_color =
		KazMath::Color(255, 255, 255, 255);
	emittData.m_minScale = 0.001f;
	emittData.m_maxScale = 0.1f;

	emittData.m_minActiveTime = 60.0f;
	emittData.m_maxActiveTime = 60.0f * 3.0f;
	for (auto& obj : m_serverEmittData)
	{
		obj = emittData;
	}

	m_serverEmittData[0].m_emittPos =
		KazMath::Vec3<float>(-39.7f, 92.0f, -12.0f);
	m_serverEmittData[1].m_emittPos =
		KazMath::Vec3<float>(-64.0f, 90.0f, -19.0f);
	m_serverEmittData[2].m_emittPos =
		KazMath::Vec3<float>(-84.0f, 90.0f, -46.0f);
	m_serverEmittData[3].m_emittPos =
		KazMath::Vec3<float>(-20.2f, 90.0f, -45.0f);
	m_serverEmittData[4].m_emittPos =
		KazMath::Vec3<float>(-21.2f, 90.0f, -4.0f);
	m_serverEmittData[5].m_emittPos =
		KazMath::Vec3<float>(-84.2f, 90.0f, -6.0f);

}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	m_sceneNum = SCENE_NONE;
	m_bulletMgr->Init();
	m_uiManager.Init();
	//m_gadgetMaanager.Init();
	if (m_isToStartPos)
	{
		m_goalPoint.Init(m_stageManager.m_player->m_transform.pos);
	}
	else
	{
		if (m_stageNum == 0)
		{
			m_goalPoint.Init({m_stageManager.m_clip1->m_transform.pos.x,
							  m_stageManager.m_clip1->m_transform.pos.y + 5.0f,
							  m_stageManager.m_clip1->m_transform.pos.z,
			});
			isClip = true;
		}
		else
		{
			m_goalPoint.Init(m_stageManager.GetGoalTransform().pos);
			isClip = false;
		}
	}
	FootprintMgr::Instance()->Init();
	m_debugCameraFlag = false;
	m_titleLogoExitTimer = 0;
	m_titleLogoExitEasingTimer = 0;
	m_intractUI.Init();
}

void GameScene::PreInit()
{
}

void GameScene::Finalize()
{
}

void GameScene::Input()
{
	if (DebugKey::Instance()->DebugKeyTrigger(DIK_1, "DebugCamera", "DIK_1"))
	{
		m_debugCameraFlag = !m_debugCameraFlag;
	}
	if (DebugKey::Instance()->DebugKeyTrigger(DIK_2, "ShotEffect", "DIK_2"))
	{
		for (int i = 0; i < m_serverSmokeEmitter.size(); ++i)
		{
			m_serverSmokeEmitter[i].Init(m_serverEmittData[i]);
		}
	}
	if (DebugKey::Instance()->DebugKeyTrigger(DIK_3, "rota", "DIK_3"))
	{
		m_turret.Stop();
	}
	if (m_isTitle && KeyBoradInputManager::Instance()->MouseInputTrigger(MouseInputNumber::MOUSE_INPUT_LEFT))
	{
		m_isTitle = false;
		Menu::InitGetFileIndex();
		m_todo.Init();
		//大きめのエコーを出す
		EchoArray::Instance()->Generate(m_player->GetTransform().pos, 80.0f, Echo::COLOR::WHITE);

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
	//

	/*if (KeyBoradInputManager::Instance()->InputTrigger(DIK_P))
	{
		m_HPBarManager.HitDamage(10, 10);
	}*/

	//int l_num = m_stageManager.GetEnemyCount();
	//m_stageManager.GetEnemyPositions(1);
	if (!m_isTitle)
	{
		//メニューが開かれていない時に更新を通す
		if (!m_menu.GetIsMenuOpen() && !m_resultManager.GetResultShow())
		{
			m_intractUI.isIntract = false;
			//m_uiManager.Update();
			//m_gadgetMaanager.Update();
			//m_HPBarManager.Update(0);

			//m_player->Update(m_camera, m_stageMeshCollision, m_bulletMgr, m_stageManager.GetColliders());
			//m_camera->Update(m_player->GetTransform(), m_stageMeshCollision, m_player->GetIsADS());
			//m_bulletMgr->Update(m_stageMeshCollision);

			m_throwableObjectController->Update(m_player->GetTransform(), m_camera->GetShotQuaternion().GetFront(), m_stageManager.GetColliders());

			m_stageManager.CheckInEcho(m_stageMeshCollision);

			m_goalPoint.CalucurateDistance(m_player->GetTransform().pos);
			m_goalPoint.Update();
			m_turret.Update();
			FootprintMgr::Instance()->Update();

			m_stageManager.Update(arg_rasterize, m_player->GetTransform());

			for (auto& obj : m_serverSmokeEmitter)
			{
				obj.Update();
			}


			if (m_HPBarManager.GetHP() > 0)
			{
				m_uiManager.Update(m_stageManager, m_player->GetTransform());

				//m_gadgetMaanager.Update();

				m_player->Update(m_camera, m_uiManager.GetNowWepon(), m_bulletMgr, m_throwableObjectController, m_stageManager.GetColliders(), m_HPBarManager, m_isTitle);
				m_enemyManager->Update(
					m_stageManager.GetColliders(),
					m_bulletMgr,
					m_player->GetTransform(),
					m_stageMeshCollision);

				if (m_debugCameraFlag)
				{
					m_debuCamera.Update();
				}
				else
				{
					m_camera->Update(m_player->GetTransform(), m_stageMeshCollision, m_player->GetIsADS(), m_isTitle);
				}

				m_stageManager.Update(arg_rasterize, m_player->GetTransform());
				//auto hogehoge = m_stageManager.GetTurretPosition(0);

				m_bulletMgr->Update(m_stageManager.GetColliders());

				/*static bool flag = false;
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
				}*/
				//nextステージへいくところを踏んだら
				//プレイヤーとゴールの当たり判定
				KazMath::Vec3<float> goalPos = m_stageManager.GetGoalTransform().pos;
				KazMath::Vec3<float> goalScale = m_stageManager.GetGoalTransform().scale;
				KazMath::Vec3<float> playerPos = m_player->GetTransform().pos;
				KazMath::Vec3<float> playerGoalDistane = goalPos - playerPos;

				int l_nowStageNum = StageSelectScene::GetStartStageNum();

				if (!m_isToStartPos && !m_isClear && fabs(playerGoalDistane.x) < goalScale.x && fabs(playerGoalDistane.z) < goalScale.z)
				{
					//すべてのステージクリア
					//インタラクト入れるなら入れる
					if (StageSelectScene::GetStartStageNum() == StageSelectScene::C_StageMaxNum - 1)
					{
						m_intractUI.isIntract = true;
						if (KeyBoradInputManager::Instance()->InputTrigger(DIK_F))
						{
							m_isToStartPos = true;
							m_goalPoint.Init(m_stageManager.m_player->m_transform.pos);
							m_todo.NextTask();

							for (int i = 0; i < m_serverSmokeEmitter.size(); ++i)
							{
								m_serverSmokeEmitter[i].Init(m_serverEmittData[i]);
							}
						}
					}
					else
					{
						if (isClip)
						{
							
						}
						else
						{
							StageSelectScene::startStageNum += 1;
							m_sceneNum = 1;
							m_isClear = true;
						}
					}
				}
				else if (m_isToStartPos)
				{
					//スタートのポジション
					goalPos = m_stageManager.m_player->m_transform.pos;
					playerGoalDistane = goalPos - playerPos;
					if (!m_isClear && fabs(playerGoalDistane.x) < goalScale.x && fabs(playerGoalDistane.z) < goalScale.z)
					{
						if (StageSelectScene::startStageNum == 0)
						{
							/*m_isToStartPos = true;
							m_goalPoint.Init(m_stageManager.m_player->m_transform.pos);*/
							//ゲームクリア
							StageSelectScene::startStageNum = 0;
							m_sceneNum = 1;
						}
						else
						{
							StageSelectScene::startStageNum -= 1;
							m_sceneNum = 3;
							m_isClear = true;
						}
					}
				}

			}
			m_HPBarManager.Update(0);
			//死んだときの更新
			/*if (m_HPBarManager.GetHP() <= 0 && m_HPBarManager.RedHP() <= 0)
			{
				m_resultManager.ShowResult();
			}*/
			//dangerを出すのをいったん切る
			/*if (KeyBoradInputManager::Instance()->InputTrigger(DIK_5))
			{
				m_dangerManager.Update(true);
			}
			else
			{
				m_dangerManager.Update(false);
			}*/

		}

		m_menu.Update();
		m_HPBarManager.Update(0);
		//死んだときの更新
		if (m_HPBarManager.GetHP() <= 0 && m_HPBarManager.RedHP() <= 0)
		{
			m_resultManager.ShowResult();
		}
		/*if (KeyBoradInputManager::Instance()->InputTrigger(DIK_5))
		{
			m_dangerManager.Update(true);
		}
		else
		{*/
			m_dangerManager.Update(false);
		//}

		//タイトルロゴが消えるまでのタイマーを加算して、一定以上になったらタイトルロゴを消す処理を入れる。
		++m_titleLogoExitTimer;
		if (TITLELOGO_EXIT_TIMER < m_titleLogoExitTimer) {

			m_titleLogoExitEasingTimer = std::clamp(m_titleLogoExitEasingTimer + 0.06f, 0.0f, 1.0f);

			float easingAmount = EasingMaker(In, Back, m_titleLogoExitEasingTimer);

			m_titleLogoTransform.pos.y = TITLELOGO_POS.y + sinf(m_titleLogoSineTimer) * TITLELOGO_SINE_MOVE;
			m_titleLogoTransform.pos.y -= easingAmount * 10.0f;

			m_clickToStartTransform = m_titleLogoTransform;
			m_clickToStartTransform.quaternion = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 1), DirectX::XM_PI / 2.0f);
			m_clickToStartTransform.pos.y -= 1.5f;

		}
		//リザルト出す
		if (m_resultManager.GetResultShow())
		{
			//メニューが開かれていない時に更新を通す
			if (!m_menu.GetIsMenuOpen() && !m_resultManager.GetResultShow())
			{
				//m_uiManager.Update();
				//m_gadgetMaanager.Update();
				//m_HPBarManager.Update(0);

				//m_player->Update(m_camera, m_stageMeshCollision, m_bulletMgr, m_stageManager.GetColliders());
				//m_camera->Update(m_player->GetTransform(), m_stageMeshCollision, m_player->GetIsADS());
				//m_bulletMgr->Update(m_stageMeshCollision);

				m_stageManager.Update(arg_rasterize, m_player->GetTransform());

				if (m_HPBarManager.GetHP() > 0)
				{
					m_uiManager.Update(m_stageManager, m_player->GetTransform());
					//m_gadgetMaanager.Update();

					m_player->Update(m_camera, m_uiManager.GetNowWepon(), m_bulletMgr, m_throwableObjectController, m_stageManager.GetColliders(), m_HPBarManager, m_isTitle);
					m_enemyManager->Update(
						m_stageManager.GetColliders(),
						m_bulletMgr,
						m_player->GetTransform(),
						m_stageMeshCollision);

					if (m_debugCameraFlag)
					{
						m_debuCamera.Update();
					}
					else
					{
						m_camera->Update(m_player->GetTransform(), m_stageMeshCollision, m_player->GetIsADS(), m_isTitle);
					}

					m_stageManager.Update(arg_rasterize, m_player->GetTransform());
					m_bulletMgr->Update(m_stageManager.GetColliders());

					/*static bool flag = false;
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
					}*/
					//nextステージへいくところを踏んだら
					//プレイヤーとゴールの当たり判定
					KazMath::Vec3<float> goalPos = m_stageManager.GetGoalTransform().pos;
					KazMath::Vec3<float> goalScale = m_stageManager.GetGoalTransform().scale;
					KazMath::Vec3<float> playerPos = m_player->GetTransform().pos;
					KazMath::Vec3<float> playerGoalDistane = goalPos - playerPos;
					if (!m_isClear && fabs(playerGoalDistane.x) < goalScale.x && fabs(playerGoalDistane.y) < goalScale.y && fabs(playerGoalDistane.z) < goalScale.z) {

						//すべてのステージクリア
						if (StageSelectScene::GetStartStageNum() == StageSelectScene::C_StageMaxNum - 1)
						{
							m_resultManager.ShowResult();
							m_resultManager.SetClear();
							StageSelectScene::startStageNum = 0;
						}
						else
						{
							StageSelectScene::startStageNum += 1;
							if (StageSelectScene::startStageNum % 2 == 0)
							{
								m_sceneNum = 1;
							}
							else
							{
								m_sceneNum = 3;
							}
						}
						m_isClear = true;
					}
				}
				m_HPBarManager.Update(0);
				//死んだときの更新
				/*if (m_HPBarManager.GetHP() <= 0 && m_HPBarManager.RedHP() <= 0)
				{
					m_resultManager.ShowResult();
				}*/
				/*if (KeyBoradInputManager::Instance()->InputTrigger(DIK_5))
				{
					m_dangerManager.Update(true);
				}*/
				/*else
				{
					m_dangerManager.Update(false);
				}*/

			}
			//リザルト出す
			else if (m_resultManager.GetResultShow())
			{
				m_resultManager.Update();
				if (KeyBoradInputManager::Instance()->InputTrigger(DIK_SPACE))
				{
					//タイトルに戻る
					//m_sceneNum = 0;
					m_sceneNum = 1;
					StageSelectScene::startStageNum = 0;
				}
			}

			//auto hogehoge = MapManager::GetEnemyData(m_stageNum);

			//int sam1 = MapManager::GetMapChips(m_stageNum, 0, 0);
			//int sam2 = MapManager::GetMapChips(m_stageNum, 4, 8);



			/*m_goalPoint.CalucurateDistance(m_player->GetTransform().pos);
			m_goalPoint.Update();

			FootprintMgr::Instance()->Update();*/

			//EnemyDebugManager::Instance()->Update();
			/*FieldAI::Instance()->DebugUpdate();
			FieldAIDebugManager::Instance()->Update();*/

		}

		//クリップとの当たり判定
		KazMath::Vec3<float> clipPos;
		KazMath::Vec3<float> goalScale = { 3.0f, 0.7f, 7.0f };
		KazMath::Vec3<float> playerPos = m_player->GetTransform().pos;
		KazMath::Vec3<float> playerGoalDistane;
		if (m_stageManager.m_clip1)
		{
			clipPos = m_stageManager.m_clip1->m_transform.pos;
			playerGoalDistane = clipPos - playerPos;
			if (fabs(playerGoalDistane.x) < goalScale.x && fabs(playerGoalDistane.z) < goalScale.z)
			{
				IntractUI::isIntract = true;
				if (KeyBoradInputManager::Instance()->InputTrigger(DIK_F))
				{
					m_stageManager.m_clip1.reset();
					//とった処理ここに入れる
					Menu::GetFile(0);
					m_todo.NextTask();
					Menu::firstLookFile = true;
					Menu::firstLookFileIndex = 0;
					isClip = false;
					m_stageManager.Erase1StageCol();
					m_goalPoint.Init(m_stageManager.GetGoalTransform().pos);
				}
			}
		}
		if (m_stageManager.m_clip2)
		{
			clipPos = m_stageManager.m_clip2->m_transform.pos;
			playerGoalDistane = clipPos - playerPos;
			if (fabs(playerGoalDistane.x) < goalScale.x && fabs(playerGoalDistane.z) < goalScale.z)
			{
				IntractUI::isIntract = true;
				if (KeyBoradInputManager::Instance()->InputTrigger(DIK_F))
				{
					m_stageManager.m_clip2.reset();
					//とった処理ここに入れる
					Menu::GetFile(1);
					Menu::firstLookFile = true;
					Menu::firstLookFileIndex = 1;
				}
			}
		}
		if (m_stageManager.m_clip3)
		{
			clipPos = m_stageManager.m_clip3->m_transform.pos;
			playerGoalDistane = clipPos - playerPos;
			if (fabs(playerGoalDistane.x) < goalScale.x && fabs(playerGoalDistane.z) < goalScale.z)
			{
				IntractUI::isIntract = true;
				if (KeyBoradInputManager::Instance()->InputTrigger(DIK_F))
				{
					m_stageManager.m_clip3.reset();
					//とった処理ここに入れる
					Menu::GetFile(2);
					Menu::firstLookFile = true;
					Menu::firstLookFileIndex = 2;
				}
			}
		}
		if (m_stageManager.m_clip4)
		{
			clipPos = m_stageManager.m_clip4->m_transform.pos;
			playerGoalDistane = clipPos - playerPos;
			if (fabs(playerGoalDistane.x) < goalScale.x && fabs(playerGoalDistane.z) < goalScale.z)
			{
				IntractUI::isIntract = true;
				if (KeyBoradInputManager::Instance()->InputTrigger(DIK_F))
				{
					m_stageManager.m_clip4.reset();
					//とった処理ここに入れる
					Menu::GetFile(3);
					Menu::firstLookFile = true;
					Menu::firstLookFileIndex = 3;
				}
			}
		}
		m_todo.Update();
	}

	//タイトル画面
	else
	{

		//サイン波で動かす。
		m_titleLogoSineTimer += 0.04f;
		m_titleLogoTransform.pos.y = TITLELOGO_POS.y + sinf(m_titleLogoSineTimer) * TITLELOGO_SINE_MOVE;

		m_titleLogoSIneRotationTimer += 0.03f;
		m_titleLogoTransform.quaternion = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 1), DirectX::XM_PI / 2.0f);
		m_titleLogoTransform.Rotation(KazMath::Vec3<float>(0, 1, 0), sinf(m_titleLogoSIneRotationTimer) * DirectX::XM_PI / 50.0f);

		m_clickToStartTransform = m_titleLogoTransform;
		m_clickToStartTransform.quaternion = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 1), DirectX::XM_PI / 2.0f);
		m_clickToStartTransform.pos.y -= 1.5f;

		static bool isHoge = false;
		if (!isHoge)
		{
			//m_player->Update(m_camera, m_uiManager.GetNowWepon(), m_bulletMgr, m_throwableObjectController, m_stageManager.GetColliders(), m_HPBarManager);
			isHoge = true;
		}
		m_player->TitleUpdate(m_camera, arg_rasterize, m_stageManager.GetColliders());

		m_camera->Update(m_player->GetTransform(), m_stageMeshCollision, m_player->GetIsADS(), m_isTitle);

	}

	m_intractUI.Update();
	m_intractUI.oldIsIntract = m_intractUI.isIntract;

}


void GameScene::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{


	m_enemyManager->Draw(arg_rasterize, arg_blasVec);

	m_bulletMgr->Draw(arg_rasterize, arg_blasVec);

	//ここにあるのはデラが描画したい者たち
	m_stageManager.Draw(arg_rasterize, arg_blasVec);

	m_player->Draw(arg_rasterize, arg_blasVec);

	for (auto& obj : m_serverSmokeEmitter)
	{
		obj.Draw(arg_rasterize, arg_blasVec);
	}

	m_menu.Draw(arg_rasterize);
	if (!m_resultManager.GetResultShow() && !m_menu.GetIsMenuOpen())
	{
		if (!m_isTitle)
		{
			//m_gadgetMaanager.Draw(arg_rasterize);
			//m_heartRateManager.Draw(arg_rasterize);


			m_uiManager.Draw(arg_rasterize);
			m_HPBarManager.Draw(arg_rasterize);
			m_dangerManager.Draw(arg_rasterize);
			m_intractUI.Draw(arg_rasterize);
			m_goalPoint.Draw(arg_rasterize);
			m_todo.Draw(arg_rasterize);
		}
	}


	m_axis.m_model.Draw(arg_rasterize, arg_blasVec, m_axixTransform);

	if (m_isTitle)
	{
		m_titleTrans.pos = { 1280.0f / 2.0f,720.0f / 2.0f - 200.0f };
		//m_titleTex.m_tex.Draw2D(arg_rasterize, m_titleTrans);
	}
	m_turret.Draw(arg_rasterize, arg_blasVec);
	m_axis.m_model.Draw(arg_rasterize, arg_blasVec, m_axixTransform);

	//if (m_isTitle)
	//{

	m_titleLogoModel.m_model.DrawRasterize(arg_rasterize, m_titleLogoTransform);
	m_clickToStart.m_model.DrawRasterize(arg_rasterize, m_clickToStartTransform);
	//}

	//m_goalPoint.Draw(arg_rasterize);

	FootprintMgr::Instance()->Draw(arg_rasterize, arg_blasVec);

	//m_menu.Draw(arg_rasterize);
	//m_menu.Draw(arg_rasterize);
	//m_line.m_render.Draw(arg_rasterize, arg_blasVec, { 0.0f,0.0f,0.0f }, { 100.0f,100.0f,100.0f }, KazMath::Color(255, 0, 0, 255));
	//m_bulletMgr->Draw(arg_rasterize, arg_blasVec);
	m_throwableObjectController->Draw(arg_rasterize, arg_blasVec);

	if (m_resultManager.GetResultShow())
	{
		m_resultManager.Draw(arg_rasterize);
	}

	DebugKey::Instance()->DrawImGui();
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
