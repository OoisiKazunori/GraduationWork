#pragma once
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../EnemyAIData.h"
#include"../KazLibrary/Helper/ISinglton.h"
#include"../KazLibrary/Render/BasicDraw.h"
#include"../Game/AI/Evaluation/FieldAI.h"
#include"../Game/Enemy/PatrolConfig.h"

class FieldAIDebugManager :public ISingleton<FieldAIDebugManager>
{
public:
	FieldAIDebugManager();

	void Init(DrawingByRasterize& arg_rasterize, const KazMath::Vec2<int>& arg_mapIDMaxSize, std::vector<std::vector<PatrolConfig::AstarData>>arg_patData);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blas);
	void DrawImGui();

	void SetGridColorForSearch(int arg_x, int arg_y, const KazMath::Color& arg_color);
	void SetGridColorForExistenceEstablishmentMap(int arg_x, int arg_y, const KazMath::Color& arg_color);
private:

	BasicDraw::BasicModelInstanceRender m_modelInstanceRender;
	int m_radioType;

	std::array<std::vector<KazMath::Color>,4>m_gridColorArray;
	KazMath::Vec2<int> m_mapChipMaxSize;

};

