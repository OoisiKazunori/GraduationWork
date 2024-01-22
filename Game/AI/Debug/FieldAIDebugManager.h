#pragma once
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../EnemyAIData.h"
#include"../KazLibrary/Helper/ISinglton.h"
#include"../KazLibrary/Render/BasicDraw.h"
#include"../Game/AI/Evaluation/FieldAI.h"

class FieldAIDebugManager :public ISingleton<FieldAIDebugManager>
{
public:
	FieldAIDebugManager();

	void Init(DrawingByRasterize& arg_rasterize);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blas);
	void DrawImGui();

	void SetGridColor(int x, int y, const KazMath::Color& arg_color);

private:

	BasicDraw::BasicModelInstanceRender m_modelInstanceRender;

	bool m_existenceEstablishmentMapFlag;
	bool m_filteringVisualizationFlag;
	bool m_congestedZoneFlag;

	std::vector<KazMath::Color>m_gridColorArray;
};

