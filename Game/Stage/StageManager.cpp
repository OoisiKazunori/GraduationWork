#include "StageManager.h"
#include"Input/KeyBoradInputManager.h"
#include "../MapLoader/MapLoader.h"
#include "../Echo/EchoArray.h"

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
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_4))
	{
		ChangeScene(arg_rasterize);
		//ステージの切り替え処理
		int changeSceneNum = 0;
		AddMapDatas(arg_rasterize, changeSceneNum);
	}

	//ステージの切り替え処理
	m_stage->Update();
	m_goal->Update();
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

	for (auto itr = m_Wall_C.begin(); itr != m_Wall_C.end(); itr++)
	{
		(*itr)->Update();
	}
	for (auto itr = m_Wall_Four_Forked_Road.begin(); itr != m_Wall_Four_Forked_Road.end(); itr++)
	{
		(*itr)->Update();
	}
	for (auto itr = m_Wall_River.begin(); itr != m_Wall_River.end(); itr++)
	{
		(*itr)->Update();
	}
	for (auto itr = m_Wall_H.begin(); itr != m_Wall_H.end(); itr++)
	{
		(*itr)->Update();
	}
	for (auto itr = m_Wall_I.begin(); itr != m_Wall_I.end(); itr++)
	{
		(*itr)->Update();
	}
	for (auto itr = m_Wall_L_LPost.begin(); itr != m_Wall_L_LPost.end(); itr++)
	{
		(*itr)->Update();
	}
	for (auto itr = m_Wall_NonPost.begin(); itr != m_Wall_NonPost.end(); itr++)
	{
		(*itr)->Update();
	}
	for (auto itr = m_Wall_L_Post.begin(); itr != m_Wall_L_Post.end(); itr++)
	{
		(*itr)->Update();
	}
	for (auto itr = m_Wall_T.begin(); itr != m_Wall_T.end(); itr++)
	{
		(*itr)->Update();
	}
	for (auto itr = m_plane.begin(); itr != m_plane.end(); itr++)
	{
		(*itr)->Update();
	}
}

void StageManager::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	m_stage->Draw(arg_rasterize, arg_blasVec);
	//m_goal->Draw(arg_rasterize, arg_blasVec);
	//for (auto l_treeItr = m_phone.begin(); l_treeItr != m_phone.end(); ++l_treeItr)
	//{
	//	(*l_treeItr)->Draw(arg_rasterize, arg_blasVec);
	//}
	//for (auto l_cylinderItr = m_cylinder.begin(); l_cylinderItr != m_cylinder.end(); ++l_cylinderItr)
	//{
	//	(*l_cylinderItr)->Draw(arg_rasterize, arg_blasVec);
	//}
	//for (auto l_block01Itr = m_block01.begin(); l_block01Itr != m_block01.end(); ++l_block01Itr)
	//{
	//	(*l_block01Itr)->Draw(arg_rasterize, arg_blasVec);
	//}
	//for (auto l_cylinderItr = m_stone.begin(); l_cylinderItr != m_stone.end(); ++l_cylinderItr)
	//{
	//	(*l_cylinderItr)->Draw(arg_rasterize, arg_blasVec);
	//}

	//for (auto itr = m_Wall_C.begin(); itr != m_Wall_C.end(); itr++)
	//{
	//	(*itr)->Draw(arg_rasterize, arg_blasVec);
	//}
	//for (auto itr = m_Wall_Four_Forked_Road.begin(); itr != m_Wall_Four_Forked_Road.end(); itr++)
	//{
	//	(*itr)->Draw(arg_rasterize, arg_blasVec);
	//}
	//for (auto itr = m_Wall_River.begin(); itr != m_Wall_River.end(); itr++)
	//{
	//	(*itr)->Draw(arg_rasterize, arg_blasVec);
	//}
	//for (auto itr = m_Wall_H.begin(); itr != m_Wall_H.end(); itr++)
	//{
	//	(*itr)->Draw(arg_rasterize, arg_blasVec);
	//}
	//for (auto itr = m_Wall_I.begin(); itr != m_Wall_I.end(); itr++)
	//{
	//	(*itr)->Draw(arg_rasterize, arg_blasVec);
	//}
	//for (auto itr = m_Wall_L_LPost.begin(); itr != m_Wall_L_LPost.end(); itr++)
	//{
	//	(*itr)->Draw(arg_rasterize, arg_blasVec);
	//}
	//for (auto itr = m_Wall_NonPost.begin(); itr != m_Wall_NonPost.end(); itr++)
	//{
	//	(*itr)->Draw(arg_rasterize, arg_blasVec);
	//}
	//for (auto itr = m_Wall_L_Post.begin(); itr != m_Wall_L_Post.end(); itr++)
	//{
	//	(*itr)->Draw(arg_rasterize, arg_blasVec);
	//}
	//for (auto itr = m_Wall_T.begin(); itr != m_Wall_T.end(); itr++)
	//{
	//	(*itr)->Draw(arg_rasterize, arg_blasVec);
	//}
	//for (auto itr = m_plane.begin(); itr != m_plane.end(); itr++)
	//{
	//	(*itr)->Draw(arg_rasterize, arg_blasVec);
	//}
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
	m_stage.reset();
	m_goal.reset();
	m_block01.clear();

	m_plane.clear();
	m_Wall_C.clear();
	m_Wall_Four_Forked_Road.clear();
	m_Wall_River.clear();
	m_Wall_H.clear();

	m_Wall_I.clear();
	m_Wall_L_LPost.clear();
	m_Wall_NonPost.clear();
	m_Wall_L_Post.clear();
	m_Wall_T.clear();

	std::list<MapObject> l_map = MapManager::GetStageData(f_stageNum);
	for (auto l_mapItr = l_map.begin(); l_mapItr != l_map.end(); ++l_mapItr)
	{
		if (l_mapItr->m_objetName.starts_with("phone") == true)
		{
			m_phone.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/tree/", "tree2.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale, l_mapItr->echoScale));

			auto collision = std::make_shared<MeshCollision>();
			collision->Setting((*m_phone.begin())->m_stageModelRender.m_model.m_modelInfo->modelData[0].vertexData, (*m_phone.begin())->m_transform);
			m_collisions.push_back(collision);
		}
		else if (l_mapItr->m_objetName.starts_with("stone"))
		{

			m_stone.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/Weapon/Rock/", "Rock.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale, l_mapItr->echoScale));

			m_stone.back()->m_echoFlag = true;
		}

		else if (l_mapItr->m_objetName.starts_with("cylinder") == true)
		{
			m_cylinder.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/MapObjects/cylinder/", "cylinder.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale));

			m_cylinder.back()->m_echoFlag = true;

			auto collision = std::make_shared<MeshCollision>();
			collision->Setting((*m_cylinder.begin())->m_stageModelRender.m_model.m_modelInfo->modelData[0].vertexData, (*--m_cylinder.end())->m_transform);
			m_collisions.push_back(collision);
		}
		else if (l_mapItr->m_objetName.starts_with("stage") == true)
		{
			m_stage = std::make_unique<StageModel>(arg_rasterize, "Resource/Stage/Stage/", "Stage.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale);
		}
		else if (l_mapItr->m_objetName.starts_with("Goal") == true)
		{
			m_goal = std::make_unique<StageModel>(arg_rasterize, "Resource/GoalTest/", "stageObjects1.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale);
		}
		else if (l_mapItr->m_objetName.starts_with("Block") == true)
		{
			auto pos = l_mapItr->m_position;
			float a = 0.5f;
			auto sca = l_mapItr->m_scale;
			float b = 0.5f;
			sca.x = sca.x * b;
			sca.y = sca.y * (b);
			sca.z = sca.z * (b);


			m_block01.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/GoalTest/", "stageObjects1.gltf",
				pos, l_mapItr->m_rotition, sca));

			m_block01.back()->m_echoFlag = true;

		}

		else if (l_mapItr->m_objetName.starts_with("plane") == true)
		{
			m_plane.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/MapObjects/Plane/", "Plane.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale));
		}
		else if (l_mapItr->m_objetName.starts_with("wallC") == true)
		{
			m_Wall_C.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/MapObjects/Wall_C/", "Wall_C.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale));



			auto collision = std::make_shared<MeshCollision>();
			auto trans = (*--m_Wall_C.end())->m_transform;
			collision->Setting((*m_Wall_C.begin())->m_stageModelRender.m_model.m_modelInfo->modelData[0].vertexData, (*--m_Wall_C.end())->m_transform);
			m_collisions.push_back(collision);
		}
		else if (l_mapItr->m_objetName.starts_with("wall_four-forked_road") == true)
		{
			m_Wall_Four_Forked_Road.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/MapObjects/Wall_Four_Forked_Road/", "Wall_Four_Forked_Road.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale));

			auto collision = std::make_shared<MeshCollision>();
			collision->Setting((*m_Wall_Four_Forked_Road.begin())->m_stageModelRender.m_model.m_modelInfo->modelData[0].vertexData, (*--m_Wall_Four_Forked_Road.end())->m_transform);
			m_collisions.push_back(collision);
		}
		else if (l_mapItr->m_objetName.starts_with("wall_River") == true)
		{
			m_Wall_River.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/MapObjects/Wall_River/", "Wall_River.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale));

			auto collision = std::make_shared<MeshCollision>();
			collision->Setting((*m_Wall_River.begin())->m_stageModelRender.m_model.m_modelInfo->modelData[0].vertexData, (*--m_Wall_River.end())->m_transform);
			m_collisions.push_back(collision);
		}
		else if (l_mapItr->m_objetName.starts_with("wallH") == true)
		{
			m_Wall_H.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/MapObjects/Wall_H/", "Wall_H.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale));

			auto collision = std::make_shared<MeshCollision>();
			collision->Setting((*m_Wall_H.begin())->m_stageModelRender.m_model.m_modelInfo->modelData[0].vertexData, (*--m_Wall_H.end())->m_transform);
			m_collisions.push_back(collision);
		}

		else if (l_mapItr->m_objetName.starts_with("wallI") == true)
		{
			m_Wall_I.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/MapObjects/Wall_I/", "Wall_I.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale));

			auto collision = std::make_shared<MeshCollision>();
			collision->Setting((*m_Wall_I.begin())->m_stageModelRender.m_model.m_modelInfo->modelData[0].vertexData, (*--m_Wall_I.end())->m_transform);
			m_collisions.push_back(collision);
		}
		else if (l_mapItr->m_objetName.starts_with("wallL_LPost") == true)
		{
			m_Wall_L_LPost.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/MapObjects/Wall_L_LPost/", "Wall_L_LPost.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale));

			auto collision = std::make_shared<MeshCollision>();
			collision->Setting((*m_Wall_L_LPost.begin())->m_stageModelRender.m_model.m_modelInfo->modelData[0].vertexData, (*--m_Wall_L_LPost.end())->m_transform);
			m_collisions.push_back(collision);
		}
		else if (l_mapItr->m_objetName.starts_with("wallL_NonPost") == true)
		{
			m_Wall_NonPost.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/MapObjects/Wall_L_NonPost/", "Wall_L_NonPost.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale));

			auto collision = std::make_shared<MeshCollision>();
			collision->Setting((*m_Wall_NonPost.begin())->m_stageModelRender.m_model.m_modelInfo->modelData[0].vertexData, (*--m_Wall_NonPost.end())->m_transform);
			m_collisions.push_back(collision);
		}
		else if (l_mapItr->m_objetName.starts_with("Wall_L_Post") == true)
		{
			m_Wall_L_Post.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/MapObjects/Wall_L_Post/", "Wall_L_Post.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale));

			auto collision = std::make_shared<MeshCollision>();
			collision->Setting((*m_Wall_L_Post.begin())->m_stageModelRender.m_model.m_modelInfo->modelData[0].vertexData, (*--m_Wall_L_Post.end())->m_transform);
			m_collisions.push_back(collision);
		}
		else if (l_mapItr->m_objetName.starts_with("wallT") == true)
		{
			m_Wall_T.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/MapObjects/Wall_T/", "Wall_T.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale));

			auto collision = std::make_shared<MeshCollision>();
			collision->Setting((*m_Wall_T.begin())->m_stageModelRender.m_model.m_modelInfo->modelData[0].vertexData, (*--m_Wall_T.end())->m_transform);
			m_collisions.push_back(collision);
		}
	}
	////emptyなら入れる
	//if (!m_stage)
	//{
	//	m_stage = std::make_unique<StageModel>(arg_rasterize, "Resource/Stage/Stage/", "Stage.gltf",
	//		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(75.0f, 14.4f, 75.0f));
	//	auto collision = std::make_shared<MeshCollision>();
	//	collision->Setting(m_stage->m_stageModelRender.m_model.m_modelInfo->modelData[0].vertexData, m_stage->m_transform);
	//	m_collisions.push_back(collision);
	//}

		m_stage = std::make_unique<StageModel>(arg_rasterize, "Resource/Stage/Test/", "Project_S_NewStage_0115_Test.gltf",
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
		auto collision = std::make_shared<MeshCollision>();
		collision->Setting(m_stage->m_stageModelRender.m_model.m_modelInfo->modelData[0].vertexData, m_stage->m_transform);
		m_collisions.push_back(collision);
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

void StageManager::ChangeScene(DrawingByRasterize& arg_rasterize)
{
	arg_rasterize.ReleasePipelineInScene();
	m_changeSceneTriggerFlag = true;
}
