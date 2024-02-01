#pragma once
#include"StageModel.h"
#include "../Game/Collision/MeshCollision.h"

class StageNumCounter
{
public:
	static int stageNumCount;
};
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

	std::list<std::unique_ptr<StageModel>> m_phone;

	std::list<std::unique_ptr<StageModel>> m_cylinder;

	std::list<std::unique_ptr<StageModel>> m_block01;
	std::list<std::unique_ptr<StageModel>> m_enemys;
	std::list<std::unique_ptr<StageModel>> m_turets;

	std::unique_ptr<StageModel> m_Bird1;
	std::unique_ptr<StageModel> m_Bird2;
	std::unique_ptr<StageModel> m_Bird3;

	std::list<std::unique_ptr<StageModel>> m_plane;



	std::list<std::shared_ptr<MeshCollision>> m_collisions[3];
	//その他木等の外部オブジェクトモデル--------------------------------

	//ステージ切り替え前に必ず呼び出してください
	void ChangeScene(DrawingByRasterize& arg_rasterize);
	std::unique_ptr<StageModel> m_goal;
public:
	StageManager();

	void Init(DrawingByRasterize& arg_rasterize, int f_stageNum);
	void Update(DrawingByRasterize& arg_rasterize);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);
	//ステージが切り替わったトリガー
	bool ChangeSceneTrigger();
	void AddMapDatas(DrawingByRasterize& arg_rasterize, int f_stageNum);

	void CheckInEcho(std::weak_ptr<MeshCollision> arg_stageMeshCollision);

	std::list<std::unique_ptr<StageModel>> m_stone;
	std::list<std::unique_ptr<StageModel>> m_magazin;

	std::list<std::shared_ptr<MeshCollision>> GetColliders() { return m_collisions[m_nowStageNumber]; };
	std::unique_ptr<StageModel> m_stage[3];
	std::unique_ptr<StageModel> m_colStage[3];
	std::unique_ptr<StageModel> m_player;
	KazMath::Transform3D GetGoalTransform();
	//何体いるか1からカウントされる
	int GetEnemyCount();
	//GetEnemyCount - 1を引数にすると丁度いいかも
	std::list<KazMath::Transform3D> GetEnemyPositions(int f_enemyNum);
	//タレット用
	int GetTuretCount();
	KazMath::Transform3D GetTuretPosition(int f_turetNum);
};

