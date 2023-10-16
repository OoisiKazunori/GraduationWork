#include "StageManager.h"
#include"Input/KeyBoradInputManager.h"
#include "../MapLoader/MapLoader.h"

StageManager::StageManager() :m_changeSceneTriggerFlag(false)
{
}

void StageManager::Init(DrawingByRasterize& arg_rasterize)
{
	m_nowStageNumber = 0;
	m_changeSceneTriggerFlag = false;

	//最初のステージはこちらで読み込む
	int stageNumber = 0;
	AddMapDatas(arg_rasterize, stageNumber);
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

void StageManager::AddMapDatas(DrawingByRasterize& arg_rasterize, int f_stageNum)
{
	m_tree.clear();
	m_stone.clear();
	m_stage.reset();
	std::list<MapObject> l_map = MapManager::GetStageData(f_stageNum);
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
			m_stage = std::make_unique<StageModel>(arg_rasterize, "Resource/Stage/", "StageBase.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale);
		}
	}
}

void StageManager::ChangeScene(DrawingByRasterize& arg_rasterize)
{
	arg_rasterize.ReleasePipelineInScene();
	m_changeSceneTriggerFlag = true;
}
