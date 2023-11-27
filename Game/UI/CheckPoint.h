#pragma once
#include"../KazLibrary/Render/BasicDraw.h"

class CheckPoint
{
public:
	CheckPoint(DrawingByRasterize& arg_rasterize);

	void Init(const KazMath::Vec3<float>& arg_pointWorldPos);
	void Update();
	void Draw(DrawingByRasterize &arg_rasterize);

private:
	KazMath::Vec3<float>m_screenPos, m_worldPos;
	BasicDraw::BasicTextureRender m_render;
};

