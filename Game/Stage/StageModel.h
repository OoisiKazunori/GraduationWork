#pragma once
#include"../KazLibrary/Loader/ModelLoader.h"
#include"../KazLibrary/Render/BasicDraw.h"
#include"../Game/Echo/EchoArray.h"

/// <summary>
/// ステージの大枠のパーツ
/// </summary>
class StageModel
{
public:
	StageModel(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName, bool f_useOffset,
		DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3 rot = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), float f_echo = 0.0f);
	StageModel(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName, bool f_useOffset, int enemyIndex, int roadIndex,
		DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3 rot = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), float f_echo = 0.0f);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);
	BasicDraw::BasicModelRender m_stageModelRender;
	KazMath::Transform3D m_transform;
	bool m_echoFlag = false;//エコーによるオブジェクト描画処理を有効化する
	int m_drawTimer = 0;
	bool m_isDrawFlag = false;

	float m_echoRad = 0.0f;
	int m_enemyIndex = -1;
	int m_roadIndex = -1;
private:
	bool m_prevIsDrawFlag = false;
	
	bool m_isSeen = false;
	int m_seenTime = 0;
	//見た判定の時間判定
	const int SeenTime = 300;
};

