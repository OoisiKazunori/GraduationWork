#pragma once
#include"../KazLibrary/Loader/ModelLoader.h"
#include"../KazLibrary/Render/BasicDraw.h"

/// <summary>
/// ステージの大枠のパーツ
/// </summary>
class StageModel
{
public:
	StageModel(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName,
		DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3 rot = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);
	BasicDraw::BasicModelRender m_stageModelRender;
	KazMath::Transform3D m_transform;
	bool m_echoFlag = false;//エコーによるオブジェクト描画処理を有効化する
	int m_drawTimer = 120;
	bool m_isDrawFlag = false;
private:
	bool m_prevIsDrawFlag = false;

};

