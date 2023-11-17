#include "StageManager.h"
#include"Input/KeyBoradInputManager.h"
#include "../MapLoader/MapLoader.h"

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
	for (auto l_treeItr = m_tree.begin(); l_treeItr != m_tree.end(); ++l_treeItr)
	{
		(*l_treeItr)->Update();
	}
	for (auto l_stoneItr = m_stone.begin(); l_stoneItr != m_stone.end(); ++l_stoneItr)
	{
		(*l_stoneItr)->Update();
	}
	for (auto l_block01Itr = m_block01.begin(); l_block01Itr != m_block01.end(); ++l_block01Itr)
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
	for (auto l_treeItr = m_tree.begin(); l_treeItr != m_tree.end(); ++l_treeItr)
	{
		(*l_treeItr)->Draw(arg_rasterize, arg_blasVec);
	}
	for (auto l_stoneItr = m_stone.begin(); l_stoneItr != m_stone.end(); ++l_stoneItr)
	{
		(*l_stoneItr)->Draw(arg_rasterize, arg_blasVec);
	}
	for (auto l_block01Itr = m_block01.begin(); l_block01Itr != m_block01.end(); ++l_block01Itr)
	{
		(*l_block01Itr)->Draw(arg_rasterize, arg_blasVec);
	}

	for (auto itr = m_Wall_C.begin(); itr != m_Wall_C.end(); itr++)
	{
		(*itr)->Draw(arg_rasterize, arg_blasVec);
	}
	for (auto itr = m_Wall_Four_Forked_Road.begin(); itr != m_Wall_Four_Forked_Road.end(); itr++)
	{
		(*itr)->Draw(arg_rasterize, arg_blasVec);
	}
	for (auto itr = m_Wall_River.begin(); itr != m_Wall_River.end(); itr++)
	{
		(*itr)->Draw(arg_rasterize, arg_blasVec);
	}
	for (auto itr = m_Wall_H.begin(); itr != m_Wall_H.end(); itr++)
	{
		(*itr)->Draw(arg_rasterize, arg_blasVec);
	}
	for (auto itr = m_Wall_I.begin(); itr != m_Wall_I.end(); itr++)
	{
		(*itr)->Draw(arg_rasterize, arg_blasVec);
	}
	for (auto itr = m_Wall_L_LPost.begin(); itr != m_Wall_L_LPost.end(); itr++)
	{
		(*itr)->Draw(arg_rasterize, arg_blasVec);
	}
	for (auto itr = m_Wall_NonPost.begin(); itr != m_Wall_NonPost.end(); itr++)
	{
		(*itr)->Draw(arg_rasterize, arg_blasVec);
	}
	for (auto itr = m_Wall_L_Post.begin(); itr != m_Wall_L_Post.end(); itr++)
	{
		(*itr)->Draw(arg_rasterize, arg_blasVec);
	}
	for (auto itr = m_Wall_T.begin(); itr != m_Wall_T.end(); itr++)
	{
		(*itr)->Draw(arg_rasterize, arg_blasVec);
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
	m_tree.clear();
	m_stone.clear();
	m_stage.reset();

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
			m_stage = std::make_unique<StageModel>(arg_rasterize, "Resource/Stage/Stage/", "Stage.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale);
		}
		else if (l_mapItr->m_objetName.starts_with("Block01") == true)
		{
			m_block01.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/Stage/StageBlock01/", "StageBlock01.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale));
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
		}
		else if (l_mapItr->m_objetName.starts_with("wall_four-forked_road") == true)
		{
			m_Wall_Four_Forked_Road.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/MapObjects/Wall_Four_Forked_Road/", "Wall_Four_Forked_Road.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale));
		}
		else if (l_mapItr->m_objetName.starts_with("wall_River") == true)
		{
			m_Wall_River.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/MapObjects/Wall_River/", "Wall_River.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale));
		}
		else if (l_mapItr->m_objetName.starts_with("wallH") == true)
		{
			m_Wall_H.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/MapObjects/Wall_H/", "Wall_H.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale));
		}

		else if (l_mapItr->m_objetName.starts_with("wallI") == true)
		{
			m_Wall_I.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/MapObjects/Wall_I/", "Wall_I.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale));
		}
		else if (l_mapItr->m_objetName.starts_with("wallL_LPost") == true)
		{
			m_Wall_L_LPost.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/MapObjects/Wall_L_LPost/", "Wall_L_LPost.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale));
		}
		else if (l_mapItr->m_objetName.starts_with("wallL_NonPost") == true)
		{
			m_Wall_NonPost.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/MapObjects/Wall_L_NonPost/", "Wall_L_NonPost.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale));
		}
		else if (l_mapItr->m_objetName.starts_with("wallL_Post") == true)
		{
			m_Wall_L_Post.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/MapObjects/Wall_L_Post/", "Wall_L_Post.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale));
		}
		else if (l_mapItr->m_objetName.starts_with("wallT") == true)
		{
			m_Wall_T.push_back(std::make_unique<StageModel>(arg_rasterize, "Resource/MapObjects/Wall_T/", "Wall_T.gltf",
				l_mapItr->m_position, l_mapItr->m_rotition, l_mapItr->m_scale));
		}

	}
	//emptyなら入れる
	if (!m_stage)
	{
		m_stage = std::make_unique<StageModel>(arg_rasterize, "Resource/Stage/Stage/", "Stage.gltf",
			DirectX::XMFLOAT3(0.0f, 10.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(75.0f, 10.0f, 75.0f));
	}
}

void StageManager::ChangeScene(DrawingByRasterize& arg_rasterize)
{
	arg_rasterize.ReleasePipelineInScene();
	m_changeSceneTriggerFlag = true;
}
