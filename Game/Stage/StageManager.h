#pragma once
#include"StageModel.h"

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
	std::list<std::unique_ptr<StageModel>> m_stone;

	std::list<std::unique_ptr<StageModel>> m_block01;
	//その他木等の外部オブジェクトモデル--------------------------------

	//ステージ切り替え前に必ず呼び出してください
	void ChangeScene(DrawingByRasterize& arg_rasterize);
public:
	StageManager();

	void Init(DrawingByRasterize& arg_rasterize, int f_stageNum);
	void Update(DrawingByRasterize &arg_rasterize);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);
	//ステージが切り替わったトリガー
	bool ChangeSceneTrigger();
	void AddMapDatas(DrawingByRasterize& arg_rasterize, int f_stageNum);

	std::unique_ptr<StageModel> m_stage;
};

