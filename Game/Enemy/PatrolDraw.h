#pragma once
#include"../KazLibrary/Render/BasicDraw.h"
#include "PatrolConfig.h"

class PatrolDraw
{
private:
	KazMath::Color m_baseColor;
	bool m_isInput = false;
	bool m_isCheck = false;

private:
	std::weak_ptr<PatrolConfig> m_patrolConfig;
	std::vector<std::vector<std::unique_ptr<
		BasicDraw::BasicModelRender>>> m_modelRenders;

public:
	PatrolDraw();
	~PatrolDraw();
	void Init();
	void Update();
	void Draw(
		DrawingByRasterize& arg_rasterize,
		Raytracing::BlasVector& arg_blasVec);

public:
	void SetData(
		DrawingByRasterize& arg_rasterize,
		std::weak_ptr<PatrolConfig> arg_config);

private:
	void Input();
};

