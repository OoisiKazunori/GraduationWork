#include "StageManager.h"
#include"Input/KeyBoradInputManager.h"
#include "../MapLoader/MapLoader.h"

StageManager::StageManager() :m_changeSceneTriggerFlag(false)
{
}

void StageManager::Init(DrawingByRasterize& arg_rasterize)
{
	m_nowStageNumber = 0;
	m_nextStageNumber = 0;
	m_changeSceneTriggerFlag = false;
	//最初のステージはこちらで読み込む
	std::list<MapObject> l_map = MapManager::GetStageData(2);
	for (auto l_mapItr = l_map.begin(); l_mapItr != l_map.end(); ++l_mapItr)
	{
		if (l_mapItr->m_objetName.starts_with("tree") == true)
		{
			m_tree.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/tree/", "tree2.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale));
		}
		else if (l_mapItr->m_objetName.starts_with("stone") == true)
		{
			m_stone.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/stone/", "stone.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale));
		}
		else if (l_mapItr->m_objetName.starts_with("stage") == true)
		{
			m_stage = std::make_unique<StageModel>(arg_rasterize, "Resource/Stage/", "Stage.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale);
		}
	}
	//playerのポジションが取れることを確認
	auto hoge = MapManager::GetPlayerStartPosition(2);
}

void StageManager::Update(DrawingByRasterize& arg_rasterize)
{
	//if (m_nowStageNumber != m_nextStageNumber)
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_4))
	{
		ChangeScene(arg_rasterize);
		//ステージの切り替え処理
		m_stage.reset();
		m_stage = std::make_unique<StageModel>(arg_rasterize, "Resource/Stage/", "Stage.gltf");
	}

	//ステージの切り替え処理
	m_stage->Update();
	for (auto l_treeItr = m_tree.begin(); l_treeItr != m_tree.end(); ++l_treeItr)
	{
		(*l_treeItr)->Update();
	}
	for (auto l_stoneItr = m_stone.begin(); l_stoneItr != m_stone.end(); ++l_stoneItr)
	{
		(*l_stoneItr)->Update();
	}
}

void StageManager::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	m_stage->Draw(arg_rasterize, arg_blasVec);
	for (auto l_treeItr = m_tree.begin(); l_treeItr != m_tree.end(); ++l_treeItr)
	{
		(*l_treeItr)->Draw(arg_rasterize, arg_blasVec);
	}
	for (auto l_stoneItr = m_stone.begin(); l_stoneItr != m_stone.end(); ++l_stoneItr)
	{
		(*l_stoneItr)->Draw(arg_rasterize, arg_blasVec);
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

void StageManager::ChangeScene(DrawingByRasterize& arg_rasterize)
{
	arg_rasterize.ReleasePipelineInScene();
	m_changeSceneTriggerFlag = true;
}
