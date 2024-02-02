#include "StageManager.h"
#include"Input/KeyBoradInputManager.h"
#include "../MapLoader/MapLoader.h"
#include "../Echo/EchoArray.h"

int StageNumCounter::stageNumCount = 0;

StageManager::StageManager() :m_changeSceneTriggerFlag(false)
{

}

void StageManager::Init(DrawingByRasterize& arg_rasterize, int f_stageNum)
{
	m_nowStageNumber = f_stageNum;
	m_changeSceneTriggerFlag = false;

	//最初のステージはこちらで読み込む
	AddMapDatas(arg_rasterize, f_stageNum);
}

void StageManager::Update(DrawingByRasterize& arg_rasterize)
{
	//if (m_nowStageNumber != m_nextStageNumber)
	//if (KeyBoradInputManager::Instance()->InputTrigger(DIK_4))
	//{
	//	ChangeScene(arg_rasterize);
	//	//ステージの切り替え処理
	//	int changeSceneNum = 0;
	//	AddMapDatas(arg_rasterize, changeSceneNum);
	//}

	//ステージの切り替え処理
	m_stage[m_nowStageNumber]->Update();
	m_goal->Update();

	//エコーの出し方
	//EchoArray::Instance()->Generate(m_transform.pos, 50.0f, Echo::COLOR::WHITE);

	for (auto l_treeItr = m_phone.begin(); l_treeItr != m_phone.end(); ++l_treeItr)
	{
		(*l_treeItr)->Update();
	}
	for (auto l_cylinderItr = m_cylinder.begin(); l_cylinderItr != m_cylinder.end(); ++l_cylinderItr)
	{
		(*l_cylinderItr)->Update();
	}
	for (auto l_block01Itr = m_block01.begin(); l_block01Itr != m_block01.end(); ++l_block01Itr)
	{
		(*l_block01Itr)->Update();
	}
	for (auto l_block01Itr = m_stone.begin(); l_block01Itr != m_stone.end(); ++l_block01Itr)
	{
		(*l_block01Itr)->Update();
	}

	for (auto itr = m_plane.begin(); itr != m_plane.end(); itr++)
	{
		(*itr)->Update();
	}

	static int bird1Ti = 300;
	static int bird2Ti = 500;
	static int bird3Ti = 700;
	//一ステージめだったら
	if (m_nowStageNumber == 0)
	{
		bird1Ti--;
		bird2Ti--;
		bird3Ti--;
		if (bird1Ti <= 0)
		{
			bird1Ti = 300;
			EchoArray::Instance()->Generate(m_Bird1->m_transform.pos, 30.0f, Echo::COLOR::WHITE, 200.0f);
		}
		if (bird2Ti <= 0)
		{
			bird2Ti = 500;
			EchoArray::Instance()->Generate(m_Bird2->m_transform.pos, 30.0f, Echo::COLOR::WHITE, 200.0f);
		}
		if (bird3Ti <= 0)
		{
			bird3Ti = 500;
			EchoArray::Instance()->Generate(m_Bird3->m_transform.pos, 30.0f, Echo::COLOR::WHITE, 200.0f);
		}
	}
}

void StageManager::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
 	m_stage[m_nowStageNumber]->Draw(arg_rasterize, arg_blasVec);
	//m_goal->Draw(arg_rasterize, arg_blasVec);
	for (auto l_treeItr = m_phone.begin(); l_treeItr != m_phone.end(); ++l_treeItr)
	{
		(*l_treeItr)->Draw(arg_rasterize, arg_blasVec);
	}
	/*for (auto l_cylinderItr = m_cylinder.begin(); l_cylinderItr != m_cylinder.end(); ++l_cylinderItr)
	{
		(*l_cylinderItr)->Draw(arg_rasterize, arg_blasVec);
	}*/
	/*for (auto l_block01Itr = m_block01.begin(); l_block01Itr != m_block01.end(); ++l_block01Itr)
	{
		(*l_block01Itr)->Draw(arg_rasterize, arg_blasVec);
	}*/
	for (auto l_cylinderItr = m_stone.begin(); l_cylinderItr != m_stone.end(); ++l_cylinderItr)
	{
		(*l_cylinderItr)->Draw(arg_rasterize, arg_blasVec);
	}

	for (auto itr = m_plane.begin(); itr != m_plane.end(); itr++)
	{
		(*itr)->Draw(arg_rasterize, arg_blasVec);
	}
}

bool StageManager::ChangeSceneTrigger()
{
	if (m_changeSceneTriggerFlag)
	{
		bool tmpFlag = m_changeSceneTriggerFlag;
		m_changeSceneTriggerFlag = false;
		return tmpFlag;
	}
	return false;
}

void StageManager::AddMapDatas(DrawingByRasterize& arg_rasterize, int f_stageNum)
{
	m_phone.clear();
	m_cylinder.clear();
	m_stage[0].reset();
	m_stage[1].reset();
	m_stage[2].reset();
	m_goal.reset();
	m_colStage[0].reset();
	m_colStage[1].reset();
	m_colStage[2].reset();
	m_block01.clear();
	m_player.reset();
	m_plane.clear();
	m_turrets.clear();

	std::list<MapObject> l_map = MapManager::GetStageData(f_stageNum);
	for (auto l_mapItr = l_map.begin(); l_mapItr != l_map.end(); ++l_mapItr)
	{
		if (l_mapItr->m_objetName.starts_with("Bird1") == true)
		{
			m_Bird1 = std::make_unique<StageModel>(arg_rasterize, "Resource/GoalTest/", "stageObjects1.gltf",
				DirectX::XMFLOAT3(-l_mapItr->m_position.x * 5.0f, l_mapItr->m_position.y + 3.0f, -l_mapItr->m_position.z * 5.0f),
				l_mapItr->m_rotition, l_mapItr->m_scale);
		}
		else if (l_mapItr->m_objetName.starts_with("Bird2") == true)
		{
			m_Bird2 = std::make_unique<StageModel>(arg_rasterize, "Resource/GoalTest/", "stageObjects1.gltf",
				DirectX::XMFLOAT3(-l_mapItr->m_position.x * 5.0f, l_mapItr->m_position.y + 3.0f, -l_mapItr->m_position.z * 5.0f),
				l_mapItr->m_rotition, l_mapItr->m_scale);
		}
		else if (l_mapItr->m_objetName.starts_with("Bird3") == true)
		{
			m_Bird3 = std::make_unique<StageModel>(arg_rasterize, "Resource/GoalTest/", "stageObjects1.gltf",
				DirectX::XMFLOAT3(-l_mapItr->m_position.x * 5.0f, l_mapItr->m_position.y + 3.0f, -l_mapItr->m_position.z * 5.0f),
				l_mapItr->m_rotition, l_mapItr->m_scale);
		}
		else if (l_mapItr->m_objetName.starts_with("player") == true)
		{
			m_player = std::make_unique<StageModel>(arg_rasterize, "Resource/GoalTest/", "stageObjects1.gltf",
				DirectX::XMFLOAT3(-l_mapItr->m_position.x * 5.0f, l_mapItr->m_position.y + 3.0f, -l_mapItr->m_position.z * 5.0f),
				l_mapItr->m_rotition, l_mapItr->m_scale);
		}
		else if (l_mapItr->m_objetName.starts_with("goal") == true)
		{
			m_goal = std::make_unique<StageModel>(arg_rasterize, "Resource/GoalTest/", "stageObjects1.gltf",
				DirectX::XMFLOAT3(-l_mapItr->m_position.x * 5.0f, l_mapItr->m_position.y + 3.0f, -l_mapItr->m_position.z * 5.0f),
				l_mapItr->m_rotition, l_mapItr->m_scale);
		}
		else if (l_mapItr->m_objetName.starts_with("Enemy") == true)
		{
			int length = (int)l_mapItr->m_objetName.size();

			std::string str = "";
			str += l_mapItr->m_objetName.at(length - 1);
			//順かい順
			int roadIndex = atoi(str.c_str());
			str = "";
			str += l_mapItr->m_objetName.at(length - 2);
			//エネミー自体の番号
			int enemyIndex = atoi(str.c_str());
			m_enemys.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/GoalTest/", "stageObjects1.gltf", enemyIndex, roadIndex,
				DirectX::XMFLOAT3(-l_mapItr->m_position.x * 5.0f, l_mapItr->m_position.y + 10.0f, -l_mapItr->m_position.z * 5.0f),
				l_mapItr->m_rotition, l_mapItr->m_scale, false));
		}
		else if (l_mapItr->m_objetName.starts_with("Turret") == true)
		{
			//タレットのモデルにする？
			m_turrets.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/GoalTest/", "stageObjects1.gltf",
				DirectX::XMFLOAT3(-l_mapItr->m_position.x * 5.0f, l_mapItr->m_position.y + 3.0f, -l_mapItr->m_position.z * 5.0f),
				l_mapItr->m_rotition, l_mapItr->m_scale));
		}

	}
	//emptyなら入れる
	if (!m_stage[m_nowStageNumber] && m_nowStageNumber == 0)
	{
		m_stage[m_nowStageNumber] = std::make_unique<StageModel>(arg_rasterize, "Resource/Stage/Stage/", "Project_S_NewStage_Model.gltf",
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
		m_colStage[m_nowStageNumber] = std::make_unique<StageModel>(arg_rasterize, "Resource/Stage/Stage/", "Project_S_NewStage_Model_collision.gltf",
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
		for (auto& index : m_colStage[m_nowStageNumber]->m_stageModelRender.m_model.m_modelInfo->modelData) {
			auto collision = std::make_shared<MeshCollision>();
			collision->Setting(index.vertexData, m_colStage[m_nowStageNumber]->m_transform);
			m_collisions[m_nowStageNumber].push_back(collision);
		}
	}
	else if (!m_stage[m_nowStageNumber] && m_nowStageNumber == 1)
	{
		m_stage[m_nowStageNumber] = std::make_unique<StageModel>(arg_rasterize, "Resource/Stage/Stage/", "Project_S_NewStage_Model2.gltf",
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
		m_colStage[m_nowStageNumber] = std::make_unique<StageModel>(arg_rasterize, "Resource/Stage/Stage/", "Project_S_NewStage_Corizon2.gltf",
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
		for (auto& index : m_colStage[m_nowStageNumber]->m_stageModelRender.m_model.m_modelInfo->modelData) {
			auto collision = std::make_shared<MeshCollision>();
			collision->Setting(index.vertexData, m_colStage[m_nowStageNumber]->m_transform);
			m_collisions[m_nowStageNumber].push_back(collision);
		}
	}
	else if (!m_stage[m_nowStageNumber] && m_nowStageNumber == 2)
	{
		m_stage[m_nowStageNumber] = std::make_unique<StageModel>(arg_rasterize, "Resource/Stage/Stage/", "Project_S_NewStage_Model3.gltf",
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
		m_colStage[m_nowStageNumber] = std::make_unique<StageModel>(arg_rasterize, "Resource/Stage/Stage/", "Project_S_NewStage_Corizon3.gltf",
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
		for (auto& index : m_colStage[m_nowStageNumber]->m_stageModelRender.m_model.m_modelInfo->modelData) {
			auto collision = std::make_shared<MeshCollision>();
			collision->Setting(index.vertexData, m_colStage[m_nowStageNumber]->m_transform);
			m_collisions[m_nowStageNumber].push_back(collision);
		}
	}
}

void StageManager::CheckInEcho(std::weak_ptr<MeshCollision> arg_stageMeshCollision)
{
	for (auto& obj : m_block01)
	{
		//全てのEchoとチェック
		obj->m_isDrawFlag = false;
		for (auto& index : EchoArray::Instance()->GetEcho()) {

			//エコーが生成されていなかったら。
			if (!index.GetIsActive()) continue;
			if (index.GetNowRadius() <= 0.1f) continue;

			//まずは球で当たり判定を行う。
			KazMath::Vec3<float> echoVec = obj->m_transform.pos - index.GetPos();
			float distance = echoVec.Length();
			if (index.GetNowRadius() <= distance) continue;

			//次にレイを飛ばして当たり判定を行う。
			MeshCollision::CheckHitResult result = arg_stageMeshCollision.lock()->CheckHitRay(index.GetPos(), echoVec.GetNormal());

			//当たっていたら
			if (!result.m_isHit || (result.m_isHit && distance <= fabs(result.m_distance))) {

				obj->m_isDrawFlag = true;
				break;
			}

		}
	}
	for (auto& obj : m_cylinder)
	{
		//全てのEchoとチェック
		obj->m_isDrawFlag = false;
		for (auto& index : EchoArray::Instance()->GetEcho()) {

			//エコーが生成されていなかったら。
			if (!index.GetIsActive()) continue;
			if (index.GetNowRadius() <= 0.1f) continue;

			//まずは球で当たり判定を行う。
			KazMath::Vec3<float> echoVec = obj->m_transform.pos - index.GetPos();
			float distance = echoVec.Length();
			if (index.GetNowRadius() <= distance) continue;

			//次にレイを飛ばして当たり判定を行う。
			MeshCollision::CheckHitResult result = arg_stageMeshCollision.lock()->CheckHitRay(index.GetPos(), echoVec.GetNormal());

			//当たっていたら
			if (!result.m_isHit || (result.m_isHit && distance <= fabs(result.m_distance))) {

				obj->m_isDrawFlag = true;
				break;
			}

		}
	}
	for (auto& obj : m_stone)
	{
		//全てのEchoとチェック
		obj->m_isDrawFlag = false;
		for (auto& index : EchoArray::Instance()->GetEcho()) {

			//エコーが生成されていなかったら。
			if (!index.GetIsActive()) continue;
			if (index.GetNowRadius() <= 0.1f) continue;

			//まずは球で当たり判定を行う。
			KazMath::Vec3<float> echoVec = obj->m_transform.pos - index.GetPos();
			float distance = echoVec.Length();
			if (index.GetNowRadius() <= distance) continue;

			//次にレイを飛ばして当たり判定を行う。
			MeshCollision::CheckHitResult result = arg_stageMeshCollision.lock()->CheckHitRay(index.GetPos(), echoVec.GetNormal());

			//当たっていたら
			if (!result.m_isHit || (result.m_isHit && distance <= fabs(result.m_distance))) {

				obj->m_isDrawFlag = true;
				break;
			}

		}
	}
}

KazMath::Transform3D StageManager::GetGoalTransform()
{
	return m_goal->m_transform;
}

int StageManager::GetEnemyCount()
{
	int l_enemyCount = 0;
	for (auto itr = m_enemys.begin(); itr != m_enemys.end(); ++itr)
	{
		if ((*itr)->m_enemyIndex > l_enemyCount)
		{
			l_enemyCount = (*itr)->m_enemyIndex;
		}
	}
	return l_enemyCount;
}

std::list<KazMath::Transform3D> StageManager::GetEnemyPositions(int f_enemyNum)
{
	std::list<KazMath::Transform3D> l_result;
	for (auto itr = m_enemys.begin(); itr != m_enemys.end(); ++itr)
	{
		if ((*itr)->m_enemyIndex == f_enemyNum)
		{
			l_result.push_back((*itr)->m_transform);
		}
	}
	return l_result;
}

int StageManager::GetTurretCount()
{
	return (int)m_turrets.size();
}
KazMath::Transform3D StageManager::GetTurretPosition(int f_turetNum)
{
	auto itr = m_turrets.begin();
	for (int i = 0; i < f_turetNum; i++)
	{
		itr++;
	}
	return (*itr)->m_transform;
}

void StageManager::ChangeScene(DrawingByRasterize& arg_rasterize)
{
	arg_rasterize.ReleasePipelineInScene();
	m_changeSceneTriggerFlag = true;
}
