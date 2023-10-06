#pragma once
#include"StageModel.h"

/// <summary>
/// ステージ全体の管理処理
/// </summary>
class StageManager
{
public:
	StageManager();

	void Init(DrawingByRasterize& arg_rasterize);
	void Update(DrawingByRasterize &arg_rasterize);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);
	//ステージが切り替わったトリガー
	bool ChangeSceneTrigger();
private:
	int m_nowStageNumber, m_nextStageNumber;
	bool m_changeSceneTriggerFlag;
	//地形のモデル
	std::unique_ptr<StageModel>m_stage;

	//その他木等の外部オブジェクトモデル--------------------------------

	//ステージ切り替え前に必ず呼び出してください
	void ChangeScene(DrawingByRasterize&arg_rasterize);
};

