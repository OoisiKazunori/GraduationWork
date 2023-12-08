#pragma once
#include"StageModel.h"
#include "../Game/Collision/MeshCollision.h"
/// <summary>
/// ステージ全体の管理処理
/// MapLoaderのデータなどをここで描画している
/// </summary>
class StageManager
{
private:
	int m_nowStageNumber, m_nextStageNumber;
	bool m_changeSceneTriggerFlag;
	//地形のモデル

	std::list<std::unique_ptr<StageModel>> m_tree;
	std::list<std::unique_ptr<StageModel>> m_cylinder;

	std::list<std::unique_ptr<StageModel>> m_block01;


	std::list<std::unique_ptr<StageModel>> m_plane;
	std::list<std::unique_ptr<StageModel>> m_Wall_C;
	std::list<std::unique_ptr<StageModel>> m_Wall_Four_Forked_Road;
	std::list<std::unique_ptr<StageModel>> m_Wall_River;
	std::list<std::unique_ptr<StageModel>> m_Wall_H;

	std::list<std::unique_ptr<StageModel>> m_Wall_I;
	std::list<std::unique_ptr<StageModel>> m_Wall_L_LPost;
	std::list<std::unique_ptr<StageModel>> m_Wall_NonPost;
	std::list<std::unique_ptr<StageModel>> m_Wall_L_Post;
	std::list<std::unique_ptr<StageModel>> m_Wall_T;

	 
	std::list<std::shared_ptr<MeshCollision>> m_collisions;
	//その他木等の外部オブジェクトモデル--------------------------------

	//ステージ切り替え前に必ず呼び出してください
	void ChangeScene(DrawingByRasterize& arg_rasterize);
	std::unique_ptr<StageModel> m_goal;
public:
	StageManager();

	void Init(DrawingByRasterize& arg_rasterize, int f_stageNum);
	void Update(DrawingByRasterize &arg_rasterize);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);
	//ステージが切り替わったトリガー
	bool ChangeSceneTrigger();
	void AddMapDatas(DrawingByRasterize& arg_rasterize, int f_stageNum);

	void CheckInEcho(std::weak_ptr<MeshCollision> arg_stageMeshCollision);

	
	std::list<std::shared_ptr<MeshCollision>> GetColliders(){ return m_collisions; };
	std::unique_ptr<StageModel> m_stage;
	KazMath::Transform3D GetGoalTransform() { return m_goal->m_transform; };
};

